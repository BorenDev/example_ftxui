cmake_minimum_required(VERSION 3.30)

project(
  example_ftxui
  VERSION 0.1.0
  LANGUAGES CXX)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  set(PROJECT_IS_TOP_LEVEL ON)
else()
  set(PROJECT_IS_TOP_LEVEL OFF)
endif()

if(NOT PROJECT_IS_TOP_LEVEL)
  set_directory_properties(PROPERTIES EXCLUDE_FROM_ALL ON)
endif()

include(deps/fetch_deps.cmake)

add_subdirectory(src)

include(CPack)
