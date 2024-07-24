#define DEBUG_SHOW_THREAD_ID 1
#include "debug.hpp"
#include <map>
#include <set>
#include <chrono>
#include <memory>
#if __cplusplus >= 201703L
# ifdef __has_include
#  if __has_include(<variant>)
#   include <variant>
#  endif
#  if __has_include(<optional>)
#   include <optional>
#  endif
#  if __cplusplus >= 202002L
#   if __has_include(<span>)
#    include <span>
#   endif
#  endif
# endif
#endif
#include <string>
#include <vector>
#include <functional>

struct Student {
    std::string name;
    int age;

    DEBUG_REPR(name, age);
};

struct Student2 {
    std::string name;
    int age;
};

DEBUG_REPR_GLOBAL(Student2, name, age);

template <class Name, class Age>
struct Student3 {
    Name name;
    Age age;
};

DEBUG_REPR_GLOBAL_TEMPLATED(Student3, (Name, Age), (class Name, class Age), name, age);

struct Student4 {
#if DEBUG_PP_VA_OPT_SUPPORT(?)
    DEBUG_REPR();
#endif
};

int main() {
    debug(), -1;
    int i = 42;
    debug(), "the answer:", i;
    std::vector<int> a{1, 2, 3, 4, 5};
    debug(), a;
    std::tuple<int, std::string> b{42, "answer"};
    debug(), b;
    std::vector<std::string> c{"hello", "world"};
    debug(), c;
    std::map<std::string, int> d{{"hello", 1}, {"world", 2}};
    debug(), d;
    std::set<int> e{1, 2, 3, 4, 5};
    debug(), e;
    std::string f = "Hello, world!";
    debug(), f;
    std::unique_ptr<int> g(new int(42));
    debug(), g;
    std::unique_ptr<int> h = nullptr;
    debug(), h;
    std::shared_ptr<int> j(new int(42));
#if __cplusplus >= 201703L
# ifdef __cpp_lib_optional
    debug(), j;
    std::optional<int> k = 42;
    debug(), k;
    std::optional<int> l = std::nullopt;
    debug(), l;
# endif
# ifdef __cpp_lib_variant
    std::variant<int, std::string> m = "hello";
    debug(), m;
# endif
# ifdef __cpp_lib_string_view
    std::string_view n = "hello";
    debug(), n;
# endif
#endif
    struct Test {};

    Test o;
    debug(), o;
    std::function<void()> p = [] {
    };
    debug(), p;
    std::u32string q = U"hello";
    debug(), q;
    std::vector<std::uint8_t> r = {1, 2, 3};
    debug(), r;
    std::vector<std::uint16_t> s = {1, 2, 3};
    debug(), s;
    std::uint8_t t = 42;
    debug(), t;
    char u = 42;
    debug(), u;
    char16_t v = 42;
    debug(), v;
#if __cplusplus >= 202002L
# ifdef __cpp_char8_t
    char8_t w = 42;
    debug(), w;
# endif
#endif
    wchar_t x = L'*';
    debug(), x;
    std::wstring y = L"hello";
    debug(), y;
    std::vector<char> z = {'h', 'e', 'l', 'l', 'o'};
    debug(), z;
#if __cplusplus >= 202002L
# ifdef __cpp_lib_span
    std::span<char> z1 = z;
    debug(), z1;
# endif
#endif
    std::chrono::nanoseconds z2(42);
    debug(), z2;
    auto z3 = std::chrono::system_clock::now();
    debug(), z3;

    struct Baby {
        std::string name;
        int age;

        auto repr() const -> decltype(std::make_tuple(name, age)) {
            return std::make_tuple(name, age);
        }
    } z4{"peng", 42};

    debug(), z4;
    auto z5 = static_cast<std::string>(debug(), "hello", 42);
    std::cerr << z5 << '\n';
    float z6 = 3.14f;
    debug(), z6;
    debug().check(z6) > 3;

    Student z7{"peng", 42};
    debug(), z7;

    Student2 z8{"peng", 42};
    debug(), z8;

    Student3<std::string, int> z9{"peng", 42};
    debug(), z9;

    debug::debug_formatter{std::cout} << z9 << " got answer " << 42 << "\n";

    Student4 z10;
    debug(), z10;

    std::errc z11 = std::errc::host_unreachable;
    debug(), z11;

    std::errc z12 = std::errc();
    debug(), z12;

    std::error_code z13((int)std::io_errc::stream, std::iostream_category());
    debug(), z13;

    std::error_condition z14((int)std::io_errc::stream, std::iostream_category());
    debug(), z14;

    return 0;
}

/*
g++ -std=c++11 test.cpp -I .
g++ -std=c++14 test.cpp -I .
g++ -std=c++17 test.cpp -I .
g++ -std=c++20 test.cpp -I .
cat debug.hpp test.cpp | sed '/\#pragma once\|\#include "debug\.hpp"/d' | xsel -ib
scp debug.hpp root@142857.red:/root/142857.red
*/
