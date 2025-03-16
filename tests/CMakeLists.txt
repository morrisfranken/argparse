project(argparse_tests)

add_executable(${PROJECT_NAME} tests.cpp)
include_directories(${PROJECT_NAME} ../include)

# Always link against the argparse library
target_link_libraries(${PROJECT_NAME} PRIVATE morrisfranken::argparse)

if(UNIX AND NOT APPLE)
    # Check for filesystem library requirements (only on Linux)
    include(CheckCXXSourceCompiles)
    check_cxx_source_compiles("
        #include <filesystem>
        int main() {
            std::filesystem::path p;
            return 0;
        }
    " FILESYSTEM_WORKS_WITHOUT_STDFS)

    if(NOT FILESYSTEM_WORKS_WITHOUT_STDFS)
        target_link_libraries(${PROJECT_NAME} PRIVATE stdc++fs)
    endif()
endif()

ADD_TEST(argparse ${PROJECT_NAME})