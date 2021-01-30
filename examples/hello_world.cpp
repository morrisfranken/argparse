/* @author: Morris Franken
 */
#include <iostream>

#include "argparse/argparse.hpp"

struct MyArgs : public argparse::Args {
    std::string &src_path           = arg("a positional string argument");
    int &k                          = kwarg("k", "A keyworded integer value");
    float &alpha                    = kwarg("a,alpha", "An optional float value").set_default(0.5f);
    bool &verbose                   = flag("v,verbose", "A flag to toggle verbose");
};

int main(int argc, char* argv[]) {
    auto args = argparse::parse<MyArgs>(argc, argv);

    if (args.verbose)
        args.print();      // prints all variables

    return 0;
}