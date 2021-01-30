/* @author: Morris Franken
 */
#include <iostream>

#include "argparse/argparse.hpp"

struct MyArgs : public argparse::Args {
    std::string &A = arg("Source path");
    std::vector<std::string> &B = arg("Variable paths").multi_argument();
    std::string &C = arg("Last");
};

// Example run: ./multiple_arguments a b b b c
int main(int argc, char* argv[]) {
    auto args = argparse::parse<MyArgs>(argc, argv);
    args.print();      // prints all variables

    return 0;
}