cmake_minimum_required(VERSION 2.8)
project(CXX11-STL-Tests)

include(CheckCXXCompilerFlag)

if(CMAKE_COMPILER_IS_GNUCXX)
  execute_process(
      COMMAND g++ -dumpversion
      OUTPUT_VARIABLE CMAKE_CXX_COMPILER_VERSION
      )
  if(CMAKE_CXX_COMPILER_VERSION GREATER 4.7 OR
     CMAKE_CXX_COMPILER_VERSION EQUAL 4.7)
  add_definitions("-std=c++11")
  else()
	  message(FATAL_ERROR 
	  "This project requires g++ 4.7 or higher")
  endif()
  set(CMAKE_CXX_FLAGS
      "${CMAKE_CXX_FLAGS} -Wall")
  set(CMAKE_CXX_FLAGS_DEBUG
      "${CMAKE_CXX_FLAGS} -O0 -g -pg")
  set(CMAKE_CXX_FLAGS_RELEASE
      "${CMAKE_CXX_FLAGS} -O4 -DNDEBUG")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO
      "${CMAKE_CXX_FLAGS} -O2 -g -pg -DNDEBUG")
elseif(${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
  check_cxx_compiler_flag("-std=c++11" CXX11_SUPPORTED)
  check_cxx_compiler_flag("-stdlib=libc++" CXX11_LIBS_SUPPORTED)
  if(CXX11_SUPPORTED AND CXX11_LIBS_SUPPORTED)
    add_definitions("-std=c++11")
    add_definitions("-stdlib=libc++")
  else()
    message(FATAL_ERROR 
	  "Compiler does not appear to support C++11. This project requires C++11")
  endif()
elseif(MSVC)
# Nothing special needed for MSVC
# Just enable all warnings for debug and set NDEBUG for release
  set(CMAKE_CXX_FLAGS_DEBUG "/W4")
  set(CMAKE_CXX_FLAGS_RELEASE "/D NDEBUG")
else()
  message(FATAL_ERROR
  "Your compiler probably does not support C++11. This project requires C++11")
endif()

add_executable(vector-vs-array vector-vs-array.cpp)
