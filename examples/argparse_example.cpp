/* @author: Morris Franken
 * This argparse version relies on copy elision to make sure the same variable is used
 * STATUS: not working, copy elision not forced for trivial copyable types.
 * Awaiting http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2020/p2025r1.html
 */
#include <iostream>

#include "argparse/argparse.hpp"

using namespace std;

enum Color {
    RED,
    BLUE,
    GREEN,
};

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
    float &alpha                    = kwarg("a,alpha", "An optional float parameter with default value").set_default(0.6f);
    std::optional<float> &beta      = kwarg("b,beta", "An optional float parameter with std::optional return");
    std::vector<int> &numbers       = kwarg("n,numbers", "An int vector, comma separated");
    std::vector<std::string> &files = kwarg("files", "multiple arguments").multi_argument();
    Color &color                    = kwarg("c,color", "An Enum input");
    bool &verbose                   = flag("v,verbose", "A flag to toggle verbose");

    CONSTRUCTOR(MyArgs);
};

int main(int argc, char* argv[]) {
    MyArgs args(argc, argv);

    if (args.verbose)
        args.print();

    return 0;
}