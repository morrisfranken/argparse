//
// Created by morris on 12-08-20.
//
#include <iostream>

#include "argparse2.h"

using namespace std;

struct Custom {
    std::string message;

    Custom() = default;
    Custom(const std::string &v) {
        message = "Creating custom class from " + v;
    }
};

struct MyArgs : public argparse2::Args {
    std::string& src_path           = arg("Source path");
    std::string& dst_path           = arg("Destination path").set_default("world");// default value set to "world"
    std::vector<std::string>& others= arg("Others").multi_argument().set_default<std::vector<std::string>>({});// default value set to "world"
    int& k                          = kwarg("k", "A required parameter (short only)", "3");   // Implicit value set to 3
    std::shared_ptr<float>& alpha   = kwarg("a,alpha", "An optional float parameter");                    // pointers have a default value of nullptr
    float& beta                     = kwarg("b,beta", "An optional float parameter").set_default(0.6f);
    float*& gamma                   = kwarg("g,gamma", "An optional float parameter with implicit value", "0.5"); // pointers have a default value of nullptr
    Custom& c                       = kwarg("c,custom", "A custom class");                                // Custom classes that support a std::string constructor
    std::vector<int>& numbers       = kwarg("n,numbers", "An optional vector of integers").set_default<std::vector<int>>({1,2});
    std::vector<int>& numbers2      = kwarg("numbers2", "An optional vector of integers").set_default("1,2,3");
    std::vector<int>& multiple      = kwarg("m,multiple", "An optional vector of integers taking multiple arguments").multi_argument().set_default(std::vector<int>{1,2});
    std::optional<float>& opt       = kwarg("o,optional", "An optional float parameter");
    bool& flag1                     = flag("f,flag", "A test flag");
    bool& verbose                   = flag("v,verbose", "A flag to toggle verbose");

    CONSTRUCTOR(MyArgs);
};

struct MyArgs2 : public argparse2::Args {
    std::string& src_path              = arg("Source path");
    std::vector<std::string>& others   = arg("Others").multi_argument().set_default("1,2");// default value set to "world"
    std::string& dst_path              = arg("Destination path");
    std::vector<std::string>& multiple = kwarg("m,multiple", "An optional vector of integers taking multiple arguments").multi_argument().set_default("");

    CONSTRUCTOR(MyArgs2);
};

// ./argparse_test a b -k=5 --custom hello --verbose
int main(int argc, char* argv[]) {
    MyArgs args(argc, argv);      // a -k -fc custom -m 3 2 1 --verbose 5

    if (args.verbose)
        args.print();

    return 0;
}