cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
project("DLOG")

enable_testing()
SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_library(dlog_lib dlog.cc)
add_executable(DlogTest dlog_test.cc)
add_test(DlogTest bin/DlogTest)
target_link_libraries(DlogTest dlog_lib ${CONAN_LIBS})

add_executable(dlog main.cc)
target_link_libraries(dlog dlog_lib ${CONAN_LIBS})
