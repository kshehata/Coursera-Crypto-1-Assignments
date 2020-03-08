cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
project("Video Hasher")

enable_testing()
SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_library(video_hasher_lib video_hasher.cc)
add_executable(VideoHasherTest video_hasher_test.cc)
add_test(VideoHasherTest bin/VideoHasherTest)
target_link_libraries(VideoHasherTest video_hasher_lib ${CONAN_LIBS})

add_executable(video_hasher main.cc)
target_link_libraries(video_hasher video_hasher_lib ${CONAN_LIBS})
