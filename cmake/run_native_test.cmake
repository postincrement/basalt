# Compile one BASIC program to a host executable and compare stdout.
# Required: BASALT, SOURCE, EXPECTED, WORKDIR
# Optional: INPUT

file(MAKE_DIRECTORY "${WORKDIR}")

set(generated_bin "${WORKDIR}/prog")
set(raw_out "${WORKDIR}/raw.txt")
set(actual_out "${WORKDIR}/actual.out")

execute_process(
  COMMAND "${BASALT}" -a native -o "${generated_bin}" "${SOURCE}"
  RESULT_VARIABLE compile_rc
  OUTPUT_VARIABLE compile_out
  ERROR_VARIABLE compile_err
)
if(NOT compile_rc EQUAL 0)
  message(FATAL_ERROR "basalt -a native failed (${compile_rc})\n${compile_out}\n${compile_err}")
endif()

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
    "native output mismatch for ${SOURCE}\n--- actual ---\n${filtered}--- expected ---\n${expected_text}")
endif()
