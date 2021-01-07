/* @author: Morris Franken
 * This argparse version relies on packed struct, and reflection to set the variables.
 * STATUS: working, but MUST use the #pragma pack(1), and #pragma pack()  (https://stackoverflow.com/questions/24887459/c-c-struct-packing-not-working)
 *  - std::optional ont working
 */
#include <iostream>

#include "argparse3.h"

using namespace std;

struct Custom {
    std::string message;

    Custom() = default;
    Custom(const std::string &v) {
        message = "Creating custom class from " + v;
    }
};

#pragma pack(1)
struct MyArgs3 : public argparse3::Args {
    std::string src_path           = arg("Source path");
    std::string dst_path           = arg("Destination path").set_default("world"s);// default value set to "world"
    std::vector<std::string> others= arg("Others").multi_argument().set_default<std::vector<std::string>>({});// default value set to "world"
    int k                          = kwarg("k", "A required parameter (short only)", "3");   // Implicit value set to 3
    std::shared_ptr<float> alpha   = kwarg("a,alpha", "An optional float parameter");                    // pointers have a default value of nullptr
    float beta                     = kwarg("b,beta", "An optional float parameter").set_default(0.6f);
    float* gamma                   = kwarg("g,gamma", "An optional float parameter with implicit value", "0.5"); // pointers have a default value of nullptr
    Custom c                       = kwarg("c,custom", "A custom class");                                // Custom classes that support a std::string constructor
    std::vector<int> numbers       = kwarg("n,numbers", "An optional vector of integers").set_default<std::vector<int>>({1,2});
    std::vector<int> numbers2      = kwarg("numbers2", "An optional vector of integers").set_default("1,2,3");
    std::vector<int> multiple      = kwarg("m,multiple", "An optional vector of integers taking multiple arguments").multi_argument().set_default(std::vector<int>{1,2});
//    std::optional<float> opt       = kwarg("o,optional", "An optional float parameter");
    bool verbose                   = flag("v,verbose", "A flag to toggle verbose");
    bool flag1                     = flag("f,flag", "A test flag");

//    MyArgs3(int argc, char *argv[]) : argparse3::Args(argc, argv) {
//        cout << "validating..." << endl;
//        cout << "offset verbose: " << offsetof(MyArgs3, verbose) << endl;
//        cout << "offset k: " << offsetof(MyArgs3, k) << endl;
//        cout << "offset alpha: " << offsetof(MyArgs3, alpha) << endl;
//        cout << "offset help: " << offsetof(MyArgs3, _help) << endl;
//        cout << "sizeof shared ptr: " << sizeof(std::shared_ptr<float>) << endl;
//        cout << "sizeof int: " << sizeof(int) << endl;
//        validate(_help);
//    }
//    bool _help = flag("help", "print help");

    CONSTRUCTOR(MyArgs3);
};
#pragma pack()

//struct MyArgs2 : public argparse3::Args {
//    std::string src_path              = arg("Source path");
//    std::vector<std::string> others   = arg("Others").multi_argument().set_default("1,2");// default value set to "world"
//    std::string dst_path              = arg("Destination path");
//    std::vector<std::string> multiple = kwarg("m,multiple", "An optional vector of integers taking multiple arguments").multi_argument().set_default("");
//
//    CONSTRUCTOR(MyArgs2);
//};

// Options:
// 1. use the reference for all input parameters                                                --> ugly, will be difficult for new users
// 2. create a macro to force reference for paramteter                                          --> perhaps even more ugly, but more easy to remember
// 3. create different api, with templated classes for kwarg, arg, flag which collapse          --> cannot assign without redefining
// 4. use packed struct, and estimate location of variable in struct based on order of execution--> many potential problems, in terms of compilers re-organising variables, or order of execution of function, and not to mention if other varialbes are being added which are not args/kwargs/flags
// 5. force copy elision?                                                                       --> not guaranteed
// 6. create param<> class which can collapse to it's original type, but holds the reference    --> additional keywords, feels less native

// TODO(morris):
// -- https://github.com/Neargye/magic_enum

struct A {
    template <typename T> inline operator T() {

    }
};

// ./argparse_test a b -k=5 --custom hello --verbose
int main(int argc, char* argv[])  {
//    A a;
//    std::optional<float> opt = a;

//    cout << "s = " << sizeof(std::shared_ptr<float>) << endl;
//    cout << "s = " << sizeof(std::shared_ptr<char[50]>) << endl;

    MyArgs3 args(argc, argv);      // a -k -fc custom -m 3 2 1 --verbose 5

    if (args.verbose)
        args.print();

    return 0;
}