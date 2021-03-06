#
# "Adapted" from Range-v3's top-level CMakeLists.txt
#

cmake_minimum_required(VERSION 3.5)
set(CMAKE_LEGACY_CYGWIN_WIN32 0)


project(Stream CXX)


find_package(Git)

include(CTest)
enable_testing()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_program(MEMORYCHECK_COMMAND valgrind)
if(MEMORYCHECK_COMMAND-NOTFOUND)
  message("[W] Valgrind not found")
else()
  message("Valgrind: ${MEMORYCHECK_COMMAND}")
  set(MEMORYCHECK_COMMAND_OPTIONS "--trace-children=yes --leak-check=full")
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -ftemplate-backtrace-limit=0")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -march=native")
  set(CMAKE_CXX_FLAGS_DEBUG "-O0 -fno-inline -g3 -fstack-protector-all")
  set(CMAKE_CXX_FLAGS_RELEASE "-Ofast -g0 -DNDEBUG")
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -ftemplate-backtrace-limit=0")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -march=native")
  set(LLVM_ROOT "$ENV{LLVM_ROOT}" CACHE PATH "Path to LLVM installation")
  if(LLVM_ROOT)
    message(STATUS "LLVM Root: ${LLVM_ROOT}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -nostdinc++ -isystem ${LLVM_ROOT}/include/c++/v1")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L ${LLVM_ROOT}/lib -l c++ -l c++abi")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath,${LLVM_ROOT}/lib")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
  endif()
  set(CMAKE_CXX_FLAGS_DEBUG "-O0 -fno-inline -g3 -fstack-protector-all")
  set(CMAKE_CXX_FLAGS_RELEASE "-Ofast -g0 -DNDEBUG")
endif()


# A directory to find Google Test sources.
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/googletest/CMakeLists.txt")
  add_subdirectory(googletest EXCLUDE_FROM_ALL)
else()
  message("GTEST Not Found at ${CMAKE_CURRENT_SOURCE_DIR}/googletest/CMakeLists.txt")
endif()

# A directory to find Google Benchmark sources.
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/benchmark/CMakeLists.txt")
  add_subdirectory(benchmark EXCLUDE_FROM_ALL)
else()
  message("Google Benchmark Not Found at ${CMAKE_CURRENT_SOURCE_DIR}/benchmark/CMakeLists.txt")
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(include)
include_directories(./)
include_directories(lib)

add_subdirectory(lib)
# A directory to find Google Test sources.
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/googletest/CMakeLists.txt")
  add_subdirectory(tests)
else()
  message("Tests not being built")
endif()

# A directory to find Google Benchmark sources.
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/benchmark/CMakeLists.txt")
  add_subdirectory(benchmarks)
else()
  message("Micro benchmarks not being built")
endif()



add_executable(delay delay.c++)
target_link_libraries(delay lazystream)

add_executable(bench1 bench1.c++)
target_link_libraries(bench1 lazystream)

add_executable(bench2 bench2.c++)
target_link_libraries(bench2 lazystream)
