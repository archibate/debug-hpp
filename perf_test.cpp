#define TINYBENCH_IMPL
#include "tinybench.hpp"

struct A {
};

BENCHMARK(BM_virtual) {
    for (auto _: h) {
        h.arg(0);
    }
}

int main() {
}
