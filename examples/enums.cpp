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

    CONSTRUCTOR(MyArgs);
};

int main(int argc, char* argv[]) {
    MyArgs args(argc, argv);

    if (args.verbose)
        args.print();      // prints all variables

    return 0;
}