#include "perf.hpp"

[[gnu::noinline]] void noinline_func() {
}

inline void inline_func() {
}

int main() {
    for (int i = 0; i < 1000; ++i) {
        Perf _ = "noinline_func";
        noinline_func();
    }
    for (int i = 0; i < 1000; ++i) {
        Perf _ = "inline_func";
        inline_func();
    }
}
