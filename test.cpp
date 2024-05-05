#include "debug.hpp"
#include <map>
#include <chrono>
#include <memory>
#include <variant>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <vector>
#include <functional>

int main() {
    int i = 42;
    debug(), i;
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
#if __cpp_lib_optional
    debug(), j;
    std::optional<int> k = 42;
    debug(), k;
    std::optional<int> l = std::nullopt;
    debug(), l;
#endif
#if __cpp_lib_variant
    std::variant<int, std::string> m = "hello";
    debug(), m;
#endif
#if __cpp_lib_string_view
    std::string_view n = "hello";
    debug(), n;
#endif
    struct Test {};
    Test o;
    debug(), o;
    std::function<void()> p = [] {};
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
#if __cpp_char8_t
    char8_t w = 42;
    debug(), w;
#endif
    wchar_t x = L'*';
    debug(), x;
    std::wstring y = L"hello";
    debug(), y;
    std::vector<char> z = {'h', 'e', 'l', 'l', 'o'};
    debug(), z;
#if __cpp_lib_span
    std::span<char> z1 = z;
    debug(), z1;
#endif
    std::chrono::nanoseconds z2(42);
    debug(), z2;
    auto z3 = std::chrono::system_clock::now();
    debug(), z3;
    return 0;
}

// g++ -std=c++11 test.cpp -I .
// g++ -std=c++14 test.cpp -I .
// g++ -std=c++17 test.cpp -I .
// g++ -std=c++20 test.cpp -I .
