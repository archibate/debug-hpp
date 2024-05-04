#include "debug.hpp"
#include <map>
#include <set>
#include <string>
#include <vector>

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
    return 0;
}
