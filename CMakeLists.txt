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

option(ARGPARSE_BUILD_EXAMPLES OFF)
option(ARGPARSE_BUILD_TESTS OFF)
if(ARGPARSE_BUILD_EXAMPLES)
    # we want to add these compile options to the main library, so that we can let the compiler warn us about issues in code the tests may not cover
    set(COMPILE_OPTIONS -Wall -Werror -Wpedantic -Wshadow -Wdouble-promotion -Wextra)
    target_compile_options(argparse INTERFACE ${COMPILE_OPTIONS})

    add_executable(argparse_example examples/argparse_example.cpp)
    target_include_directories(argparse_example PRIVATE include)
    target_link_libraries(argparse_example PRIVATE morrisfranken::argparse)

    add_executable(hello_world examples/hello_world.cpp)
    target_include_directories(hello_world PRIVATE include)
    target_link_libraries(hello_world PRIVATE morrisfranken::argparse)

    add_executable(multiple_arguments examples/multiple_arguments.cpp)
    target_include_directories(multiple_arguments PRIVATE include)
    target_link_libraries(multiple_arguments PRIVATE morrisfranken::argparse)

    set(EXE_NAME argparse_subcommands)
    add_executable(${EXE_NAME} examples/${EXE_NAME}.cpp)
    target_include_directories(${EXE_NAME} PRIVATE include)
    target_link_libraries(${EXE_NAME} PRIVATE morrisfranken::argparse)

    add_executable(enums examples/enums.cpp)
    target_include_directories(enums PRIVATE include)
    target_link_libraries(enums PRIVATE morrisfranken::argparse)
    #target_compile_options(argparse_test PUBLIC -ftime-report)
    #target_precompile_headers(argparse_test PUBLIC include/argparse.h) --> should be enabled on release
endif()

if(ARGPARSE_BUILD_TESTS)
    ENABLE_TESTING()
    add_subdirectory(tests)
endif()
