/* @author: Morris Franken
 * This argparse version relies on copy elision to make sure the same variable is used
 * STATUS: not working, copy elision not forced for trivial copyable types.
 * Awaiting http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2020/p2025r1.html
 */
#include <iostream>

#include "argparse4.h"

using namespace std;

struct Entry {
    template <typename T> inline operator T();
    template <typename T> inline operator T&() {
        T* t = new T{};
        if constexpr (!std::is_reference<T>::value) {
            cout << " NOT A REF" << endl;
        }
        return *t;
    }
};

enum class Color { RED = 2, BLUE = 4, GREEN = 8 };

struct Custom {
    std::string message;

    Custom() = default;
    Custom(const std::string &v) {
        message = "Creating custom class from " + v;
    }
};

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


int main(int argc, char* argv[]) {
    MyArgs args(argc, argv);
    args.print();

    return 0;
}