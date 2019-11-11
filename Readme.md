# ArgParse; A Simple Argument Parser for C++
A lightweight header-only library for parsing command line arguments in an elegant manner. ArgParse allows you to define a variable and its key/help on a single line while representing all the program arguments in a struct. 

## Usage
```c++
#include "argparse.h"

struct MyArgs : public argparse::Args {
    std::string src_path     = arg("Source path");
    std::string dst_path     = arg("Destination path");
    int k                    = kwarg("k", "A required parameter (short only)");
    double alpha             = kwarg("a,alpha", "An optional float parameter", /*default=*/"0.5");
    std::vector<int> numbers = kwarg("n,numbers", "An optional int-vector (',' separated)", /*default=*/"");
    bool verbose             = flag("v,verbose", "A flag to toggle verbose");

    CONSTRUCTOR(MyArgs);
};

int main(int argc, char* argv[]) {
    MyArgs args(argc, argv);

    if (args.verbose)
        args.print();

    return 0;
}
```
Argparse distinguishes 3 different types of arguments:
  - `arg(...)` : positional arguments
  - `kwarg(...)` : arguments that require a key and a value, e.g. `--variable 0.5`. These may have a default value.
  - `flag(...)` : arguments that do not take any value, but are uses to set a certain flag to true (e.g. `--verbose`).

Argparse support the following syntax
```
--long
--long=argument
--long argument
-a
-ab
-abc=argument
-abc argument
```
Where on the last 2 lines, `c` takes an argument while `a` and `b` do not. In addition, an argument may be a comma-separated vector.

## Custom classes
ArgParse support custom classes, as long as they have a constructor that takes a `std::string` as input. For example:
```c++
#include "argparse.h"

struct Custom {
    std::string message;

    Custom() = default;
    Custom(const std::string &v) {
        message = "Creating custom class from " + v;
    }
};

struct MyArgs : public argparse::Args {
    Custom c = kwarg("c,custom", "A custom class");

    CONSTRUCTOR(MyArgs);
};

int main(int argc, char* argv[]) {
    MyArgs args(argc, argv);
    std::cout << args.c.message << std::endl;
    return 0;
}
```
```
./argparse_test -c hello_world
Creating custom class from hello_world
```
## Examples
The example in the first code block prints the variables when the `--verbose` flag is used

```
$ ./argparse_test src dst -k 4 --alpha=0.2 --numbers=4,5,6 --verbose
    arg_0(Source ...) : src
    arg_1(Destina...) : dst
               --help : 0
                   -k : 4
           -a,--alpha : 0.2
         -n,--numbers : 4,5,6 
         -v,--verbose : 1
```
The `--help` is automatically added in ArgParse.
```
$ ./argparse_test --help
Usage: ./argparse arg_0 arg_1  [options...]
            arg_0 : Source path
            arg_1 : Destination path

Options:
           --help : print help
               -k : A required parameter (short only) [*]
       -a,--alpha : An optional parameter [0.5]
     -n,--numbers : An optional vector (',' separated) []
     -v,--verbose : A flag to toggle verbose

```
When it fails to parse the input string, it will display an error and exit. E.g. here we'll set `k` to be `notanumber` 
```
$ ./argparse_test src dst -k notanumber --alpha=0.2 --verbose
Invalid argument, could not convert "notanumber" for -k (A required parameter (short only))
```

## Installing
Since it is an header-only library, you can simply copy the `include/argparse.h` file in your own project. 

Alternatively you can build&install it using the following commands
```
mkdir build && cd build
cmake ..
make
sudo make install
```

It can be included in your cmake project as follows:
```
find_package(argparse REQUIRED)
``` 