cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
project("Padding Oracle")

enable_testing()
SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(padding_oracle main.cc)
target_link_libraries(padding_oracle ${CONAN_LIBS})
