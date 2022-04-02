# Argparse; Modern Argument Parser for C++17
A lightweight header-only library for parsing command line arguments in an elegant manner. Argparse allows you to define variables as a one-liner without redefining their type or names while representing all the program arguments in a struct that can be easily passed to functions. 

## Usage
```c++
#include "argparse/argparse.hpp"

struct MyArgs : public argparse::Args {
    std::string &src_path = arg("a positional string argument");
    int &k                = kwarg("k", "A keyworded integer value");
    float &alpha          = kwarg("a,alpha", "An optional float value").set_default(0.5f);
    bool &verbose         = flag("v,verbose", "A flag to toggle verbose");
};


int main(int argc, char* argv[]) {
    MyArgs args = argparse::parse<MyArgs>(argc, argv);

    if (args.verbose)
        args.print();       // prints all variables 

    return 0;
}
```
Example output when setting the `verbose` flag in the example above, it will print the capturing arguments with the `args.print()` function: 
```
$ ./argparse_test source -k 4 --verbose
    arg_0(a posit...) : source
                   -k : 4
           -a,--alpha : 0.5
         -v,--verbose : true
               --help : false
```

# Input
Argparse distinguishes 3 different types of arguments:

| Type | Function                                                                                                                 |
| --- |:-------------------------------------------------------------------------------------------------------------------------|
| `arg(help)`    | positional arguments                                                                                                     |
| `kwarg(key,help,implicit)`  | keyworded-arguments that require a key and a value, e.g. `--variable 0.5`.                                               |
| `flag(key,help)`   | a boolean argument that is by default `false`, but can be set to `true` by defining it on the commandline (e.g. `--verbose`) |

Argparse supports the following syntax:
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

# Default values
`Args` and `Kwargs` may have a default value, which will be used when the argument is not present on the commandline. These can be passed through the `set_default` function, it accepts either a string or the type of the parameter itself:
```c++
std::string &dst_path           = arg("An optional positional argument").set_default("output.png");
float &alpha                    = kwarg("a,alpha", "An optional float parameter with float as default").set_default(0.5f);
float &beta                     = kwarg("b,beta", "An optional float parameter with string as default").set_default("0.5");
std::vector<int> &numbers       = kwarg("n,numbers", "An optional vector of integers").set_default(std::vector<int>{1,2});
std::vector<int> &values        = kwarg("v,values", "An optional vector of integers, with string as default").set_default("3,4");
```

# Implicit values
`Kwargs` may have an implicit value, meaning that when the argument is present on the commandline, but no value is set, it will use the implicit value. Implicit values are passed as string.
```c++
int &k                          = kwarg("k", "An implicit int parameter", /*implicit*/"3");
float &alpha                    = kwarg("a,alpha", "A implicit float parameter", /*implicit*/"0.5");
std::vector<int> &numbers       = kwarg("n,numbers", "A implicit int vector", /*implicit*/"1,2,3");
```
Examples:
On the commandline the implicit values can be overwritten by using the `=` sign followed by the value. Examples:
```
$ argparse_test -k
k = 3
$ argparse_test -k=9
k = 9
$ argparse_test --numbers
numbers = 1,2,3
$ argparse_test --numbers=3,4,5
numbers = 3,4,5
```

# Vectors and multiple arguments
Argparse supports `std::vector`. There are 2 ways in which the vector can be read from the commandline, either a vector can be parsed from a comma-separated string, or by setting the `multi_argument()` flag to aggregate multiple program arguments into the vector, e.g. when using the `./*` in bash to list all the files in a directory.  
```c++
std::vector<int> &numbers           = kwarg("n,numbers", "An int vector");
std::vector<std::string> &tags      = kwarg("t,tags", "A word vector");
std::vector<std::string> &files     = kwarg("files", "multiple arguments").multi_argument();
```
Example usage:
```bash
$ argparse_test --numbers 3,4,5,6                
$ argparse_test --numbers=3,4,5,6
$ argparse_test --tags="hello"
$ argparse_test --tags="hello, world"
$ argparse_test --files a b c
$ argparse_test --files ./*               # files will now contain a list of the files in the current directory
```
In case there are other positional arguments, Argparse will make sure that they are correctly assigned. For example, consider the following example:
```c++
std::string &A = arg("Source path");
std::vector<std::string> &B = arg("Variable paths").multi_argument();
std::string &C = arg("Last");
```
And the following input:
```bash
$ argparse_test a b b b c
```
Argparse will assign the non-multiple arguments first, such that `A=a`, `C=c` and `B=b,b,b` 


# Pointers and Optionals
In situations where setting a default value is not sufficient, Argparse supports `std::optional`, and (smart)pointers, these can be used in situations where you'd like to distinguish whether an argument was set by the user. When declaring a raw pointer or a `std::shared_ptr`, the default value for these are automatically set to `nullptr` (or `std::nullopt` for `std::optional`). 
```c++
std::shared_ptr<float> &alpha   = kwarg("a,alpha", "An optional smart-pointer float parameter");
std::optional<float> &beta      = kwarg("b,beta", "An optional float parameter with std::optional return");
float* &gamma                   = kwarg("g,gamma", "An optional raw pointer float parameter");
```
For example:
```bash
$ ./argparse_test --alpha 0.4
   -a,--alpha : 0.4
    -b,--beta : none
   -g,--gamma : none
```

# Enums
One of the reasons for creating this library was to natively support Enums using [magic_enum](https://github.com/Neargye/magic_enum). If it is found on the system, Argparse supports automatic conversion from commandline to `enum`. Consider the following example:

```c++
enum Color {
    RED,
    BLUE,
    GREEN,
};

struct MyArgs : public argparse::Args {
    Color &color = kwarg("c,color", "An Enum input");
};

int main(int argc, char* argv[]) {
    auto args = argparse::parse<MyArgs>(argc, argv);
    args.print();      // prints all variables

    return 0;
}
```
Running it will automatically convert the input to the `Color` enum (case-insensitive):
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
When using a custom class, Argparse will try to create the class using the constructor with an `std::string` as parameter. See `examples/argparse_example.cpp` for an example using a custom class.


# Raise exception on error
When invalid arguments are passed to the commandline, argparse will simply print the error and exit the program by default. However, you can choose to let argparse throw a catchable exception instead by setting the `raise_on_error` flag to `true` on the `parse` function. For example:
```c++
int main(int argc, char* argv[]) {
     try {
        auto args = argparse::parse<MyArgs>(argc, argv, /*raise_on_error*/ true);
     } catch (const std::runtime_error &e) {
        std::cerr << "failed to parse arguments: " << e.what() << std::endl;
        return -1;
     }
    return 0;
}
```
# Examples and help flag
The `--help` is automatically added in ArgParse. Consider the following example usage when executing `argparse_test` (int `examples/argparse_example.cpp`): 
```
$ ./argparse_example --help
Welcome to Argparse
Usage: ./argparse_example arg_0 arg_1  [options...]
            arg_0 : Source path [required]
            arg_1 : Destination path [default: default_destination]

Options:
               -k : An implicit int parameter [implicit: "3", required]
       -a,--alpha : An optional float parameter with default value [default: 0.6]
        -b,--beta : An optional float parameter with std::optional return [default: none]
     -n,--numbers : An int vector, comma separated [required]
          --files : multiple arguments [required]
       -c,--color : An Enum input [allowed: <red, blue, green>, required]
     -v,--verbose : A flag to toggle verbose [implicit: "true", default: false]
           --help : print help [implicit: "true", default: false]
```
You may notice the `Welcome to Argparse` message, this message was created by overwriting the `welcome` function (see `examples/argparse_example.cpp`)

In case it fails to parse the input string, it will display an error and exit. E.g. here we'll set `k` to be `notanumber`: 
```
$ ./argparse_test src dst -k notanumber
Invalid argument, could not convert "notanumber" for -k (An implicit int parameter)
```

# Installing
Since it is a header-only library, you can simply copy the `include/argparse.hpp` file into your own project. 

Alternatively, you can build&install it using the following commands:
```
mkdir build && cd build
cmake ..
make
sudo make install
```

It can be included in your cmake project as follows:
```
find_package(argparse REQUIRED)

target_link_libraries(${PROJECT_NAME} PUBLIC argparse::argparse)
``` 

# FAQ
 - **Why references?**
   
    This is a good question. In order to support implicit parameters, multiple parameters and invariance to the order of input, Argparse needs to know all the possible input arguments before assigning them. And since the goal of this library is to define a variable only once, I needed a way to modify the contents of the returned value after it has seen all the arguments. Returning by reference makes this possible. In the future, when guaranteed copy-elision is implemented for primitive types, the reference can be removed.  
