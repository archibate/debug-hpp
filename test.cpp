#include "debug.hpp"
#include <vector>

int main() {
    std::vector<int> a{1, 2, 3, 4, 5};
    debug(), a;
    std::tuple<int, std::string> b{42, "answer"};
    debug(), b;
    std::vector<std::string> c{"hello", "world"};
    debug(), c;
    return 0;
}
