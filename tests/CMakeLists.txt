project(argparse_tests)

add_executable(${PROJECT_NAME} tests.cpp)
include_directories(${PROJECT_NAME} ../include)

target_link_libraries(${PROJECT_NAME} PRIVATE morrisfranken::argparse)
ADD_TEST(argparse ${PROJECT_NAME})