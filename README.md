# debug.hpp

Tired of the dumb debugging of manually traversing containers and `cout <<` them one by one? 🥵

Try this handy header-only library 🚀 prints everything including STL containers without pain! 🤩

[English](README.md) | [简体中文](README.zh_CN.md)

- supports from C++11 to C++23 ✅
- neat syntax, easy-to-use ✅
- print almost all STL containers ✅
- custom class supported ✅
- automatically disabled in Release build ✅
- single header only ✅
- thread safe ✅

## 🎨 Usage

```cpp
debug(), "my variable is", your_variable;
```

The above code prints:

```
your_file.cpp:233:  my variable is {1, 2, 3}
```

> suppose your_variable is an `std::vector`

> [!WARNING]
> debug() only works in `Debug` build! It is automatically disabled in `Release` build (we do this by checking whether the NDEBUG macro is defined). Yeah, completely no outputs in `Release` build, this is by design.

This is a feature for convenience: you don't have to busy removing all the debug() sentences after debug done, simply switch to `Release` build and everything debug is gone, no runtime overhead! And when you need debug simply switch back to `Debug` build and everything debug() you written before is back in life.

If you mean to use debug() even in `Release` build, just `#define DEBUG_LEVEL 1` before including this header file to force enable debugging.

## ✨ Printing custom classes

```cpp
struct Student {
    std::string name;
    int age;

    // typically we just return a tuple directly, and debug() will dump this class just like a tuple:
    auto repr() const {
        return std::make_tuple(name, age);
    }

    // alternatively return a string:
    std::string repr() const {
        return "Student{name: " + name + " age: " + std::to_string(age) + "}";
    }

    // returning a map is also okay, the variant here will be automatically unwrapped and printed:
    auto repr() const {
        return std::map<std::string>, std::variant<std::string, int>>({"name", name}, {"age", age});
    }
};

// if adding member function is not possible, you may also define repr as free function within the same namespace as Student (thanks to C++'s ADL mechanism):
inline auto repr(Student const &stu) {
    return std::make_tuple(name, age);
}
```

> [!WARNING]
> make sure you have the `const` qualifier! otherwise debug() will refuse to invoke the repr function.

## 🎁 Save debug output as string

```cpp
auto s = std::string(debug(), "my variable is", your_variable);
// content of `s`: "your_file.cpp:233:  my variable is {1, 2, 3}"

auto s = std::string(debug().noloc(), "my variable is", your_variable);
// content of `s`: "my variable is {1, 2, 3}"
```

## 📝 Redirect debug output to spdlog

```cpp
#define DEBUG_OUTPUT(str) spdlog::info(x)  // define this macro before including the header to customize where debug() output its result
#include "debug.hpp"
```

## 🚩 Assertion check

```cpp
debug().check(some_variable) > 0;
```

Will trigger a 'trap' interrupt (__debugbreak for MSVC and __builtin_trap for GCC, configurable, see below) for the debugger to catch when `some_variable > 0` is false, as well as printing human readable error message:
```
your_file.cpp:233:  assertion failed: 3 < 0
```

## 🌠 Release build

After debugging complete, no need to busy removing all debug() calls! Simply:
```cpp
#define NDEBUG
```
would supress all debug() prints and assertion checks, completely no runtime overhead. For CMake or Visual Studio users, simply switch to `Release` build would supress debug() prints. Since they automatically define `NDEBUG` for you in `Release`, `RelWithDebInfo` and `MinSizeRel` build types.

## 😏 Tested compilers

- x86-64 gcc 4.8.1 (-std=c++11)
- x86-64 gcc 13.2 (-std=c++20)
- x86-64 gcc 9.1 (-std=c++11)
- x86-64 gcc 13.2 (-std=c++14)
- x86-64 gcc 9.1 (-std=c++17)
- x86-64 clang 6.0.0 (-std=c++11 -stdlib=libc++)
- armv7-a clang 9.0.0 (-std=c++11)
- armv8-a clang 18.1.0 (-std=c++23)
- x64 msvc v19.38 (/std:c++20)
- arm64 msvc v19.38 (/std:c++14)
- x64 msvc v19.31 (/std:c++17)
- x86 msvc v19.14 (/std:c++11)
- x64 msvc v19.latest (/std:c++latest)

See https://godbolt.org/z/jYdj4T44n

## 🏅 To conclude

TL;DR: This is a useful debugging utility the C++ programmers had all dreamed of:

1. print using the neat comma syntax, easy-to-use
2. supports printing STL objects including string, vector, tuple, optional, variant, unique_ptr, type_info, and so on.
3. just add a member method named `repr`, e.g. `std::string repr() const { ... }` to support printing your custom class!
4. classes that are not supported to print will be shown in something like `[TypeName@0xdeadbeaf]` where 0xdeadbeaf is it's address.
5. highly configurable, customize the behaviour by defining the DEBUG_xxx macros (see below)
6. when debug done, supress all debug messages by simply `#define NDEBUG`, the whole library is disabled at compile-time, no runtime overhead
7. Thread safe, every line of message is always distinct, no annoying interleaving output rushing into console (typical experience when using cout)

## 😜 Configurations

Here is a list of configurable macros, define them **before** including this header file to take effect:

* `#define DEBUG_LEVEL 0` (default when defined NDEBUG) - disable debug output, completely no runtime overhead
* `#define DEBUG_LEVEL 1` (default when !defined NDEBUG) - enable debug output, prints everything you asked to print

* `#define DEBUG_SHOW_LOCATION 1` (default) - show source location mark before each line of the debug output (e.g. "file.cpp:233")
* `#define DEBUG_SHOW_LOCATION 0` - do not show the location mark

* `#define DEBUG_OUTPUT std::cerr <<` (default) - controls where to output the debug strings (must be callable as DEBUG_OUTPUT(str))

* `#define DEBUG_PANIC_METHOD 0` - throws an runtime error with debug string as message when assertion failed
* `#define DEBUG_PANIC_METHOD 1` (default) - print the error message when assertion failed, then triggers a 'trap' interrupt, useful for debuggers to catch, if no debuggers attached, the program would terminate
* `#define DEBUG_PANIC_METHOD 2` - print the error message when assertion failed, and then call std::terminate
* `#define DEBUG_PANIC_METHOD 3` - print the error message when assertion failed, do not terminate, do not throw any exception

* `#define DEBUG_REPR_NAME repr` (default) - if an object x have a member function like `x.repr()` or a global function `repr` supporting `repr(x)`, then the value of `x.repr()` or `repr(x)` will be printed instead for this object

* `#define DEBUG_RANGE_BRACE "{}"` (default) - controls format for range-like objects (supporting begin(x) and end(x)) in "{1, 2, 3, ...}"
* `#define DEBUG_RANGE_COMMA ", "` (default) - ditto

* `#define DEBUG_TUPLE_BRACE "{}"` (default) - controls format for tuple-like objects (supporting std::tuple_size<X>) in "{1, 2, 3}"
* `#define DEBUG_TUPLE_COMMA ", "` (default) - ditto

* `#define DEBUG_MAGIC_ENUM magic_enum::enum_name` - enable printing enum in their name rather than value, if you have magic_enum.hpp

* `#define DEBUG_UNSIGNED_AS_HEXADECIMAL 0` (default) - print unsigned integers as decimal
* `#define DEBUG_UNSIGNED_AS_HEXADECIMAL 1` - print unsigned integers as hexadecimal
* `#define DEBUG_UNSIGNED_AS_HEXADECIMAL 2` - print unsigned integers as full-width hexadecimal

* `#define DEBUG_HEXADECIMAL_UPPERCASE 0` (default) - print hexadecimal values in lowercase (a-f)
* `#define DEBUG_HEXADECIMAL_UPPERCASE 1` - print hexadecimal values in uppercase (A-F)

* `#define DEBUG_SUPRESS_NON_ASCII 0` (default) - consider non-ascii characters in std::string as printable (e.g. UTF-8 encoded Chinese characters)
* `#define DEBUG_SUPRESS_NON_ASCII 1` - consider non-ascii characters in std::string as not printable (print them in e.g. '\xfe' instead)

* `#define DEBUG_SHOW_SOURCE_CODE_LINE 1` - enable debug output with detailed source code level information (requires readable source file path)

* `#define DEBUG_NULLOPT_STRING "nullopt"` (default) - controls how to print optional-like objects (supporting *x and (bool)x) when it is nullopt

* `#define DEBUG_NULLPTR_STRING "nullptr"` (default) - controls how to print pointer-like objects (supporting static_cast<void const volatile *>(x.get())) when it is nullptr

* `#define DEBUG_SMART_POINTER_MODE 1` (default) - print smart pointer as raw pointer address (e.g. 0xdeadbeaf)
* `#define DEBUG_SMART_POINTER_MODE 2` - print smart pointer as content value unless nullptr (e.g. 42)
* `#define DEBUG_SMART_POINTER_MODE 3` - print smart pointer as both content value and raw pointer address (e.g. 42@0xdeadbeaf)

* `#define DEBUG_NAMESPACE_BEGIN` (default) - expose debug in the global namespace
* `#define DEBUG_NAMESPACE_END` (default) - ditto

* `#define DEBUG_NAMESPACE_BEGIN namespace mydebugger {` - expose debug in the the namespace `mydebugger`, and use it like `mydebugger::debug()`
* `#define DEBUG_NAMESPACE_END }` - ditto

* `#define DEBUG_CLASS_NAME debug` (default) - the default name for the debug class is `debug()`, you may define your custom name here

## 💝 Questions?

What's your opinion for this handy printing utility? Any suggestions or feature request would be welcome in the [GitHub issues](https://github.com/archibate/debug-hpp/issues)!
