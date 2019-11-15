#include <iostream>
#include <memory>

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
    std::string src_path          = arg("Source path");
    std::string dst_path          = arg("Destination path").set_default(std::string("world"));// default value set to "world"
    const int k                   = kwarg("k", "A required parameter (short only)", "3");   // Implicit value set to 3
    std::shared_ptr<float> alpha  = kwarg("a,alpha", "An optional float parameter");                    // pointers have a default value of nullptr
    float beta                    = kwarg("b,beta", "An optional float parameter").set_default(0.6f);
    float *gamma                  = kwarg("g,gamma", "An optional float parameter with implicit value", "0.5"); // pointers have a default value of nullptr
    Custom c                      = kwarg("c,custom", "A custom class");                                // Custom classes that support a std::string constructor
    std::vector<int> numbers      = kwarg("n,numbers", "An optional vector of integers (',' separated)").set_default(std::vector<int>{1,2});
    std::optional<float> opt      = kwarg("o,optional", "An optional float parameter").set_default<std::optional<float>>(std::nullopt); // due to an issue with the implementation of c++17, we have to specify the default to be nullopt
    bool verbose                  = flag("v,verbose", "A flag to toggle verbose");

    CONSTRUCTOR(MyArgs);
};

// ./argparse_test a b -k=5 --custom hello --verbose
int main(int argc, char* argv[]) {
    MyArgs args(argc, argv);

    if (args.verbose)
        args.print();

//    argparse::Args args(argc, argv);
//    auto src_path = args.arg("Source path");
//    args.validate();

    return 0;
}