project(scorep_plugin_ucx)

cmake_minimum_required(VERSION 3.8)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(UCX_INCLUDE_DIRS "$ENV{UCX_PATH}/src") # "/mnt/central/testing/src/openucx/ucx/src")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/common")

include(scorep_plugin_common/DefaultBuildType.cmake)

add_subdirectory(scorep_plugin_cxx_wrapper)

add_library(scorep_plugin_ucx
            SHARED
            src/scorep_plugin_ucx.cpp
            src/utils.cpp
            src/ucx_sampling.cpp)

set_target_properties(scorep_plugin_ucx PROPERTIES CXX_STANDARD 17)

target_include_directories(scorep_plugin_ucx PRIVATE
  src 
  include
  ${UCX_INCLUDE_DIRS})
  
target_compile_options(scorep_plugin_ucx INTERFACE -Wall -pedantic -Wextra -fPIC)

target_link_libraries(scorep_plugin_ucx PRIVATE 
  Scorep::scorep-plugin-cxx
  libucs.so)

install(TARGETS scorep_plugin_ucx DESTINATION lib)

