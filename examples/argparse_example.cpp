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
    std::string &anonymous          = arg("an anonymous positional string argument");
    std::string &src_path           = arg("src_path", "a positional string argument");
    std::string &dst_path           = arg("Destination path").set_default("default_destination");
    int &k                          = kwarg("k", "An implicit int parameter", /*implicit*/"3");
    float &alpha                    = kwarg("a,alpha", "An optional float parameter with default value").set_default(0.6);
    std::optional<float> &beta      = kwarg("b,beta", "An optional float parameter with std::optional return");
    int &val                        = kwarg("val", "An optional int value").set_default(20.0);
    short &small                    = kwarg("short", "An optional short value").set_default(16);
    size_t &std_sizet               = kwarg("size_t", "An optional size_t value").set_default(19);
    long long &seed                 = kwarg("seed", "An optional size_t value").set_default(time(NULL));
    std::vector<int> &numbers       = kwarg("n,numbers", "An int vector, comma separated");
    std::vector<std::string> &files = kwarg("files", "multiple arguments").multi_argument();
    int &height                     = kwarg("h,height", "The height of an input image").set_default(256);
    std::wstring &wstring           = kwarg("wstring", "An optional wstring value").set_default(L"hello");
    bool &verbose                   = flag("v,verbose", "A flag to toggle verbose");

    void welcome() override {
        std::cout << "Welcome to Argparse" << std::endl;
    }
};

int main(int argc, char* argv[]) {
    auto args = argparse::parse<MyArgs>(argc, argv);

    if (args.verbose)
        args.print();

    return 0;
}