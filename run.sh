# g++ -std=c++11 test.cpp -I . -o /tmp/a.out && /tmp/a.out
g++ -std=c++20 -DNDEBUG -O3 perf_test.cpp -I . -o /tmp/a.out && /tmp/a.out
