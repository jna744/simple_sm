cmake_minimum_required(VERSION 3.13)

project(simple_state_machine VERSION 0.1)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  set(SSM_PRIMARY_PROJECT true)
  set(CMAKE_EXPORT_COMPILE_COMMANDS true)
endif()

find_package(smp REQUIRED)

add_library(ssm INTERFACE)

target_include_directories(
  ssm INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

target_link_libraries(ssm INTERFACE smp::smp)

if(BUILD_TESTING AND SSM_PRIMARY_PROJECT)
  add_subdirectory(test)
endif()

install(
  TARGETS ssm
  EXPORT ssmTargets
  PUBLIC_HEADER DESTINATION include
)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
  ssmConfigVersion.cmake
  VERSION ${PACKAGE_VERSION}
  COMPATIBILITY AnyNewerVersion
)

install(
  EXPORT ssmTargets
  FILE ssmConfig.cmake
  NAMESPACE ssm::
  DESTINATION lib/cmake/ssm
)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION include)
