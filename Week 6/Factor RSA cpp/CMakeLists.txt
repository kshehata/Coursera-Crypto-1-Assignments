cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
project("Factor RSA")

enable_testing()
SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_library(factor_rsa_lib factor_rsa.cc)
add_executable(FactorRSATest factor_rsa_test.cc)
add_test(FactorRSATest bin/FactorRSATest)
target_link_libraries(FactorRSATest factor_rsa_lib ${CONAN_LIBS})
