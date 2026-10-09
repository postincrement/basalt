cmake_minimum_required(VERSION 3.16)
project(basalt LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(FLEX REQUIRED)

# macOS ships Bison 2.3, which cannot parse this grammar. Prefer a keg-only
# Homebrew Bison 3 when the default one is too old.
if(NOT BISON_EXECUTABLE)
  find_program(_BASALT_BISON_NEW bison
    PATHS /opt/homebrew/opt/bison/bin /usr/local/opt/bison/bin
    NO_DEFAULT_PATH)
  if(_BASALT_BISON_NEW)
    set(BISON_EXECUTABLE "${_BASALT_BISON_NEW}" CACHE FILEPATH "GNU Bison 3 or newer")
  endif()
endif()
find_package(BISON REQUIRED)
if(BISON_VERSION VERSION_LESS 3.0)
  message(FATAL_ERROR
    "basalt needs GNU Bison 3 or newer (found ${BISON_VERSION} at ${BISON_EXECUTABLE}). "
    "On macOS: brew install bison")
endif()

set(BASALT_GENERATED_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated)
file(MAKE_DIRECTORY ${BASALT_GENERATED_DIR})

BISON_TARGET(mbasic ${CMAKE_CURRENT_SOURCE_DIR}/mbasic.ypp
  ${BASALT_GENERATED_DIR}/mbasic.tab.cc
  DEFINES_FILE ${BASALT_GENERATED_DIR}/mbasic.tab.hpp)
FLEX_TARGET(mbasic ${CMAKE_CURRENT_SOURCE_DIR}/mbasic.l
  ${BASALT_GENERATED_DIR}/mbasic.lex.cc)
ADD_FLEX_BISON_DEPENDENCY(mbasic mbasic)

set(BASALT_SOURCES
  basalt.cc
  ast.cc
  codegen.cc
  outputgen.cc
  common.cc
  c/c_codegen.cc
  c/c_runtime.cc
  z80/z80_codegen.cc
  pretty/pretty_codegen.cc
  ${BISON_mbasic_OUTPUTS}
  ${FLEX_mbasic_OUTPUTS}
)

option(BASALT_LLVM "Build the LLVM IR backend" OFF)
if(BASALT_LLVM)
  find_package(LLVM REQUIRED CONFIG)
  message(STATUS "Using LLVM ${LLVM_PACKAGE_VERSION} from ${LLVM_DIR}")
  list(APPEND BASALT_SOURCES llvm/llvm_codegen.cc llvm/native_codegen.cc)
endif()

add_executable(basalt ${BASALT_SOURCES})

target_include_directories(basalt PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${BASALT_GENERATED_DIR}
)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU|AppleClang")
  set_source_files_properties(${FLEX_mbasic_OUTPUTS} PROPERTIES
    COMPILE_OPTIONS "-Wno-unused-function;-Wno-sign-compare;-Wno-deprecated-register")
endif()

if(BASALT_LLVM)
  llvm_map_components_to_libnames(LLVM_LIBS core support native irreader passes)
  target_include_directories(basalt PRIVATE ${LLVM_INCLUDE_DIRS})
  target_compile_definitions(basalt PRIVATE BASALT_WITH_LLVM ${LLVM_DEFINITIONS})
  target_link_libraries(basalt PRIVATE ${LLVM_LIBS})

  set(BASALT_RUNTIME_OBJECT ${CMAKE_CURRENT_BINARY_DIR}/basaltrt.o)
  add_custom_command(
    OUTPUT ${BASALT_RUNTIME_OBJECT}
    COMMAND ${CMAKE_C_COMPILER} -c -O2 -ffunction-sections -fdata-sections
            -o ${BASALT_RUNTIME_OBJECT}
            ${CMAKE_CURRENT_SOURCE_DIR}/basaltrt.c
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/basaltrt.c
    COMMENT "Compile BASIC runtime")
  add_custom_target(basaltrt_obj DEPENDS ${BASALT_RUNTIME_OBJECT})
  add_dependencies(basalt basaltrt_obj)
  target_compile_definitions(basalt PRIVATE
    "BASALT_RUNTIME_OBJECT=\"${BASALT_RUNTIME_OBJECT}\"")
  if(APPLE)
    execute_process(COMMAND xcrun --show-sdk-path
      OUTPUT_VARIABLE BASALT_SYSROOT OUTPUT_STRIP_TRAILING_WHITESPACE)
    execute_process(COMMAND xcrun --show-sdk-version
      OUTPUT_VARIABLE BASALT_SDK_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE)
    execute_process(COMMAND sw_vers -productVersion
      OUTPUT_VARIABLE BASALT_MACOS_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE)
    execute_process(COMMAND xcrun --find ld
      OUTPUT_VARIABLE BASALT_LD OUTPUT_STRIP_TRAILING_WHITESPACE)
    execute_process(COMMAND xcrun --find dsymutil
      OUTPUT_VARIABLE BASALT_DSYMUTIL OUTPUT_STRIP_TRAILING_WHITESPACE)
    if(BASALT_SYSROOT STREQUAL "" OR BASALT_LD STREQUAL "")
      message(FATAL_ERROR "native executables need the macOS SDK and ld (xcrun)")
    endif()
    target_compile_definitions(basalt PRIVATE
      "BASALT_SYSROOT=\"${BASALT_SYSROOT}\""
      "BASALT_SDK_VERSION=\"${BASALT_SDK_VERSION}\""
      "BASALT_MACOS_VERSION=\"${BASALT_MACOS_VERSION}\""
      "BASALT_LD=\"${BASALT_LD}\""
      "BASALT_DSYMUTIL=\"${BASALT_DSYMUTIL}\"")
  endif()
  if(APPLE)
    # LLVM's exported link line sometimes needs libz.
    find_package(ZLIB)
    if(ZLIB_FOUND)
      target_link_libraries(basalt PRIVATE ZLIB::ZLIB)
    endif()
  endif()
endif()

enable_testing()

file(GLOB BASALT_BASIC_TESTS CONFIGURE_DEPENDS
  ${CMAKE_CURRENT_SOURCE_DIR}/verify/test_*.bas)

foreach(test_src ${BASALT_BASIC_TESTS})
  get_filename_component(test_name ${test_src} NAME_WE)
  set(expected ${CMAKE_CURRENT_SOURCE_DIR}/verify/expected/${test_name}.out)
  set(input_file ${CMAKE_CURRENT_SOURCE_DIR}/verify/expected/${test_name}.in)
  if(NOT EXISTS ${input_file})
    set(input_file "")
  endif()
  add_test(NAME c_${test_name}
    COMMAND ${CMAKE_COMMAND}
      -DBASALT=$<TARGET_FILE:basalt>
      -DCC=${CMAKE_C_COMPILER}
      -DSOURCE=${test_src}
      -DEXPECTED=${expected}
      -DINPUT=${input_file}
      -DARCH=ansi-c
      -DWORKDIR=${CMAKE_CURRENT_BINARY_DIR}/ctest/c/${test_name}
      -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/run_c_test.cmake)
endforeach()

add_test(NAME c_run_unsupported
  COMMAND ${CMAKE_COMMAND}
    -DBASALT=$<TARGET_FILE:basalt>
    -DSOURCE=${CMAKE_CURRENT_SOURCE_DIR}/verify/fail_run.bas
    -DWORKDIR=${CMAKE_CURRENT_BINARY_DIR}/ctest/c/fail_run
    -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/expect_fail.cmake)

option(BASALT_Z80_TESTS "Run Z80 backend tests (needs z80asm and a CP/M runner)" OFF)
set(BASALT_Z80ASM "" CACHE FILEPATH "Path to z80asm")
set(BASALT_CPM "" CACHE STRING "CP/M runner command prefix; the .com path is appended")

if(BASALT_Z80_TESTS)
  if(BASALT_Z80ASM STREQUAL "" OR BASALT_CPM STREQUAL "")
    message(FATAL_ERROR "BASALT_Z80_TESTS requires BASALT_Z80ASM and BASALT_CPM")
  endif()
  foreach(test_src ${BASALT_BASIC_TESTS})
    get_filename_component(test_name ${test_src} NAME_WE)
    set(expected ${CMAKE_CURRENT_SOURCE_DIR}/verify/expected/${test_name}.out)
    set(input_file ${CMAKE_CURRENT_SOURCE_DIR}/verify/expected/${test_name}.in)
    if(NOT EXISTS ${input_file})
      set(input_file "")
    endif()
    if(EXISTS ${expected})
      add_test(NAME z80_${test_name}
        COMMAND ${CMAKE_COMMAND}
          -DBASALT=$<TARGET_FILE:basalt>
          -DZ80ASM=${BASALT_Z80ASM}
          -DCPM=${BASALT_CPM}
          -DSOURCE=${test_src}
          -DEXPECTED=${expected}
          -DINPUT=${input_file}
          -DWORKDIR=${CMAKE_CURRENT_BINARY_DIR}/ctest/z80/${test_name}
          -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/run_z80_test.cmake)
    endif()
  endforeach()
endif()

if(BASALT_LLVM)
  foreach(test_src ${BASALT_BASIC_TESTS})
    get_filename_component(test_name ${test_src} NAME_WE)
    set(expected ${CMAKE_CURRENT_SOURCE_DIR}/verify/expected/${test_name}.out)
    set(input_file ${CMAKE_CURRENT_SOURCE_DIR}/verify/expected/${test_name}.in)
    if(NOT EXISTS ${input_file})
      set(input_file "")
    endif()
    if(EXISTS ${expected})
      add_test(NAME llvm_${test_name}
        COMMAND ${CMAKE_COMMAND}
          -DBASALT=$<TARGET_FILE:basalt>
          -DCC=${CMAKE_C_COMPILER}
          -DRUNTIME=${CMAKE_CURRENT_SOURCE_DIR}/basaltrt.c
          -DSOURCE=${test_src}
          -DEXPECTED=${expected}
          -DINPUT=${input_file}
          -DWORKDIR=${CMAKE_CURRENT_BINARY_DIR}/ctest/llvm/${test_name}
          -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/run_llvm_test.cmake)
    endif()
  endforeach()
  if(APPLE)
    foreach(test_src ${BASALT_BASIC_TESTS})
      get_filename_component(test_name ${test_src} NAME_WE)
      set(expected ${CMAKE_CURRENT_SOURCE_DIR}/verify/expected/${test_name}.out)
      set(input_file ${CMAKE_CURRENT_SOURCE_DIR}/verify/expected/${test_name}.in)
      if(NOT EXISTS ${input_file})
        set(input_file "")
      endif()
      if(EXISTS ${expected})
        add_test(NAME native_${test_name}
          COMMAND ${CMAKE_COMMAND}
            -DBASALT=$<TARGET_FILE:basalt>
            -DSOURCE=${test_src}
            -DEXPECTED=${expected}
            -DINPUT=${input_file}
            -DWORKDIR=${CMAKE_CURRENT_BINARY_DIR}/ctest/native/${test_name}
            -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/run_native_test.cmake)
      endif()
    endforeach()
    find_program(BASALT_DWARFDUMP dwarfdump)
    if(BASALT_DWARFDUMP)
      add_test(NAME native_debug
        COMMAND ${CMAKE_COMMAND}
          -DBASALT=$<TARGET_FILE:basalt>
          -DDWARFDUMP=${BASALT_DWARFDUMP}
          -DSOURCE=${CMAKE_CURRENT_SOURCE_DIR}/verify/test_1.bas
          -DWORKDIR=${CMAKE_CURRENT_BINARY_DIR}/ctest/native/debug
          -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/run_native_debug.cmake)
    endif()
  endif()
endif()
