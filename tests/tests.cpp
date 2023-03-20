/* @author: Morris Franken
 * This argparse version relies on copy elision to make sure the same variable is used
 * STATUS: not working, copy elision not forced for trivial copyable types.
 * Awaiting http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2020/p2025r1.html
 */
#undef NDEBUG
#include <iostream>
#include <cassert>

#include "argparse/argparse.hpp"

using namespace std;

struct Custom {
    std::string message;

    Custom() = default;
    Custom(const std::string &v) {
        message = v;
    }
};

std::pair<int, char**> get_argc_argv(std::string &str) {
    std::string key;
    std::vector<char*> splits = {(char *)str.c_str()};
    for (int i = 1; i < str.size(); i++) {
        if (str[i] == ' ') {
            str[i] = '\0';
            splits.emplace_back(&str[++i]);
        }
    }
    char** argv = new char*[splits.size()];
    for (int i = 0; i < splits.size(); i++) {
        argv[i] = splits[i];
    }

    return {(int)splits.size(), argv};
}

template <typename T> T test_args(std::string command) {
    cout << command << endl;
    const auto &[argc, argv] = get_argc_argv(command);
    auto args = argparse::parse<T>(argc, argv);
    args.print();
    cout << endl;
    return args;
}

void TEST_MULTI() {
    struct Args : public argparse::Args {
        std::string &A = arg("Source path");
        std::vector<std::string> &B = arg("Variable paths").multi_argument();
        std::string &C = arg("Last");
        std::vector<std::string> &D = kwarg("d", "comma-separated").set_default("");
    };

    {
        Args args = test_args<Args>("argparse_test a b b b c -d 1,2,3,");

        assert(args.A == "a");
        assert(args.B.size() == 3 && args.B[0] == "b" && args.B[2] == "b");
        assert(args.C == "c");
        assert(args.D.size() == 3);
    }
}

void TEST_MULTI2() {
    struct Args : public argparse::Args {
        std::string& A              = arg("Source path");
        std::vector<std::string>& B = arg("Variable paths").multi_argument();
        std::string& C              = arg("Last");
        std::vector<std::string>& v = kwarg("variable", "Variable paths").multi_argument();
        bool& verbose               = flag("verbose", "A flag to toggle verbose");
    };

    {
        Args args = test_args<Args>("argparse_test a b b b --variable v v v --verbose c");

        assert(args.A == "a");
        assert(args.B.size() == 3 && args.B[0] == "b" && args.B[2] == "b");
        assert(args.C == "c");
        assert(args.v.size() == 3 && args.v[0] == "v" && args.v[2] == "v");
    }
}

#ifdef HAS_MAGIC_ENUM
void TEST_ENUM() {
    enum Color {
        RED,
        BLUE,
        GREEN,
    };

    struct Args : public argparse::Args {
        Color& color  = kwarg("c,color", "An Enum input");
        Color& color2 = kwarg("color2", "An Enum input").set_default(RED);
        Color& color3 = kwarg("color3", "An Enum input", "green");
    };

    {
        Args args = test_args<Args>("argparse_test --color blue --color3");

        assert(args.color == BLUE);
        assert(args.color2 == RED);
        assert(args.color3 == GREEN);
    }
}
#endif

void TEST_ALL() {
    struct Args : public argparse::Args {
        std::string& src_path           = arg("Source path");
        std::string& dst_path           = arg("Destination path").set_default("world");// default value set to "world"
        std::vector<std::string>& others= arg("Others").multi_argument().set_default<std::vector<std::string>>({});// default value set to empty vector
        int& k                          = kwarg("k", "A required parameter (short only)", "3");   // Implicit value set to 3
        std::shared_ptr<float>& alpha   = kwarg("a,alpha", "An optional float parameter");                    // pointers have a default value of nullptr
        float& beta                     = kwarg("b,beta", "An optional float parameter with default set as float").set_default(0.6f);
        float& beta2                    = kwarg("beta2", "An optional float parameter with default set as string").set_default("0.6f");
        float*& gamma                   = kwarg("g,gamma", "An optional float parameter with implicit value", "0.5"); // pointers have a default value of nullptr
        Custom& custom                  = kwarg("c,custom", "A custom class");                                // Custom classes that support a std::string constructor
        std::vector<int>& numbers       = kwarg("n,numbers", "An optional vector of integers").set_default<std::vector<int>>({1,2});
        std::vector<int>& numbers2      = kwarg("numbers2", "An optional vector of integers").set_default("3,4,5");
        std::vector<std::string> &files = kwarg("files", "multiple arguments").multi_argument();
        std::optional<float>& opt       = kwarg("o,optional", "An optional float parameter");
        bool& flag1                     = flag("f,flag", "A test flag");
        bool& verbose                   = flag("v,verbose", "A flag to toggle verbose");
    };

    {
        Args args = test_args<Args>("argparse_test source_path destination -k=5 --alpha=1 --beta 3.3 --gamma --numbers=1,2,3,4,5 --numbers2 6,7,8 --files f1 f2 f3 --custom hello_custom --optional 1 --verbose");

        assert(args.src_path == "source_path");
        assert(args.dst_path == "destination");
        assert(args.k == 5);
        assert(args.alpha != nullptr && std::abs(*args.alpha - 1) < 0.0001);
        assert(std::abs(args.beta - 3.3) < 0.0001);
        assert(std::abs(args.beta2 - 0.6) < 0.0001);
        assert(args.gamma != nullptr && std::abs(*args.gamma - 0.5) < 0.0001);
        assert(args.numbers.size() == 5 && args.numbers[2] == 3);
        assert(args.numbers2.size() == 3 && args.numbers2[2] == 8);
        assert(args.files.size() == 3 && args.files[2] == "f3");
        assert(std::abs(args.opt.value() - 1.0f) < 0.0001);
        assert(args.custom.message == "hello_custom");
        assert(args.flag1 == false);
        assert(args.verbose);
    }

    {
        Args args = test_args<Args>("argparse_test source_path -k --files --custom hello_custom --optional 1 --verbose");

        assert(args.src_path == "source_path");
        assert(args.dst_path == "world");
        assert(args.k == 3);
        assert(args.alpha == nullptr);
        assert(std::abs(args.beta - 0.6) < 0.0001);
        assert(std::abs(args.beta2 - 0.6) < 0.0001);
        assert(args.gamma == nullptr);
        assert(args.numbers.size() == 2 && args.numbers[1] == 2);
        assert(args.numbers2.size() == 3 && args.numbers2[2] == 5);
        assert(args.files.empty());
        assert(std::abs(args.opt.value() - 1.0f) < 0.0001);
        assert(args.custom.message == "hello_custom");
        assert(args.flag1 == false);
        assert(args.verbose);
    }
}

void TEST_THROW() {
    struct Args : public argparse::Args {
        std::string &A = arg("Source path");
        int& k         = kwarg("k", "").set_default(0);   // Implicit value set to 3
    };

    {
        std::string command = "argparse_test";
        const auto &[argc, argv] = get_argc_argv(command);
        try {
            auto args = argparse::parse<Args>(argc, argv, true);
        } catch (const std::runtime_error &e) {
            assert(std::string(e.what()) ==  "Argument missing: arg_0 (Source path)");
        }
    }

    {
        std::string command = "argparse_test source_path -k=notanobumber";
        const auto &[argc, argv] = get_argc_argv(command);
        try {
            auto args = argparse::parse<Args>(argc, argv, true);
        } catch (const std::runtime_error &e) {
            assert(std::string(e.what()) ==  "Invalid argument, could not convert \"notanobumber\" for -k ()");
        }
    }
}

void TEST_SUBCOMMANDS() {
    struct CommitArgs : public argparse::Args {
        bool &all                       = flag("a,all", "Tell the command to automatically stage files that have been modified and deleted, but new files you have not told git about are not affected.");
        std::string &message            = kwarg("m,message", "Use the given <msg> as the commit message.");
    };

    struct PushArgs : public argparse::Args {
        std::string &source             = arg("Source repository").set_default("origin");
        std::string &destination        = arg("Destination repository").set_default("master");
    };

    struct MainArgs : public argparse::Args {
        bool &verbose                   = flag("v,verbose", "A flag to toggle verbose");

        CommitArgs &commit              = subcommand("commit");
        PushArgs &push                  = subcommand("push");

        void welcome() {
            std::cout << "Welcome to Argparse" << std::endl;
        }
    };

    {
        MainArgs args = test_args<MainArgs>("argparse_test --verbose");
        assert(args.commit.is_valid == false);
        assert(args.push.is_valid == false);
        assert(args.verbose == true);
    }

    {
        MainArgs args = test_args<MainArgs>("argparse_test commit -am \"testing-argparse\"");
        assert(args.push.is_valid == false);
        assert(args.commit.is_valid == true);
        assert(args.commit.all == true);
        assert(args.commit.message == "\"testing-argparse\"");
    }

    {
        MainArgs args = test_args<MainArgs>("argparse_test push origin dev");
        assert(args.commit.is_valid == false);
        assert(args.push.is_valid == true);
        assert(args.push.source == "origin");
        assert(args.push.destination == "dev");
    }
}

int main(int argc, char* argv[]) {
    TEST_ALL();
    TEST_MULTI();
    TEST_MULTI2();
    TEST_THROW();
#ifdef HAS_MAGIC_ENUM
    TEST_ENUM();
#else
    std::cout << "Magic Enum not installed in this system, therefore native enum support disabled" << std::endl;
#endif

    TEST_SUBCOMMANDS();

    std::cout << "finished all tests" << std::endl;
    return 0;
}