/* @author: Morris Franken
 */
#include <iostream>
#include "argparse/argparse.hpp"

struct CommitArgs : public argparse::Args {
    bool &all                       = flag("a,all", "Tell the command to automatically stage files that have been modified and deleted, but new files you have not told git about are not affected.");
    std::string &message            = kwarg("m,message", "Use the given <msg> as the commit message.");

    int run() override {
        std::cout << "running commit with the with the following message: " << this->message << std::endl;
        return 0;
    }
};

struct PushArgs : public argparse::Args {
    std::string &source             = arg("Source repository").set_default("origin");
    std::string &destination        = arg("Destination repository").set_default("master");

    void welcome() override {
        std::cout << "Push code changes to remote" << std::endl;
    }

    int run() override {
        std::cout << "running push with the following parameters" << std::endl;
        print();
        return 0;
    }
};

struct MyArgs : public argparse::Args {
    bool &version                   = flag("v,version", "Print version");

    CommitArgs &commit              = subcommand("commit");
    PushArgs &push                  = subcommand("push");
};

int main(int argc, char* argv[]) {
    auto args = argparse::parse<MyArgs>(argc, argv);

    if (args.version) {
        std::cout << "argparse_subcomands version 1.0.0" << std::endl;
        return 0;
    }

    return args.run_subcommands();
}