/* @author: Morris Franken
 */
#include <iostream>

#include "argparse/argparse.hpp"

enum Color {
    RED,
    BLUE,
    GREEN,
};

struct MyArgs : public argparse::Args {
    Color &color                    = kwarg("c,color", "An Enum input");
    bool &verbose                   = flag("v,verbose", "A flag to toggle verbose");
};

int main(int argc, char* argv[]) {
    auto args = argparse::parse<MyArgs>(argc, argv);

    if (args.verbose)
        args.print();      // prints all variables

    return 0;
}