cmake_minimum_required(VERSION 3.15)
project(argparse)

set(CMAKE_CXX_STANDARD 17)

set(VERSION 1.1.0)
message(STATUS "argparse version ${VERSION}")

add_executable(argparse_test src/argparse_test.cpp include/argparse.h)
include_directories(argparse_test include)

# installing the header-only-library
add_library(${PROJECT_NAME} INTERFACE)

include(CMakePackageConfigHelpers)
set(ARGPARSE_CMAKE_DIR "lib/cmake/argparse" CACHE STRING "Installation directory for cmake files, relative to ${CMAKE_INSTALL_PREFIX}.")
set(version_config "${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake")
set(project_config "${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config.cmake")
set(targets_export_name ${PROJECT_NAME}-targets)

# Generate the version, config and target files into the build directory.
write_basic_package_version_file(
        ${version_config}
        VERSION ${VERSION}
        COMPATIBILITY AnyNewerVersion)
configure_package_config_file(
        ${PROJECT_SOURCE_DIR}/${PROJECT_NAME}-config.cmake.in
        ${project_config}
        INSTALL_DESTINATION ${ARGPARSE_CMAKE_DIR})
export(TARGETS ${PROJECT_NAME} NAMESPACE ${PROJECT_NAME}::
        FILE ${PROJECT_BINARY_DIR}/${targets_export_name}.cmake)

# Install version, config and target files.
install(FILES ${project_config} ${version_config}
        DESTINATION ${ARGPARSE_CMAKE_DIR})
install(EXPORT ${targets_export_name}
        DESTINATION ${ARGPARSE_CMAKE_DIR}
        NAMESPACE ${PROJECT_NAME}::)

# Install the header file and export the target
install(TARGETS ${PROJECT_NAME} EXPORT ${targets_export_name} DESTINATION lib)
install(FILES ${PROJECT_SOURCE_DIR}/include/argparse.h DESTINATION include)