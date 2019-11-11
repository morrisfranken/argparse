#include <iostream>

#include "argparse.h"

using namespace std;

struct Custom {
    std::string message;

    Custom() = default;
    Custom(const std::string &v) {
        message = "Creating custom class from " + v;
    }
};

struct MyArgs : public argparse::Args {
    std::string src_path     = arg("Source path");
    std::string dst_path     = arg("Destination path");
    const int k              = kwarg("k", "A required parameter (short only)");
    double alpha             = kwarg("a,alpha", "An optional float parameter", "0.5");
    Custom c                 = kwarg("c,custom", "A custom class");
    std::vector<int> numbers = kwarg("n,numbers", "An optional vector of integers (',' separated)", "");
    bool verbose             = flag("v,verbose", "A flag to toggle verbose");

    CONSTRUCTOR(MyArgs);
};

// ./argparse_test a b -k=5 --custom hello --verbose
int main(int argc, char* argv[]) {
    MyArgs args(argc, argv);
    args.print();

    if (args.verbose)
        std::cout << args.c.message << std::endl;

    return 0;
}