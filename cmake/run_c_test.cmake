# Compile one BASIC program with the ANSI C backend and compare stdout.
# Required: BASALT, CC, SOURCE, EXPECTED, WORKDIR
# Optional: INPUT (file fed to stdin), ARCH (default ansi-c)

if(NOT DEFINED ARCH OR ARCH STREQUAL "")
  set(ARCH "ansi-c")
endif()

file(MAKE_DIRECTORY "${WORKDIR}")

set(generated_c "${WORKDIR}/prog.c")
set(generated_bin "${WORKDIR}/prog")
set(raw_out "${WORKDIR}/raw.txt")
set(actual_out "${WORKDIR}/actual.out")

execute_process(
  COMMAND "${BASALT}" -a "${ARCH}" -o "${generated_c}" "${SOURCE}"
  RESULT_VARIABLE compile_rc
  OUTPUT_VARIABLE compile_out
  ERROR_VARIABLE compile_err
)
if(NOT compile_rc EQUAL 0)
  message(FATAL_ERROR "basalt failed (${compile_rc})\n${compile_out}\n${compile_err}")
endif()

execute_process(
  COMMAND "${CC}" -std=c11 -lm -o "${generated_bin}" "${generated_c}"
  RESULT_VARIABLE cc_rc
  OUTPUT_VARIABLE cc_out
  ERROR_VARIABLE cc_err
)
if(NOT cc_rc EQUAL 0)
  file(READ "${generated_c}" generated_text)
  message(FATAL_ERROR "C compile failed (${cc_rc})\n${cc_out}\n${cc_err}\n--- generated ---\n${generated_text}")
endif()

set(run_args)
if(DEFINED INPUT AND NOT INPUT STREQUAL "" AND EXISTS "${INPUT}")
  execute_process(
    COMMAND "${generated_bin}"
    INPUT_FILE "${INPUT}"
    OUTPUT_FILE "${raw_out}"
    ERROR_VARIABLE run_err
    RESULT_VARIABLE run_rc
  )
else()
  execute_process(
    COMMAND "${generated_bin}"
    OUTPUT_FILE "${raw_out}"
    ERROR_VARIABLE run_err
    RESULT_VARIABLE run_rc
  )
endif()
if(NOT run_rc EQUAL 0)
  message(FATAL_ERROR "program failed (${run_rc})\n${run_err}")
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
    "output mismatch for ${SOURCE}\n--- actual ---\n${filtered}--- expected ---\n${expected_text}")
endif()
