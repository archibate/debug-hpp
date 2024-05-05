#include "debug.hpp"
#include <map>
#include <memory>
#include <variant>
#include <optional>
#include <set>
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
    return 0;
}

// g++ -std=c++11 test.cpp -I .
