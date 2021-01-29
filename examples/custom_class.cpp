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
    Custom &custom                 = kwarg("c,custom", "A custom class");

    CONSTRUCTOR(MyArgs);
};

int main(int argc, char* argv[]) {
    MyArgs args(argc, argv);
    args.print();

    return 0;
}