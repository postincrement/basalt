if(NOT DEFINED BASALT OR NOT DEFINED SOURCE)
  message(FATAL_ERROR "BASALT and SOURCE are required")
endif()
file(MAKE_DIRECTORY "${WORKDIR}")
execute_process(
  COMMAND "${BASALT}" -a ansi-c -o "${WORKDIR}/fail.c" "${SOURCE}"
  RESULT_VARIABLE rc
  OUTPUT_VARIABLE out
  ERROR_VARIABLE err
)
if(rc EQUAL 0)
  message(FATAL_ERROR "expected basalt to reject ${SOURCE}\n${out}\n${err}")
endif()
if(NOT err MATCHES "RUN is not supported")
  message(FATAL_ERROR "missing RUN diagnostic\n${err}")
endif()
