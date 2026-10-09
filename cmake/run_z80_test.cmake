# Assemble one BASIC program with the Z80 backend and compare stdout.
# Required: BASALT, Z80ASM, CPM, SOURCE, EXPECTED, WORKDIR
# CPM is a command prefix; the generated .com path is appended.

file(MAKE_DIRECTORY "${WORKDIR}")

set(generated_asm "${WORKDIR}/prog.asm")
set(generated_com "${WORKDIR}/prog.com")
set(raw_out "${WORKDIR}/raw.txt")
set(actual_out "${WORKDIR}/actual.out")

execute_process(
  COMMAND "${BASALT}" -a z80 -o "${generated_asm}" "${SOURCE}"
  RESULT_VARIABLE compile_rc
  OUTPUT_VARIABLE compile_out
  ERROR_VARIABLE compile_err
)
if(NOT compile_rc EQUAL 0)
  message(FATAL_ERROR "basalt -a z80 failed (${compile_rc})\n${compile_out}\n${compile_err}")
endif()

execute_process(
  COMMAND "${Z80ASM}" -o "${generated_com}" "${generated_asm}"
  RESULT_VARIABLE asm_rc
  OUTPUT_VARIABLE asm_out
  ERROR_VARIABLE asm_err
)
if(NOT asm_rc EQUAL 0)
  message(FATAL_ERROR "z80asm failed (${asm_rc})\n${asm_out}\n${asm_err}")
endif()

separate_arguments(CPM_ARGS NATIVE_COMMAND "${CPM}")
if(DEFINED INPUT AND NOT INPUT STREQUAL "" AND EXISTS "${INPUT}")
  execute_process(
    COMMAND ${CPM_ARGS} "${generated_com}"
    INPUT_FILE "${INPUT}"
    OUTPUT_FILE "${raw_out}"
    ERROR_VARIABLE run_err
    RESULT_VARIABLE run_rc
  )
else()
  execute_process(
    COMMAND ${CPM_ARGS} "${generated_com}"
    OUTPUT_FILE "${raw_out}"
    ERROR_VARIABLE run_err
    RESULT_VARIABLE run_rc
  )
endif()
if(NOT run_rc EQUAL 0)
  message(FATAL_ERROR "CP/M run failed (${run_rc})\n${run_err}")
endif()

file(READ "${raw_out}" raw_text)
string(REPLACE "\r\n" "\n" raw_text "${raw_text}")
string(REPLACE "\r" "\n" raw_text "${raw_text}")

set(filtered "")
set(seen_start FALSE)
string(REGEX REPLACE "\n$" "" raw_text "${raw_text}")
string(REPLACE "\n" ";" lines "${raw_text}")
foreach(line IN LISTS lines)
  if(seen_start)
    string(APPEND filtered "${line}\n")
  elseif(line MATCHES "^--START")
    set(seen_start TRUE)
  endif()
endforeach()

if(NOT seen_start)
  message(FATAL_ERROR "program did not print --START--\n${raw_text}")
endif()

file(WRITE "${actual_out}" "${filtered}")

execute_process(
  COMMAND "${CMAKE_COMMAND}" -E compare_files "${actual_out}" "${EXPECTED}"
  RESULT_VARIABLE diff_rc
)
if(NOT diff_rc EQUAL 0)
  file(READ "${EXPECTED}" expected_text)
  message(FATAL_ERROR
    "Z80 output mismatch for ${SOURCE}\n--- actual ---\n${filtered}--- expected ---\n${expected_text}")
endif()
