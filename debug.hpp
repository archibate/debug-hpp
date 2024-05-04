#pragma once
//
// debug.hpp - prints everything!
//
// Usage:
//   debug(), "my variable is", your_variable;
//
// Results in:
//   your_file.cpp:233:  my variable is {1, 2, 3}
//
// (suppose your_variable is an std::vector)
//
// **WARNING**:
//   debug() only works in `Debug` build! It is automatically disabled in `Release` build (we do this by checking whether the NDEBUG macro is defined). Yeah, completely no outputs in `Release` build, this is by design.
//
//   This is a feature for convenience, e.g. you don't have to remove all the debug() sentences after debug done, simply switch to `Release` build and everything debug is gone, no runtime overhead! And when you need debug simply switch back to `Debug` build and everything debug() you written before is back in life.
//
//   If you insist to use debug() even in `Release` build, please `#define DEBUG_LEVEL 1` before including this header file.
//
//
// Assertion check is also supported:
//   debug().check(some_variable) > 0;
//
// Will trigger a 'trap' interrupt (__debugbreak for MSVC and __builtin_trap for GCC, configurable, see below) for the debugger to catch when `some_variable > 0` is false, as well as printing human readable error message:
//   your_file.cpp:233:  assertion failed: 3 < 0
//
//
// After debugging complete, no need to busy removing all debug() calls! Simply:
//   #define NDEBUG
// would supress all debug() prints and assertion checks, completely no runtime overhead. For CMake or Visual Studio users, simply switch to `Release` build would supress debug() prints. Since they automatically define `NDEBUG` for you in `Release`, `RelWithDebInfo` and `MinSizeRel` build types.
//
//
// TL;DR: This is a useful debugging utility the C++ programmers had all dreamed of:
//
//   1. print using the neat comma syntax, easy-to-use
//   2. supports printing STL objects including string, vector, tuple, optional, variant, unique_ptr, type_info, and so on. (C++20 required)
//   3. just add a member method named `repr`, e.g. `std::string repr() const { ... }` to support printing your custom class!
//   4. classes that are not supported to print will be shown in something like `[TypeName@0xdeadbeaf]` where 0xdeadbeaf is it's address.
//   5. highly configurable, customize the behaviour by defining the DEBUG_xxx macros (see below)
//   6. when debug done, supress all debug messages by simply `#define NDEBUG`, the whole library is disabled at compile-time, no runtime overhead
//   7. Thread safe, every line of message is always distinct, no annoying interleaving output rushing into console (typical experience when using cout)
//
//
// Here is a list of configurable macros, define them **before** including this header file to take effect:
//
// `#define DEBUG_LEVEL 0` (default when defined NDEBUG) - disable debug output, completely no runtime overhead
// `#define DEBUG_LEVEL 1` (default when !defined NDEBUG) - enable debug output, prints everything you asked to print
//
// `#define DEBUG_SHOW_SOURCE_LOCATION 1` (default) - show source location mark before each line of the debug output (e.g. "file.cpp:233")
// `#define DEBUG_SHOW_SOURCE_LOCATION 0` - do not show the location mark
//
// `#define DEBUG_DEFAULT_OUTPUT std::cerr` (default) - controls where to output the debug strings (must be std::ostream &)
//
// `#define DEBUG_PANIC_METHOD 0` - throws an runtime error with debug string as message when assertion failed
// `#define DEBUG_PANIC_METHOD 1` (default) - print the error message when assertion failed, then triggers a 'trap' interrupt, useful for debuggers to catch, if no debuggers attached, the program would terminate
// `#define DEBUG_PANIC_METHOD 2` - print the error message when assertion failed, and then call std::terminate
// `#define DEBUG_PANIC_METHOD 3` - print the error message when assertion failed, do not terminate, do not throw any exception
//
// `#define DEBUG_REPR_NAME repr` (default) - if an object x have a member function like `x.repr()` or a global function `repr` supporting `repr(x)`, then the value of `x.repr()` or `repr(x)` will be printed instead for this object
//
// `#define DEBUG_RANGE_BRACE "{}"` (default) - controls format for range-like objects (supporting begin(x) and end(x)) in "{1, 2, 3, ...}"
// `#define DEBUG_RANGE_COMMA ", "` (default) - ditto
//
// `#define DEBUG_TUPLE_BRACE "{}"` (default) - controls format for tuple-like objects (supporting std::tuple_size<X>) in "{1, 2, 3}"
// `#define DEBUG_TUPLE_COMMA ", "` (default) - ditto
//
// `#define DEBUG_MAGIC_ENUM magic_enum::enum_name` - enable printing enum in their name rather than value, if you have magic_enum.hpp
//
// `#define DEBUG_UNSIGNED_AS_HEXADECIMAL 0` (default) - print unsigned integers as decimal
// `#define DEBUG_UNSIGNED_AS_HEXADECIMAL 1` - print unsigned integers as hexadecimal
// `#define DEBUG_UNSIGNED_AS_HEXADECIMAL 2` - print unsigned integers as full-width hexadecimal
//
// `#define DEBUG_HEXADECIMAL_UPPERCASE 0` (default) - print hexadecimal values in lowercase (a-f)
// `#define DEBUG_HEXADECIMAL_UPPERCASE 1` - print hexadecimal values in uppercase (A-F)
//
// `#define DEBUG_SUPRESS_NON_ASCII 0` (default) - consider non-ascii characters in std::string as printable (e.g. UTF-8 encoded Chinese characters)
// `#define DEBUG_SUPRESS_NON_ASCII 1` - consider non-ascii characters in std::string as not printable (print them in e.g. '\xfe' instead)
//
// `#define DEBUG_SHOW_SOURCE_CODE_LINE 1` - enable debug output with detailed source code level information (requires readable source file path)
//
// `#define DEBUG_SHOW_NULLOPT "nullopt"` (default) - controls how to print optional-like objects (supporting *x and (bool)x) when it is nullopt
//
// `#define DEBUG_NAMESPACE_BEGIN` (default) - expose debug in the global namespace
// `#define DEBUG_NAMESPACE_END` (default) - ditto
//
// `#define DEBUG_NAMESPACE_BEGIN namespace mydebugger {` (default) - expose debug in the the namespace `mydebugger`, and use it like `mydebugger::debug()`
// `#define DEBUG_NAMESPACE_END }` (default) - ditto
//
// `#define DEBUG_CLASS_NAME debug` (default) - the default name for the debug class is `debug()`, you may define your custom name here
//

#ifndef DEBUG_LEVEL
# ifdef NDEBUG
#  define DEBUG_LEVEL 0
# else
#  define DEBUG_LEVEL 1
# endif
#endif

#ifndef DEBUG_SHOW_SOURCE_LOCATION
#define DEBUG_SHOW_SOURCE_LOCATION 1
#endif

#ifndef DEBUG_REPR_NAME
# define DEBUG_REPR_NAME repr
#endif

#ifndef DEBUG_NAMESPACE_BEGIN
# define DEBUG_NAMESPACE_BEGIN
#endif

#ifndef DEBUG_NAMESPACE_END
# define DEBUG_NAMESPACE_END
#endif

#ifndef DEBUG_DEFAULT_OUTPUT
# define DEBUG_DEFAULT_OUTPUT std::cerr
#endif

#ifndef DEBUG_DEFAULT_ENABLED
# define DEBUG_DEFAULT_ENABLED true
#endif

#ifndef DEBUG_ENABLE_FILES_MATCH
# define DEBUG_ENABLE_FILES_MATCH 0
#endif

#ifndef DEBUG_PANIC_METHOD
# define DEBUG_PANIC_METHOD 1
#endif

#ifndef DEBUG_SUPRESS_NON_ASCII
# define DEBUG_SUPRESS_NON_ASCII 0
#endif

#ifndef DEBUG_SEPARATOR_TAB
# define DEBUG_SEPARATOR_TAB '\t'
#endif

#ifndef DEBUG_RANGE_BRACE
# define DEBUG_RANGE_BRACE "{}"
#endif

#ifndef DEBUG_RANGE_COMMA
# define DEBUG_RANGE_COMMA ", "
#endif

#ifndef DEBUG_TUPLE_BRACE
# define DEBUG_TUPLE_BRACE "{}"
#endif

#ifndef DEBUG_TUPLE_COMMA
# define DEBUG_TUPLE_COMMA ", "
#endif

#ifndef DEBUG_ENUM_BRACE
# define DEBUG_ENUM_BRACE "()"
#endif

#ifndef DEBUG_SHOW_NULLOPT
# define DEBUG_SHOW_NULLOPT "nullopt"
#endif

#ifndef DEBUG_UNKNOWN_TYPE_BRACE
# define DEBUG_UNKNOWN_TYPE_BRACE "[]"
#endif

#ifndef DEBUG_UNKNOWN_TYPE_AT
# define DEBUG_UNKNOWN_TYPE_AT '@'
#endif

#ifndef DEBUG_POINTER_HEXADECIMAL_PREFIX
# define DEBUG_POINTER_HEXADECIMAL_PREFIX "0x"
#endif

#ifndef DEBUG_UNSIGNED_AS_HEXADECIMAL
# define DEBUG_UNSIGNED_AS_HEXADECIMAL 0
#endif

#ifndef DEBUG_HEXADECIMAL_UPPERCASE
# define DEBUG_HEXADECIMAL_UPPERCASE 0
#endif

#ifdef DEBUG_CLASS_NAME
# define debug DEBUG_CLASS_NAME
#endif

#if DEBUG_LEVEL

# include <limits>
# include <cstdint>
# include <cstdlib>
# include <iomanip>
# include <iostream>
# if DEBUG_LEVEL >= 2
#  include <fstream>
#  include <unordered_map>
# endif
# ifndef DEBUG_SOURCE_LOCATION
#  if defined(__has_include)
#   if __has_include(<source_location>)
#    include <source_location>
#    if __cpp_lib_source_location
#     define DEBUG_SOURCE_LOCATION std::source_location
#    endif
#   endif
#  endif
# endif
# ifndef DEBUG_SOURCE_LOCATION
#  if defined(__has_include)
#   if __has_include(<experimental/source_location>)
#    include <experimental/source_location>
#    if __cpp_lib_experimental_source_location
#     define DEBUG_SOURCE_LOCATION std::experimental::source_location
#    endif
#   endif
#  endif
# endif
# include <type_traits>
# include <utility>
# include <typeinfo>
# include <sstream>
# include <string>
# include <memory>
# ifndef DEBUG_CUSTOM_DEMANGLE
#  ifndef DEBUG_HAS_CXXABI_H
#   if defined(__has_include)
#    if defined(__unix__) && __has_include(<cxxabi.h>)
#     include <cxxabi.h>
#     define DEBUG_HAS_CXXABI_H
#    endif
#   endif
#  else
#   include <cxxabi.h>
#  endif
# endif
# ifndef DEBUG_STRING_VIEW
#  if defined(__has_include)
#   if __has_include(<string_view>)
#    include <string_view>
#    if __cpp_lib_string_view
#     define DEBUG_STRING_VIEW std::string_view
#    endif
#   endif
#  endif
# endif
# ifndef DEBUG_STRING_VIEW
#  if defined(__has_include)
#   if __has_include(<experimental/string_view>)
#    include <experimental/string_view>
#    if __cpp_lib_experimental_string_view
#     define DEBUG_STRING_VIEW std::experimental::string_view
#    endif
#   endif
#  endif
# endif
# ifndef DEBUG_STRING_VIEW
#  include <string>
#  define DEBUG_STRING_VIEW std::string
# endif
# if defined(__has_cpp_attribute)
#  if __has_cpp_attribute(unlikely)
#   define DEBUG_UNLIKELY [[unlikely]]
#  else
#   define DEBUG_UNLIKELY
#  endif
#  if __has_cpp_attribute(nodiscard)
#   define DEBUG_NODISCARD [[nodiscard]]
#  else
#   define DEBUG_NODISCARD
#  endif
# else
#  define DEBUG_UNLIKELY
#  define DEBUG_NODISCARD
# endif

DEBUG_NAMESPACE_BEGIN

namespace debug_details {

# ifndef DEBUG_SOURCE_LOCATION
struct debug_source_location {
    char const *fn;
    int ln;
    int col;
    char const *fun;

    char const *file_name() const noexcept {
        return fn;
    }

    int line() const noexcept {
        return ln;
    }

    int column() const noexcept {
        return col;
    }

    char const *function_name() const noexcept {
        return fun;
    }

    static debug_source_location current() noexcept {
        return {"???", 0, 0, "?"};
    }
};

#  ifndef DEBUG_SOURCE_LOCATION_FAKER
#   define DEBUG_SOURCE_LOCATION_FAKER \
       { __FILE__, __LINE__, 0, __func__ }
#  endif

#  define DEBUG_SOURCE_LOCATION debug_details::debug_source_location
# endif

static void debug_quotes(std::ostream &oss, DEBUG_STRING_VIEW sv, char quote) {
    oss << quote;
    for (char c: sv) {
        switch (c) {
        case '\n': oss << "\\n"; break;
        case '\r': oss << "\\r"; break;
        case '\t': oss << "\\t"; break;
        case '\\': oss << "\\\\"; break;
        case '\0': oss << "\\0"; break;
        default:
            if ((c >= 0 && c < 0x20) || c == 0x7F
# if DEBUG_SUPRESS_NON_ASCII
                || (static_cast<unsigned char>(c) >= 0x80)
# endif
            ) {
                auto f = oss.flags();
                oss << "\\x" << std::hex << std::setfill('0') << std::setw(2)
                    << static_cast<int>(c)
# if EBUG_HEXADECIMAL_UPPERCASE
                    << std::uppercase
# endif
                    ;
                oss.flags(f);
            } else {
                if (c == quote) {
                    oss << '\\';
                }
                oss << c;
            }
            break;
        }
    }
    oss << quote;
}

# ifdef DEBUG_CUSTOM_DEMANGLE
static std::string debug_demangle(char const *name) {
    return DEBUG_CUSTOM_DEMANGLE(name);
}
# else
static std::string debug_demangle(char const *name) {
#  ifdef DEBUG_HAS_CXXABI_H
    int status;
    char *p = abi::__cxa_demangle(name, 0, 0, &status);
    std::string s = p ? p : name;
    std::free(p);
#  else
    std::string s = name;
#  endif
    return s;
}
# endif

# if __cpp_if_constexpr && __cpp_concepts && \
     __cpp_lib_type_trait_variable_templates

template <class T>
static void debug_format(std::ostream &oss, T const &t) {
    using std::begin;
    using std::end;
    if constexpr ((std::is_convertible<T, DEBUG_STRING_VIEW>::value ||
                   std::is_convertible<T, std::string>::value) &&
                  !std::is_same<T, char const *>::value) {
        if constexpr (!std::is_convertible<T, DEBUG_STRING_VIEW>::value) {
            std::string s = t;
            debug_quotes(oss, s, '"');
        } else {
            debug_quotes(oss, t, '"');
        }
    } else if constexpr (std::is_same<T, bool>::value) {
        auto f = oss.flags();
        oss << std::boolalpha << t;
        oss.flags(f);
    } else if constexpr (std::is_same<T, char>::value ||
                         std::is_same<T, signed char>::value) {
        debug_quotes(oss, {reinterpret_cast<char const *>(&t), 1}, '\'');
    } else if constexpr (
#  if __cpp_char8_t
        std::is_same<T, char8_t>::value ||
#  endif
        std::is_same<T, char16_t>::value || std::is_same<T, char32_t>::value) {
        auto f = oss.flags();
        oss << "'\\"
            << " xu U"[sizeof(T)] << std::hex << std::setfill('0')
            << std::setw(sizeof(T) * 2)
#  if DEBUG_HEXADECIMAL_UPPERCASE
            << std::uppercase
#  endif
            << static_cast<std::uint32_t>(t) << "'";
        oss.flags(f);
#  if DEBUG_UNSIGNED_AS_HEXADECIMAL
    } else if constexpr (std::is_integral<T>::value &&
                         std::is_unsigned<T>::value) {
        auto f = oss.flags();
        oss << "0x" << std::hex << std::setfill('0')
#   if DEBUG_UNSIGNED_AS_HEXADECIMAL >= 2
            << std::setw(sizeof(T) * 2)
#   endif
#   if DEBUG_HEXADECIMAL_UPPERCASE
            << std::uppercase
#   endif
            ;
        if constexpr (sizeof(T) == 1) {
            oss << static_cast<unsigned int>(t);
        } else {
            oss << t;
        }
        oss.flags(f);
#  endif
    } else if constexpr (std::is_floating_point<T>::value) {
        auto f = oss.flags();
        oss << std::fixed << std::setprecision(std::numeric_limits<T>::digits10)
            << t;
        oss.flags(f);
    } else if constexpr (requires(std::ostream &oss, T const &t) {
                             oss << t;
                         }) {
        oss << t;
    } else if constexpr (std::is_pointer<T>::value) {
        auto f = oss.flags();
        oss << DEBUG_POINTER_HEXADECIMAL_PREFIX << std::hex << std::setfill('0')
#  if DEBUG_HEXADECIMAL_UPPERCASE
            << std::uppercase
#  endif
            ;
        oss << reinterpret_cast<std::uintptr_t>(
            reinterpret_cast<void const volatile *>(t));
        oss.flags(f);
    } else if constexpr (requires(T const &t) { begin(t) != end(t); }) {
        oss << DEBUG_RANGE_BRACE[0];
        bool add_comma = false;
        for (auto &&i: t) {
            if (add_comma) {
                oss << DEBUG_RANGE_COMMA;
            }
            add_comma = true;
            debug_format(oss, std::forward<decltype(i)>(i));
        }
        oss << DEBUG_RANGE_BRACE[1];
    } else if constexpr (requires(T const &t) { std::tuple_size<T>::value; }) {
        oss << DEBUG_TUPLE_BRACE[0];
        bool add_comma = false;
        std::apply(
            [&](auto &&...args) {
                (([&] {
                     if (add_comma) {
                         oss << DEBUG_TUPLE_COMMA;
                     }
                     add_comma = true;
                     debug_format(oss, std::forward<decltype(args)>(args));
                 }()),
                 ...);
            },
            t);
        oss << DEBUG_TUPLE_BRACE[1];
    } else if constexpr (std::is_enum<T>::value) {
#  ifdef DEBUG_MAGIC_ENUM
        oss << DEBUG_MAGIC_ENUM(t);
#  else
        oss << debug_demangle(typeid(T).name()) << DEBUG_ENUM_BRACE[0];
        oss << static_cast<typename std::underlying_type<T>::type>(t);
        oss << DEBUG_ENUM_BRACE[1];
#  endif
    } else if constexpr (std::is_same<T, std::type_info>::value) {
        oss << debug_demangle(t.name());
    } else if constexpr (requires(T const &t) { t.DEBUG_REPR_NAME(); }) {
        debug_format(oss, t.DEBUG_REPR_NAME());
    } else if constexpr (requires(T const &t) { t.DEBUG_REPR_NAME(oss); }) {
        t.DEBUG_REPR_NAME(oss);
    } else if constexpr (requires(T const &t) { DEBUG_REPR_NAME(t); }) {
        debug_format(oss, DEBUG_REPR_NAME(t));
    } else if constexpr (requires(T const &t) { DEBUG_REPR_NAME(oss, t); }) {
        DEBUG_REPR_NAME(oss, t);
    } else if constexpr (requires(T const &t) {
                             visit([&oss](auto const &) {}, t);
                         }) {
        visit([&oss](auto const &t) { debug_format(oss, t); }, t);
    } else if constexpr (requires(T const &t) {
                             (*t);
                             (bool)t;
                         }) {
        if ((bool)t) {
            debug_format(oss, *t);
        } else {
            oss << DEBUG_SHOW_NULLOPT;
        }
    } else {
        oss << DEBUG_UNKNOWN_TYPE_BRACE[0] << debug_demangle(typeid(t).name())
            << DEBUG_UNKNOWN_TYPE_AT;
        debug_format(oss, reinterpret_cast<void const *>(std::addressof(t)));
        oss << DEBUG_UNKNOWN_TYPE_BRACE[1];
    }
}

# else

template <class T>
struct debug_void {
    using type = void;
};

template <bool v>
struct debug_bool_constant {
    enum {
        value = v
    };
};

#  define DEBUG_COND(n, ...)                                                  \
      template <class T, class = void>                                        \
      struct debug_cond_##n : std::false_type {};                             \
      template <class T>                                                      \
      struct debug_cond_##n<T,                                                \
                            typename debug_void<decltype(__VA_ARGS__)>::type> \
          : std::true_type {};

DEBUG_COND(is_ostream_ok, std::declval<std::ostream &>()
                              << std::declval<T const &>());
DEBUG_COND(is_range,
           begin(std::declval<T const &>()) != end(std::declval<T const &>()));
DEBUG_COND(is_tuple, std::tuple_size<T>::value);
DEBUG_COND(is_member_repr, std::declval<T const &>().DEBUG_REPR_NAME());
DEBUG_COND(is_member_repr_stream, std::declval<T const &>().DEBUG_REPR_NAME(
                                      std::declval<std::ostream &>()));
DEBUG_COND(is_adl_repr, DEBUG_REPR_NAME(std::declval<T const &>()));
DEBUG_COND(is_adl_repr_stream, DEBUG_REPR_NAME(std::declval<std::ostream &>(),
                                               std::declval<T const &>()));

struct variant_test_lambda {
    std::ostream &oss;

    template <class T>
    void operator()(T const &) const {}
};

DEBUG_COND(is_variant, visit(std::declval<variant_test_lambda const &>(),
                             std::declval<T const &>()));
DEBUG_COND(is_optional,
           ((*std::declval<T>(), (void)0), ((bool)std::declval<T>(), (void)0)));

#  define DEBUG_CON(n, ...) \
      template <class T>    \
      struct debug_cond_##n : debug_bool_constant<__VA_ARGS__> {};

DEBUG_CON(string, std::is_convertible<T, DEBUG_STRING_VIEW>::value ||
                      std::is_convertible<T, std::string>::value &&
                          !std::is_same<T, char const *>::value);
DEBUG_CON(bool, std::is_same<T, bool>::value);
DEBUG_CON(char,
          std::is_same<T, char>::value || std::is_same<T, signed char>::value);
#  if __cpp_char8_t
DEBUG_CON(unicode_char, std::is_same<T, char8_t>::value ||
                            std::is_same<T, char16_t>::value ||
                            std::is_same<T, char32_t>::value ||
                            std::is_same<T, wchar_t>::value);
#  else
DEBUG_CON(unicode_char, std::is_same<T, char16_t>::value ||
                            std::is_same<T, char32_t>::value ||
                            std::is_same<T, wchar_t>::value);
#  endif
#  if DEBUG_UNSIGNED_AS_HEXADECIMAL
DEBUG_CON(integral_unsigned,
          std::is_integral<T>::value &&std::is_unsigned<T>::value);
#  else
DEBUG_CON(integral_unsigned, false);
#  endif
DEBUG_CON(floating_point, std::is_floating_point<T>::value);
DEBUG_CON(pointer, std::is_pointer<T>::value);
DEBUG_CON(enum, std::is_enum<T>::value);
DEBUG_CON(type_info, std::is_same<T, std::type_info>::value);

template <class T, class = void>
struct debug_format_trait;

template <class T>
static void debug_format(std::ostream &oss, T const &t) {
    debug_format_trait<T>()(oss, t);
}

template <class T, class>
struct debug_format_trait {
    void operator()(std::ostream &oss, T const &t) const {
        oss << t;
        oss << DEBUG_UNKNOWN_TYPE_BRACE[0] << debug_demangle(typeid(t).name())
            << DEBUG_UNKNOWN_TYPE_AT;
        debug_format(oss, reinterpret_cast<void const *>(std::addressof(t)));
        oss << DEBUG_UNKNOWN_TYPE_BRACE[1];
    }
};

template <class T>
struct debug_format_trait<
    T, typename std::enable_if<
           debug_cond_string<T>::value &&
           !std::is_convertible<T, DEBUG_STRING_VIEW>::value>::type> {
    void operator()(std::ostream &oss, T const &t) const {
        std::string s = t;
        debug_quotes(oss, s, '"');
    }
};

template <class T>
struct debug_format_trait<
    T, typename std::enable_if<
           debug_cond_string<T>::value &&
           std::is_convertible<T, DEBUG_STRING_VIEW>::value>::type> {
    void operator()(std::ostream &oss, T const &t) const {
        debug_quotes(oss, t, '"');
    }
};

template <class T>
struct debug_format_trait<
    T, typename std::enable_if<!debug_cond_string<T>::value &&
                               debug_cond_bool<T>::value>::type> {
    void operator()(std::ostream &oss, T const &t) const {
        auto f = oss.flags();
        oss << std::boolalpha << t;
        oss.flags(f);
    }
};

template <class T>
struct debug_format_trait<
    T, typename std::enable_if<!debug_cond_string<T>::value &&
                               !debug_cond_bool<T>::value &&
                               debug_cond_char<T>::value>::type> {
    void operator()(std::ostream &oss, T const &t) const {
        debug_quotes(oss, {reinterpret_cast<char const *>(&t), 1}, '\'');
    }
};

template <class T>
struct debug_format_trait<
    T, typename std::enable_if<!debug_cond_string<T>::value &&
                               !debug_cond_bool<T>::value &&
                               !debug_cond_char<T>::value &&
                               debug_cond_unicode_char<T>::value>::type> {
    void operator()(std::ostream &oss, T const &t) const {
        auto f = oss.flags();
        oss << "'\\"
            << " xu U"[sizeof(T)] << std::hex << std::setfill('0')
            << std::setw(sizeof(T) * 2)
#  if DEBUG_HEXADECIMAL_UPPERCASE
            << std::uppercase
#  endif
            << static_cast<std::uint32_t>(t) << "'";
        oss.flags(f);
    }
};

template <class T>
struct debug_format_trait<
    T, typename std::enable_if<
           !debug_cond_string<T>::value && !debug_cond_bool<T>::value &&
           !debug_cond_char<T>::value && !debug_cond_unicode_char<T>::value &&
           debug_cond_integral_unsigned<T>::value>::type> {
    void operator()(std::ostream &oss, T const &t) const {
        auto f = oss.flags();
        oss << "0x" << std::hex << std::setfill('0')
#  if DEBUG_UNSIGNED_AS_HEXADECIMAL >= 2
            << std::setw(sizeof(T) * 2)
#  endif
#  if DEBUG_HEXADECIMAL_UPPERCASE
            << std::uppercase
#  endif
            ;
        if
#  if __cpp_if_constexpr
            constexpr
#  endif
            (sizeof(T) == 1) {
            oss << static_cast<unsigned int>(t);
        } else {
            oss << t;
        }
        oss.flags(f);
    }
};

template <class T>
struct debug_format_trait<
    T, typename std::enable_if<
           !debug_cond_string<T>::value && !debug_cond_bool<T>::value &&
           !debug_cond_char<T>::value && !debug_cond_unicode_char<T>::value &&
           !debug_cond_integral_unsigned<T>::value &&
           debug_cond_floating_point<T>::value>::type> {
    void operator()(std::ostream &oss, T const &t) const {
        auto f = oss.flags();
        oss << std::fixed << std::setprecision(std::numeric_limits<T>::digits10)
            << t;
        oss.flags(f);
    }
};

template <class T>
struct debug_format_trait<
    T, typename std::enable_if<
           !debug_cond_string<T>::value && !debug_cond_bool<T>::value &&
           !debug_cond_char<T>::value && !debug_cond_unicode_char<T>::value &&
           !debug_cond_integral_unsigned<T>::value &&
           !debug_cond_floating_point<T>::value &&
           debug_cond_is_ostream_ok<T>::value>::type> {
    void operator()(std::ostream &oss, T const &t) const {
        oss << t;
    }
};

template <class T>
struct debug_format_trait<
    T, typename std::enable_if<
           !debug_cond_string<T>::value && !debug_cond_bool<T>::value &&
           !debug_cond_char<T>::value && !debug_cond_unicode_char<T>::value &&
           !debug_cond_integral_unsigned<T>::value &&
           !debug_cond_floating_point<T>::value &&
           !debug_cond_is_ostream_ok<T>::value &&
           debug_cond_pointer<T>::value>::type> {
    void operator()(std::ostream &oss, T const &t) const {
        auto f = oss.flags();
        oss << DEBUG_POINTER_HEXADECIMAL_PREFIX << std::hex << std::setfill('0')
#  if DEBUG_HEXADECIMAL_UPPERCASE
            << std::uppercase
#  endif
            ;
        oss << reinterpret_cast<std::uintptr_t>(
            reinterpret_cast<void const volatile *>(t));
        oss.flags(f);
    }
};

template <class T>
struct debug_format_trait<
    T,
    typename std::enable_if<
        !debug_cond_string<T>::value && !debug_cond_bool<T>::value &&
        !debug_cond_char<T>::value && !debug_cond_unicode_char<T>::value &&
        !debug_cond_integral_unsigned<T>::value &&
        !debug_cond_floating_point<T>::value &&
        !debug_cond_is_ostream_ok<T>::value && !debug_cond_pointer<T>::value &&
        debug_cond_is_range<T>::value>::type> {
    void operator()(std::ostream &oss, T const &t) const {
        oss << DEBUG_RANGE_BRACE[0];
        bool add_comma = false;
        auto b = begin(t);
        auto e = end(t);
        for (auto it = b; it != e; ++it) {
            if (add_comma) {
                oss << DEBUG_RANGE_COMMA;
            }
            add_comma = true;
            debug_format(oss, std::forward<decltype(*it)>(*it));
        }
        oss << DEBUG_RANGE_BRACE[1];
    }
};

#  if __cpp_lib_integer_sequence
template <class F, class Tuple, std::size_t... I>
void debug_apply_impl(F &&f, Tuple &&t, std::index_sequence<I...>) {
    std::forward<F>(f)(std::get<I>(std::forward<Tuple>(t))...);
}

template <class F, class Tuple, std::size_t... I>
void debug_apply(F &&f, Tuple &&t) {
    debug_apply_impl(
        std::forward<F>(f), std::forward<Tuple>(t),
        std::make_index_sequence<
            std::tuple_size<typename std::decay<Tuple>::type>::value>{});
}
#  else
template <std::size_t... I>
struct debug_index_sequence {};

template <std::size_t N, std::size_t... I>
struct debug_make_index_sequence
    : debug_make_index_sequence<N - 1, I..., N - 1> {};

template <std::size_t... I>
struct debug_make_index_sequence<0, I...> : debug_index_sequence<I...> {};

template <class F, class Tuple, std::size_t... I>
void debug_apply_impl(F &&f, Tuple &&t, debug_index_sequence<I...>) {
    return std::forward<F>(f)(std::get<I>(std::forward<Tuple>(t))...);
}

template <class F, class Tuple, std::size_t... I>
void debug_apply(F &&f, Tuple &&t) {
    return debug_apply_impl(
        std::forward<F>(f), std::forward<Tuple>(t),
        debug_make_index_sequence<
            std::tuple_size<typename std::decay<Tuple>::type>::value>{});
}
#  endif

struct debug_apply_lambda {
    std::ostream &oss;
    bool &add_comma;

    template <class... Args>
    void operator()(Args &&...args) const {
        int unused[] = {(
            [&] {
                if (add_comma) {
                    oss << DEBUG_TUPLE_COMMA;
                }
                add_comma = true;
                debug_format(oss, std::forward<decltype(args)>(args));
            }(),
            0)...};
        (void)unused;
    }
};

template <class T>
struct debug_format_trait<
    T, typename std::enable_if<
           !debug_cond_string<T>::value && !debug_cond_bool<T>::value &&
           !debug_cond_char<T>::value && !debug_cond_unicode_char<T>::value &&
           !debug_cond_integral_unsigned<T>::value &&
           !debug_cond_floating_point<T>::value &&
           !debug_cond_is_ostream_ok<T>::value &&
           !debug_cond_pointer<T>::value && !debug_cond_is_range<T>::value &&
           debug_cond_is_tuple<T>::value>::type> {
    void operator()(std::ostream &oss, T const &t) const {
        oss << DEBUG_TUPLE_BRACE[0];
        bool add_comma = false;
        debug_apply(debug_apply_lambda{oss, add_comma}, t);
        oss << DEBUG_TUPLE_BRACE[1];
    }
};

template <class T>
struct debug_format_trait<
    T, typename std::enable_if<
           !debug_cond_string<T>::value && !debug_cond_bool<T>::value &&
           !debug_cond_char<T>::value && !debug_cond_unicode_char<T>::value &&
           !debug_cond_integral_unsigned<T>::value &&
           !debug_cond_floating_point<T>::value &&
           !debug_cond_is_ostream_ok<T>::value &&
           !debug_cond_pointer<T>::value && !debug_cond_is_range<T>::value &&
           !debug_cond_is_tuple<T>::value && debug_cond_enum<T>::value>::type> {
    void operator()(std::ostream &oss, T const &t) const {
#  ifdef DEBUG_MAGIC_ENUM
        oss << DEBUG_MAGIC_ENUM(t);
#  else
        oss << debug_demangle(typeid(T).name()) << DEBUG_ENUM_BRACE[0];
        oss << static_cast<typename std::underlying_type<T>::type>(t);
        oss << DEBUG_ENUM_BRACE[1];
#  endif
    }
};

template <>
struct debug_format_trait<std::type_info, void> {
    void operator()(std::ostream &oss, std::type_info const &t) const {
        oss << debug_demangle(t.name());
    }
};

template <class T>
struct debug_format_trait<
    T, typename std::enable_if<
           !debug_cond_string<T>::value && !debug_cond_bool<T>::value &&
           !debug_cond_char<T>::value && !debug_cond_unicode_char<T>::value &&
           !debug_cond_integral_unsigned<T>::value &&
           !debug_cond_floating_point<T>::value &&
           !debug_cond_is_ostream_ok<T>::value &&
           !debug_cond_pointer<T>::value && !debug_cond_is_range<T>::value &&
           !debug_cond_is_tuple<T>::value && !debug_cond_enum<T>::value &&
           debug_cond_is_member_repr<T>::value>::type> {
    void operator()(std::ostream &oss, T const &t) const {
        oss << t.DEBUG_REPR_NAME();
    }
};

template <class T>
struct debug_format_trait<
    T,
    typename std::enable_if<
        !debug_cond_string<T>::value && !debug_cond_bool<T>::value &&
        !debug_cond_char<T>::value && !debug_cond_unicode_char<T>::value &&
        !debug_cond_integral_unsigned<T>::value &&
        !debug_cond_floating_point<T>::value &&
        !debug_cond_is_ostream_ok<T>::value && !debug_cond_pointer<T>::value &&
        !debug_cond_is_range<T>::value && !debug_cond_is_tuple<T>::value &&
        !debug_cond_enum<T>::value && !debug_cond_is_member_repr<T>::value &&
        debug_cond_is_member_repr_stream<T>::value>::type> {
    void operator()(std::ostream &oss, T const &t) const {
        t.DEBUG_REPR_NAME(oss);
    }
};

template <class T>
struct debug_format_trait<
    T,
    typename std::enable_if<
        !debug_cond_string<T>::value && !debug_cond_bool<T>::value &&
        !debug_cond_char<T>::value && !debug_cond_unicode_char<T>::value &&
        !debug_cond_integral_unsigned<T>::value &&
        !debug_cond_floating_point<T>::value &&
        !debug_cond_is_ostream_ok<T>::value && !debug_cond_pointer<T>::value &&
        !debug_cond_is_range<T>::value && !debug_cond_is_tuple<T>::value &&
        !debug_cond_enum<T>::value && !debug_cond_is_member_repr<T>::value &&
        !debug_cond_is_member_repr_stream<T>::value &&
        debug_cond_is_adl_repr<T>::value>::type> {
    void operator()(std::ostream &oss, T const &t) const {
        oss << DEBUG_REPR_NAME(t);
    }
};

template <class T>
struct debug_format_trait<
    T,
    typename std::enable_if<
        !debug_cond_string<T>::value && !debug_cond_bool<T>::value &&
        !debug_cond_char<T>::value && !debug_cond_unicode_char<T>::value &&
        !debug_cond_integral_unsigned<T>::value &&
        !debug_cond_floating_point<T>::value &&
        !debug_cond_is_ostream_ok<T>::value && !debug_cond_pointer<T>::value &&
        !debug_cond_is_range<T>::value && !debug_cond_is_tuple<T>::value &&
        !debug_cond_enum<T>::value && !debug_cond_is_member_repr<T>::value &&
        !debug_cond_is_member_repr_stream<T>::value &&
        !debug_cond_is_adl_repr<T>::value &&
        debug_cond_is_adl_repr_stream<T>::value>::type> {
    void operator()(std::ostream &oss, T const &t) const {
        DEBUG_REPR_NAME(oss, t);
    }
};

struct debug_visit_lambda {
    std::ostream &oss;

    template <class T>
    void operator()(T const &t) const {
        debug_format(oss, t);
    }
};

template <class T>
struct debug_format_trait<
    T,
    typename std::enable_if<
        !debug_cond_string<T>::value && !debug_cond_bool<T>::value &&
        !debug_cond_char<T>::value && !debug_cond_unicode_char<T>::value &&
        !debug_cond_integral_unsigned<T>::value &&
        !debug_cond_floating_point<T>::value &&
        !debug_cond_is_ostream_ok<T>::value && !debug_cond_pointer<T>::value &&
        !debug_cond_is_range<T>::value && !debug_cond_is_tuple<T>::value &&
        !debug_cond_enum<T>::value && !debug_cond_is_member_repr<T>::value &&
        !debug_cond_is_member_repr_stream<T>::value &&
        !debug_cond_is_adl_repr<T>::value &&
        !debug_cond_is_adl_repr_stream<T>::value &&
        debug_cond_is_variant<T>::value>::type> {
    void operator()(std::ostream &oss, T const &t) const {
        visit(debug_visit_lambda{oss}, t);
    }
};

template <class T>
struct debug_format_trait<
    T,
    typename std::enable_if<
        !debug_cond_string<T>::value && !debug_cond_bool<T>::value &&
        !debug_cond_char<T>::value && !debug_cond_unicode_char<T>::value &&
        !debug_cond_integral_unsigned<T>::value &&
        !debug_cond_floating_point<T>::value &&
        !debug_cond_is_ostream_ok<T>::value && !debug_cond_pointer<T>::value &&
        !debug_cond_is_range<T>::value && !debug_cond_is_tuple<T>::value &&
        !debug_cond_enum<T>::value && !debug_cond_is_member_repr<T>::value &&
        !debug_cond_is_member_repr_stream<T>::value &&
        !debug_cond_is_adl_repr<T>::value &&
        !debug_cond_is_adl_repr_stream<T>::value &&
        !debug_cond_is_variant<T>::value &&
        debug_cond_is_optional<T>::value>::type> {
    void operator()(std::ostream &oss, T const &t) const {
        if ((bool)t) {
            debug_format(oss, *t);
        } else {
            oss << DEBUG_SHOW_NULLOPT;
        }
    }
};

# endif

} // namespace debug_details

struct DEBUG_NODISCARD debug {
private:
    std::ostream &cout;
    std::ostringstream oss;

    enum {
        silent = 0,
        print = 1,
        panic = 2,
        supress = 3,
    } state;

    char const *line;
    DEBUG_SOURCE_LOCATION loc;

    debug &add_location_marks() {
        char const *fn = loc.file_name();
        for (char const *fp = fn; *fp; ++fp) {
            if (*fp == '/') {
                fn = fp + 1;
            }
        }
# if DEBUG_SHOW_SOURCE_LOCATION
        oss << fn << ':' << loc.line() << ':' << DEBUG_SEPARATOR_TAB;
# endif
        if (line) {
            oss << '[' << line << ']' << DEBUG_SEPARATOR_TAB;
# if DEBUG_LEVEL >= 2
        } else {
            static thread_local std::unordered_map<std::string, std::string>
                fileCache;
            auto key = std::to_string(loc.line()) + loc.file_name();
            if (auto it = fileCache.find(key);
                it != fileCache.end() && !it->second.empty()) [[likely]] {
                oss << '[' << it->second << ']';
            } else if (auto file = std::ifstream(loc.file_name());
                       file.is_open()) [[likely]] {
                std::string line;
                for (int i = 0; i < loc.line(); ++i) {
                    if (!std::getline(file, line))
                        DEBUG_UNLIKELY {
                            line.clear();
                            break;
                        }
                }
                if (auto pos = line.find_first_not_of(" \t\r\n");
                    pos != line.npos) [[likely]] {
                    line = line.substr(pos);
                }
                if (!line.empty()) [[likely]] {
                    if (line.back() == ';') [[likely]] {
                        line.pop_back();
                    }
                    oss << '[' << line << ']';
                }
                fileCache.try_emplace(key, std::move(line));
            } else {
                oss << '[' << '?' << ']';
                fileCache.try_emplace(key);
            }
# endif
        }
        oss << ' ';
        return *this;
    }

    template <class T>
    struct DEBUG_NODISCARD debug_condition {
    private:
        debug &d;
        T const &t;

        template <class U>
        debug &check(bool cond, U const &u, char const *sym) {
            if (!cond)
                DEBUG_UNLIKELY {
                    d.on_error("assertion failed:") << t << sym << u;
                }
            return d;
        }

    public:
        explicit debug_condition(debug &d, T const &t) noexcept : d(d), t(t) {}

        template <class U>
        debug &operator<(U const &u) {
            return check(t < u, u, "<");
        }

        template <class U>
        debug &operator>(U const &u) {
            return check(t > u, u, ">");
        }

        template <class U>
        debug &operator<=(U const &u) {
            return check(t <= u, u, "<=");
        }

        template <class U>
        debug &operator>=(U const &u) {
            return check(t >= u, u, ">=");
        }

        template <class U>
        debug &operator==(U const &u) {
            return check(t == u, u, "==");
        }

        template <class U>
        debug &operator!=(U const &u) {
            return check(t != u, u, "!=");
        }
    };

    debug &on_error(char const *msg) {
        if (state != supress) {
            state = panic;
            add_location_marks();
        } else {
            oss << ' ';
        }
        oss << msg;
        return *this;
    }

    template <class T>
    debug &on_print(T const &t) {
        if (state == supress)
            return *this;
        if (state == silent) {
            state = print;
            add_location_marks();
        } else {
            oss << ' ';
        }
        debug_details::debug_format(oss, t);
        return *this;
    }

# if DEBUG_ENABLE_FILES_MATCH
    static bool file_detected(char const *file) noexcept {
        static auto files = std::getenv("DEBUG_FILES");
        if (!files)
            return true;
        DEBUG_STRING_VIEW sv = files;
        /* std::size_t pos = 0, nextpos; */
        /* while ((nextpos = sv.find(' ')) != sv.npos) { */
        /*     if (sv.substr(pos, nextpos - pos) == tag) { */
        /*         return true; */
        /*     } */
        /*     pos = nextpos + 1; */
        /* } */
        if (sv.find(file) != sv.npos) {
            return true;
        }
        return false;
    }
# endif

public:
    explicit debug(bool enable = DEBUG_DEFAULT_ENABLED,
                   std::ostream &cout = DEBUG_DEFAULT_OUTPUT,
                   char const *line = nullptr,
                   DEBUG_SOURCE_LOCATION const &loc =
                       DEBUG_SOURCE_LOCATION::current()) noexcept
        : cout(cout),
          state(enable
# if DEBUG_ENABLE_FILES_MATCH
                        && file_detected(loc.file_name())
# endif
                    ? silent
                    : supress),
          line(line),
          loc(loc) {
    }

    debug(debug &&) = delete;
    debug(debug const &) = delete;

    template <class T>
    debug_condition<T> check(T const &t) noexcept {
        return debug_condition<T>{*this, t};
    }

    template <class T>
    debug_condition<T> operator>>(T const &t) noexcept {
        return debug_condition<T>{*this, t};
    }

    debug &fail(bool fail = true) {
        if (fail)
            DEBUG_UNLIKELY {
                on_error("failed:");
            }
        else {
            state = supress;
        }
        return *this;
    }

    debug &on(bool enable) {
        if (!enable) [[likely]] {
            state = supress;
        }
        return *this;
    }

    template <class T>
    debug &operator<<(T const &t) {
        return on_print(t);
    }

    template <class T>
    debug &operator,(T const &t) {
        return on_print(t);
    }

    ~debug()
# if DEBUG_PANIC_METHOD == 0
        noexcept(false)
# endif
    {
        if (state == panic)
            DEBUG_UNLIKELY {
# if DEBUG_PANIC_METHOD == 0
                throw std::runtime_error(oss.str());
# elif DEBUG_PANIC_METHOD == 1
                oss << '\n';
                cout << oss.str();
#  if defined(_MSC_VER)
                __debugbreak();
                return;
#  elif defined(__GNUC__) && defined(__has_builtin)
#   if __has_builtin(__builtin_trap)
                __builtin_trap();
                return;
#   else
                std::terminate();
#   endif
#  else
                std::terminate();
#  endif
# elif DEBUG_PANIC_METHOD == 2
                oss << '\n';
                cout << oss.str();
                std::terminate();
# else
                oss << '\n';
                cout << oss.str();
                return;
# endif
            }
        if (state == print) {
            oss << '\n';
            cout << oss.str();
        }
    }

    operator std::string() {
        std::string ret = oss.str();
        state = supress;
        return ret;
    }
};

DEBUG_NAMESPACE_END

#else

DEBUG_NAMESPACE_BEGIN

struct debug {
    debug(bool = true, char const * = nullptr) noexcept {}

    debug(debug &&) = delete;
    debug(debug const &) = delete;

    template <class T>
    debug &operator,(T const &) {
        return *this;
    }

    template <class T>
    debug &operator<<(T const &) {
        return *this;
    }

    debug &on(bool) {
        return *this;
    }

    debug &fail(bool = true) {
        return *this;
    }

    ~debug() noexcept(false) {}

private:
    struct debug_condition {
        debug &d;

        explicit debug_condition(debug &d) : d(d) {}

        template <class U>
        debug &operator<(U const &) {
            return d;
        }

        template <class U>
        debug &operator>(U const &) {
            return d;
        }

        template <class U>
        debug &operator<=(U const &) {
            return d;
        }

        template <class U>
        debug &operator>=(U const &) {
            return d;
        }

        template <class U>
        debug &operator==(U const &) {
            return d;
        }

        template <class U>
        debug &operator!=(U const &) {
            return d;
        }
    };

public:
    template <class T>
    debug_condition check(T const &) noexcept {
        return debug_condition{*this};
    }

    template <class T>
    debug_condition operator>>(T const &) noexcept {
        return debug_condition{*this};
    }

    operator std::string() {
        return {};
    }
};

DEBUG_NAMESPACE_END

#endif

#ifdef DEBUG_CLASS_NAME
# undef debug
#elif DEBUG_LEVEL
# ifdef DEBUG_SOURCE_LOCATION_FAKER
#  define debug()                                                 \
      debug(DEBUG_DEFAULT_ENABLED, DEBUG_DEFAULT_OUTPUT, nullptr, \
            DEBUG_SOURCE_LOCATION_FAKER)
# endif
#endif
