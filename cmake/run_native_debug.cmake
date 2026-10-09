# Compile with -g and check that DWARF records the BASIC line and its text.
# Required: BASALT, DWARFDUMP, SOURCE, WORKDIR

file(MAKE_DIRECTORY "${WORKDIR}")
set(generated_bin "${WORKDIR}/prog")

execute_process(
  COMMAND "${BASALT}" -g -o "${generated_bin}" "${SOURCE}"
  RESULT_VARIABLE compile_rc
  ERROR_VARIABLE compile_err
)
if(NOT compile_rc EQUAL 0)
  message(FATAL_ERROR "basalt -g failed (${compile_rc})\n${compile_err}")
endif()

execute_process(
  COMMAND "${generated_bin}"
  OUTPUT_VARIABLE program_out
  RESULT_VARIABLE run_rc
)
if(NOT run_rc EQUAL 0)
  message(FATAL_ERROR "debug build failed to run (${run_rc})")
endif()
if(NOT program_out MATCHES "hello, world")
  message(FATAL_ERROR "debug build produced unexpected output:\n${program_out}")
endif()

execute_process(
  COMMAND "${DWARFDUMP}" --debug-info --debug-line "${generated_bin}.dSYM"
  OUTPUT_VARIABLE dwarf
  ERROR_VARIABLE dwarf_err
  RESULT_VARIABLE dwarf_rc
)
if(NOT dwarf_rc EQUAL 0)
  message(FATAL_ERROR "dwarfdump failed (${dwarf_rc})\n${dwarf_err}")
endif()
if(NOT dwarf MATCHES "10 print")
  message(FATAL_ERROR "debug info is missing the source statement\n${dwarf}")
endif()
if(NOT dwarf MATCHES "hello, world")
  message(FATAL_ERROR "debug info is missing the source statement text\n${dwarf}")
endif()
if(NOT dwarf MATCHES "test_1.bas")
  message(FATAL_ERROR "debug info is missing the source file name\n${dwarf}")
endif()
