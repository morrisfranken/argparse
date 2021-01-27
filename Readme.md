# Argparse; Modern Argument Parser for C++
A lightweight header-only library for parsing command line arguments in an elegant manner. Argparse allows you to define variables as a one-liner without redefining their type or names while representing all the program arguments in a struct that can be easily passed to functions. 

## Usage
```c++
#include "argparse/argparse.hpp"

struct MyArgs : public argparse::Args {
    std::string &src_path           = arg("Source path");
    std::string &dst_path           = arg("Destination path").set_default("default_destination");
    int &k                          = kwarg("k", "An implicit int parameter", /*implicit*/"3");
    float &alpha                    = kwarg("a,alpha", "An optional float parameter with default value").set_default(0.6f);
    std::optional<float> &beta      = kwarg("b,beta", "An optional float parameter with std::optional return");
    std::vector<int> &numbers       = kwarg("n,numbers", "An int vector, comma separated");
    std::vector<std::string> &files = kwarg("files", "mutliple arguments").multi_argument();
    bool &verbose                   = flag("v,verbose", "A flag to toggle verbose");

    CONSTRUCTOR(MyArgs);
};


int main(int argc, char* argv[]) {
    MyArgs args(argc, argv);

    if (args.verbose)
        args.print();

    return 0;
}
```
Example output when setting the `verbose` flag in the example above, it will print the capturing arguments with the `args.print()` function: 
```
$ ./argparse_test source -k --alpha 0.3 --numbers 1,2,3 --files ../src/*.cpp --verbose
    arg_0(Source ...) : source
    arg_1(Destina...) : default_destination
                   -k : 3
           -a,--alpha : 0.3
            -b,--beta : none
         -n,--numbers : 1,2,3
              --files : ../src/argparse2_test.cpp,../src/argparse3_test.cpp,../src/argparse4_test.cpp,../src/argparse_test.cpp
         -v,--verbose : true
               --help : false
```
Argparse also automatically enables `--help`:
```
$ ./argparse_test --help
Usage: ./argparse_test arg_0 arg_1  [options...]
            arg_0 : Source path [required]
            arg_1 : Destination path [default: default_destination]

Options:
               -k : An implicit int parameter [implicit: "3", required]
       -a,--alpha : An optional float parameter with default value [default: 0.6]
        -b,--beta : An optional float parameter with std::optional return [default: none]
     -n,--numbers : An int vector, comma separated [required]
          --files : mutliple arguments [required]
     -v,--verbose : A flag to toggle verbose [implicit: "true", default: false]
           --help : print help [implicit: "true", default: false]
```

# Input
Argparse distinguishes 3 different types of arguments:

| Type | Function  |
| --- |:-------------|
| `arg(help)`    | positional arguments |
| `kwarg(key,help,implicit)`  | keyworded-arguments that require a key and a value, e.g. `--variable 0.5`.  |
| `flag(key,help)`   | arguments that do not take any value, but are used to set a certain flag to true (e.g. `--verbose`) |

Argparse supports the following syntax
```
--long
--long=value
--long value
-a
-ab
-abc=value
-abc value
```
Where on the last 2 lines, `a` and `b` are considered `flags`, while `c` is considered a `kwarg` and is set to `value`. In addition, an argument may be a comma-separated vector.

# Vectors and multiple arguments
Argparse supports `std::vector`. There are 2 ways in which the vector can be read from the commandline, either a vector can be parsed from a comma-separated string, or by setting using the `multi_argument()` flag to aggregate multiple program argument into the vector, e.g. when using the `./*` in bash to list all the files in a directory.  
```c++
std::vector<int> &numbers           = kwarg("n,numbers", "An int vector");
std::vector<std::string> &tags      = kwarg("t,tags", "A word vector");
std::vector<std::string> &files     = kwarg("files", "mutliple arguments").multi_argument();
```
Example usage:
```
$ argparse_test --numbers 3,4,5,6
$ argparse_test --numbers=3,4,5,6
$ argparse_test --tags="hello"
$ argparse_test --tags="hello, world"
$ argparse_test --files a b c
$ argparse_test --files ./*
```
In case there ar other positional arguments, Argparse will make sure that they are correctly assigned 
####TODO

# Implicit values
`Kwargs` may have an implicit value, meaning that when the argument is present on the commandline, but no value is set (e.g. when it's passed as a flag), it will use the implicit value if set. Implicit values passed as string.
```c++
int &k                              = kwarg("k", "An implicit int parameter", /*implicit*/"3");
float &alpha                        = kwarg("a,alpha", "A implicit float parameter", /*implicit*/"0.5");
std::vector<int> &numbers           = kwarg("n,numbers", "A implicit int vector", /*implicit*/"1,2,3");
```
Examples
```
$ argparse_test -k
k = 3
$ argparse_test -k 9
k = 9
```

# Default values
`Args` and `Kwargs` may have a default value, which will be used when the argument is not present on the commandline. These can be passed through the `set_default` function, it accepts either a string or the type of the parameter itself
```c++
std::string &dst_path           = arg("An optional positional argument").set_default("output.png");
float &alpha                    = kwarg("a,alpha", "An optional float parameter with float as default").set_default(0.5f);
float &beta                     = kwarg("b,beta", "An optional float parameter with string as default").set_default("0.5");
std::vector<int> &numbers       = kwarg("n,numbers", "An optional vector of integers").set_default(std::vector<int>{1,2});
```

# Pointers and Optionals
In situations where setting a default value is not sufficient, Argparse supports `std::optional`, and (smart)pointers, these can be used in situations where you'd like to distinguish whether an argument was set by the user, or when a default value was used. When declaring a raw pointer or a `std::shared_ptr`, the default value for these is automatically set to `nullptr` (or `std::nullopt` for `std::optional`). 
```c++
std::shared_ptr<float> &alpha   = kwarg("a,alpha", "An optional smart-pointer float parameter");
std::optional<float> &beta      = kwarg("b,beta", "An optional float parameter with std::optional return");
float& *gamma                   = kwarg("g,gamma", "An optional raw pointer float parameter");
```
To check if `alpha` or `beta` has been set, and to fetch the value, consider the following example which prints the value if the argument has been set, or "nullptr" if it has not been set.
```c++
cout << "alpha is: " << (args.alpha != nullptr? std::to_string(*args.alpha) : "nullptr") << endl;
cout << "beta is: " << (args.beta.has_value()? std::to_string(args.beta.value()) : "nullptr") << endl;
```

# Enums
If [magic_enum](https://github.com/Neargye/magic_enum) is found on the system, Argparse supports automatic conversion from commandline to `enum`. E.g. consider th following example

```
enum Color {
    RED,
    BLUE,
    GREEN,
};
...
    Color &color                    = kwarg("c,color", "An Enum input");
...
```
Running it, and it will automatically convert the input to the `Color` enum (case-insensitive) 
```
$ ./argparse_test --color blue
```
It will only accept a valid input for enums, and the help-tip will display the available options for this enum:
```
$ ./argparse_test --help
...
        -c,--color : An Enum input [allowed: <red, blue, green>, required]
...
```

# Custom classes


# Examples
The `--help` is automatically added in ArgParse. Consider the following example usage when executing `argparse_test` (int `src/argparse_test.cpp`) 
```
$ ./argparse_test --help
Usage: ./argparse_test arg_0 arg_1  [options...]
            arg_0 : Source path
            arg_1 : Destination path [default: world]

Options:
           --help : print help
               -k : A required parameter (short only) [implicit: 3, required]
       -a,--alpha : An optional float parameter [default: null]
        -b,--beta : An optional float parameter [default: 0.6]
       -g,--gamma : An optional float parameter with implicit value [implicit: 0.5, default: null]
      -c,--custom : A custom class [required]
     -n,--numbers : An optional vector of integers (',' separated) [default: unknown]
     -v,--verbose : A flag to toggle verbose

```
In the example, the `args.print()` function is executed when the `--verbose` flag is present. This will list all the captured input arguments.

```
$ ./argparse_test src dst -k 4 --alpha=0.2 -c hello --numbers=4,5,6 --verbose
    arg_0(Source ...) : src
    arg_1(Destina...) : dst
               --help : 0
                   -k : 4
           -a,--alpha : 0.2
            -b,--beta : 0.6
           -g,--gamma : null
          -c,--custom : hello
         -n,--numbers : 4,5,6
         -v,--verbose : 1
```
In case it fails to parse the input string, it will display an error and exit. E.g. here we'll set `k` to be `notanumber` 
```
$ ./argparse_test src dst -k notanumber
Invalid argument, could not convert "notanumber" for -k (A required parameter (short only))
```

# Installing
Since it is an header-only library, you can simply copy the `include/argparse.h` file in to your own project. 

Alternatively, you can build&install it using the following commands
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

# FAQ
 - Why references