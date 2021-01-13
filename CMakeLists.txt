# This cmake is hugely inspired by https://github.com/p-ranav/argparse/blob/master/CMakeLists.txt
cmake_minimum_required(VERSION 3.15)
project(argparse VERSION 1.4.0 LANGUAGES CXX)

include(GNUInstallDirs)

# installing the header-only-library
add_library(${PROJECT_NAME} INTERFACE)
add_library(morrisfranken::argparse ALIAS argparse)

target_compile_features(argparse INTERFACE cxx_std_17)
target_include_directories(argparse INTERFACE
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>)

install(TARGETS argparse EXPORT argparseConfig)
install(EXPORT argparseConfig
        NAMESPACE argparse::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/argparse)
install(FILES ${CMAKE_CURRENT_LIST_DIR}/include/argparse/argparse.hpp
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/argparse)

export(EXPORT argparseConfig
        NAMESPACE argparse::)

option(ARGPARSE_BUILD_TESTS OFF)
if(ARGPARSE_BUILD_TESTS)
    add_executable(argparse_test src/argparse4_test.cpp)
    include_directories(argparse_test include)

    #target_compile_options(argparse_test PUBLIC -ftime-report)
    #target_precompile_headers(argparse_test PUBLIC include/argparse4.h) --> should be enabled on release
endif()

