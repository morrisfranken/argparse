/* @author: Morris Franken
 */
#include <iostream>
#include "argparse/argparse.hpp"

struct Custom {
    std::string message;

    Custom() = default;
    Custom(const std::string &v) {
        message = "Creating custom class from " + v;
    }
};

struct MyArgs : public argparse::Args {
    std::string &src_path           = arg("Source path");
    std::string &dst_path           = arg("Destination path").set_default("default_destination");
    int &k                          = kwarg("k", "An implicit int parameter", /*implicit*/"3");
    float &alpha                    = kwarg("a,alpha", "An optional float parameter with default value").set_default(0.6);
    std::optional<float> &beta      = kwarg("b,beta", "An optional float parameter with std::optional return");
    int &val                        = kwarg("val", "An optional int value").set_default(20.0);
    short &small                    = kwarg("short", "An optional short value").set_default(16);
    std::vector<int> &numbers       = kwarg("n,numbers", "An int vector, comma separated");
    std::vector<std::string> &files = kwarg("files", "multiple arguments").multi_argument();
    bool &verbose                   = flag("v,verbose", "A flag to toggle verbose");

    void welcome() {
        std::cout << "Welcome to Argparse" << std::endl;
    }
};

int main(int argc, char* argv[]) {
    auto args = argparse::parse<MyArgs>(argc, argv);

    if (args.verbose)
        args.print();

    return 0;
}