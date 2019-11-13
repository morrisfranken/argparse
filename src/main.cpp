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
    std::string src_path        = arg("Source path");
    std::string dst_path        = arg("Destination path").set_default("world"s);
    const int k                 = kwarg("k", "A required parameter (short only)", "3");
    std::optional<float> alpha1 = kwarg("a,alpha1", "An optional float parameter").set_default(std::optional<float>(nullopt));
    float alpha2                = kwarg("2,alpha2", "An optional float parameter").set_default(0.6f);
    float alpha3                = kwarg("3,alpha3", "An optional float parameter");
    Custom c                    = kwarg("c,custom", "A custom class");
    std::vector<int> numbers    = kwarg("n,numbers", "An optional vector of integers (',' separated)").set_default(std::vector<int>{1,2});
    bool verbose                = flag("v,verbose", "A flag to toggle verbose");

    CONSTRUCTOR(MyArgs);
};

// ./argparse_test a b -k=5 --custom hello --verbose
int main(int argc, char* argv[]) {
    MyArgs args(argc, argv);
    args.print();


//    cout << args.alpha.has_value() << " : " << *args.alpha << endl;
//    cout << args.alpha << endl;

//    if (args.verbose)
//        std::cout << args.c.message << std::endl;

    return 0;
}