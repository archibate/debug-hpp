# debug.hpp

Prints everything!

[English](README.md) | [简体中文](README.zh_CN.md)

> support from C++11 to C++20

## Usage

```cpp
debug(), "my variable is", your_variable;
```

Results in:
```
your_file.cpp:233:  my variable is {1, 2, 3}
```

> suppose your_variable is an `std::vector`

**WARNING**: debug() only works in `Debug` build! It is automatically disabled in `Release` build (we do this by checking whether the NDEBUG macro is defined). Yeah, completely no outputs in `Release` build, this is by design.

This is a feature for convenience: you don't have to busy removing all the debug() sentences after debug done, simply switch to `Release` build and everything debug is gone, no runtime overhead! And when you need debug simply switch back to `Debug` build and everything debug() you written before is back in life.

If you insist to use debug() even in `Release` build, please `#define DEBUG_LEVEL 1` before including this header file.


Assertion check is also supported:
```cpp
debug().check(some_variable) > 0;
```

Will trigger a 'trap' interrupt (__debugbreak for MSVC and __builtin_trap for GCC, configurable, see below) for the debugger to catch when `some_variable > 0` is false, as well as printing human readable error message:
```
your_file.cpp:233:  assertion failed: 3 < 0
```


After debugging complete, no need to busy removing all debug() calls! Simply:
```cpp
#define NDEBUG
```
would supress all debug() prints and assertion checks, completely no runtime overhead. For CMake or Visual Studio users, simply switch to `Release` build would supress debug() prints. Since they automatically define `NDEBUG` for you in `Release`, `RelWithDebInfo` and `MinSizeRel` build types.


TL;DR: This is a useful debugging utility the C++ programmers had all dreamed of:

1. print using the neat comma syntax, easy-to-use
2. supports printing STL objects including string, vector, tuple, optional, variant, unique_ptr, type_info, and so on. (C++20 required)
3. just add a member method named `repr`, e.g. `std::string repr() const { ... }` to support printing your custom class!
4. classes that are not supported to print will be shown in something like `[TypeName@0xdeadbeaf]` where 0xdeadbeaf is it's address.
5. highly configurable, customize the behaviour by defining the DEBUG_xxx macros (see below)
6. when debug done, supress all debug messages by simply `#define NDEBUG`, the whole library is disabled at compile-time, no runtime overhead
7. Thread safe, every line of message is always distinct, no annoying interleaving output rushing into console (typical experience when using cout)


## Configurations

Here is a list of configurable macros, define them **before** including this header file to take effect:

* `#define DEBUG_LEVEL 0` (default when defined NDEBUG) - disable debug output, completely no runtime overhead
* `#define DEBUG_LEVEL 1` (default when !defined NDEBUG) - enable debug output, prints everything you asked to print
* `#define DEBUG_LEVEL 2` - enable debug output with detailed source code level information (requires source files readable)

* `#define DEBUG_DEFAULT_OUTPUT std::cerr` (default) - controls where to output the debug strings

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

* `#define DEBUG_SHOW_NULLOPT "nullopt"` (default) - controls how to print optional-like objects (supporting *x and (bool)x) when it is nullopt

* `#define DEBUG_NAMESPACE_BEGIN` (default) - expose debug in the global namespace
* `#define DEBUG_NAMESPACE_END` (default) - ditto

* `#define DEBUG_NAMESPACE_BEGIN namespace mydebugger {` (default) - expose debug in the the namespace `mydebugger`, and use it like `mydebugger::debug()`
* `#define DEBUG_NAMESPACE_END }` (default) - ditto

* `#define DEBUG_CLASS_NAME debug` (default) - the default name for the debug class is `debug()`, you may define your custom name here
