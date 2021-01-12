/* @author: Morris Franken
 * This argparse version relies on copy elision to make sure the same variable is used
 * STATUS: not working, copy elision not forced for trivial copyable types.
 * Awaiting http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2020/p2025r1.html
 */
#include <iostream>

#include "argparse4.h"

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

#if true
struct MyArgs : public argparse4::Args {
    std::string &src_path           = arg("Source path");
    std::string &dst_path           = arg("Destination path").set_default("default_destination");
    int &k                          = kwarg("k", "An implicit int parameter", /*implicit*/"3");
    float &alpha                    = kwarg("a,alpha", "An optional float parameter with default value").set_default(0.6f);
    std::optional<float> &beta      = kwarg("b,beta", "An optional float parameter with std::optional return");
    std::vector<int> &numbers       = kwarg("n,numbers", "An int vector, comma separated");
    std::vector<std::string> &files = kwarg("files", "mutliple arguments").multi_argument();
    Color &color                    = kwarg("c,color", "An Enum input");
    bool &verbose                   = flag("v,verbose", "A flag to toggle verbose");

    CONSTRUCTOR(MyArgs);
};
#else
struct MyArgs : public argparse4::Args {
    std::string& src_path           = arg("Source path");
    std::string& dst_path           = arg("Destination path").set_default("world");// default value set to "world"
    std::vector<std::string>& others= arg("Others").multi_argument().set_default<std::vector<std::string>>({});// default value set to empty vector
    int& k                          = kwarg("k", "A required parameter (short only)", "3");   // Implicit value set to 3
    std::shared_ptr<float>& alpha   = kwarg("a,alpha", "An optional float parameter");                    // pointers have a default value of nullptr
    float& beta                     = kwarg("b,beta", "An optional float parameter with default set as float").set_default(0.6f);
    float& beta2                    = kwarg("b,beta", "An optional float parameter with default set as string").set_default("0.6f");
    float*& gamma                   = kwarg("g,gamma", "An optional float parameter with implicit value", "0.5"); // pointers have a default value of nullptr
    Custom& custom                  = kwarg("c,custom", "A custom class");                                // Custom classes that support a std::string constructor
    std::vector<int>& numbers       = kwarg("n,numbers", "An optional vector of integers").set_default<std::vector<int>>({1,2});
    std::vector<int>& numbers2      = kwarg("numbers2", "An optional vector of integers").set_default("1,2,3");
    std::vector<int>& multiple      = kwarg("m,multiple", "An optional vector of integers taking multiple arguments").multi_argument().set_default(std::vector<int>{1,2});
    std::optional<float>& opt       = kwarg("o,optional", "An optional float parameter");
    Color &color                    = kwarg("c,color", "An Enum input");
    bool& flag1                     = flag("f,flag", "A test flag");
    bool& verbose                   = flag("v,verbose", "A flag to toggle verbose");

    void welcome() override {
        cout << "*************************" << endl
             << "** Welcome to Argparse **" << endl
             << "*************************" << endl;
    }

    CONSTRUCTOR(MyArgs);
};
#endif


int main(int argc, char* argv[]) {
    MyArgs args(argc, argv);

    cout << "beta is: " << (args.beta.has_value()? std::to_string(args.beta.value()) : "nullptr") << endl;

    if (args.verbose)
        args.print();

    return 0;
}