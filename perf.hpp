#pragma once

#include <algorithm>
#include <chrono>
#include <csignal>
#include <cstdlib>
#include <deque>
#include <fstream>
#include <map>
#include <mutex>
#include <vector>
#if __cplusplus >= 202002L
# if defined(__has_include)
#  if __has_include(<source_location>)
#   include <source_location>
#  endif
# endif
#endif

struct Perf {
    struct Location {
        char const *func;
        char const *file;
        int line;

    private:
        struct Private {
            explicit Private() = default;
        };

    public:
#if __cpp_lib_source_location
        Location(std::source_location const &loc =
                     std::source_location::current()) noexcept
            : Location(loc.function_name(), loc.file_name(), loc.line()) {}

        Location(const char *func, std::source_location const &loc =
                     std::source_location::current()) noexcept
            : Location(func, loc.file_name(), loc.line()) {}

# define PERF Perf()
#else
# if defined(__has_builtin)
#  if __has_builtin(__builtin_FUNCTION) && __has_builtin(__builtin_FILE) && \
      __has_builtin(__builtin_LINE)
        Location(char const *func = __builtin_FUNCTION(),
                 Private = Private(),
                 char const *file = __builtin_FILE(),
                 int line = __builtin_LINE()) noexcept
            : Location(func, file, line) {}

#   define PERF(...) Perf()
#  else
        Location(const char *func = "?") noexcept : Location(func, "???", 0) {}

#   define PERF(...) Perf({__func__, __FILE__, __LINE__})
#  endif
# else
        Location(const char *func = "?") noexcept : Location(func, "???", 0) {}

#  define PERF(...) Perf({__func__, __FILE__, __LINE__})
# endif
#endif

        Location(char const *func, char const *file, int line) noexcept
            : func(func),
              file(file),
              line(line) {}

        bool operator<(Location const &rhs) const {
            return func < rhs.func ||
                   (func == rhs.func &&
                    (file < rhs.file || (file == rhs.file && line < rhs.line)));
        }
    };

private:
    Location location;
    std::uint64_t t0;

    struct PerfTableEntry {
        std::uint64_t t0;
        std::uint64_t t1;
        Location location;

        PerfTableEntry(std::uint64_t t0, std::uint64_t t1, Location location)
            : t0(t0),
              t1(t1),
              location(location) {}
    };

    struct PerfThreadLocal {
        std::deque<PerfTableEntry> table;

        PerfThreadLocal() = default;

        PerfThreadLocal(PerfThreadLocal &&) = delete;

        ~PerfThreadLocal() {
            gatherData(table);
        }
    };

#if __cpp_inline_variables
    static inline thread_local PerfThreadLocal perthread;
#else
    static PerfThreadLocal &perthread() {
        static thread_local PerfThreadLocal perthread;
        return perthread;
    }
#endif

    struct PerfGather {
        PerfGather() {
            output = std::getenv("PERF_OUTPUT");
            std::signal(
                SIGINT, +[](int signo) { std::exit(130); });
        }

        PerfGather(PerfGather &&) = delete;

        ~PerfGather() {
            if (output) {
                std::ofstream fout(output);
                for (auto const &entry: table) {
                    fout << entry.t0 << ',' << entry.t1 << ','
                         << entry.location.func << ',' << entry.location.file
                         << entry.location.line;
                }
            }

            struct ChartEntry {
                std::uint64_t min = std::numeric_limits<std::uint64_t>::max();
                std::uint64_t sum = 0;
                std::uint64_t max = 0;
                std::uint64_t nr = 0;

                void add(PerfTableEntry const &e) {
                    std::uint64_t d = e.t1 - e.t0;
                    min = std::min(min, d);
                    sum += d;
                    max = std::max(max, d);
                    ++nr;
                }
            };

            std::map<Location, ChartEntry> chart;
            for (auto it = table.begin(); it != table.end(); ++it) {
                chart[it->location].add(*it);
            }

            auto timeFormat = [](std::uint64_t d) -> std::string {
                if (d < 10000) {
                    return std::to_string(d) + "ns";
                } else if (d < 10000000) {
                    return std::to_string(d / 1000) + "us";
                } else if (d < 10000000000) {
                    return std::to_string(d / 1000000) + "ms";
                } else if (d < 10000000000000) {
                    return std::to_string(d / 1000000000) + "s";
                } else {
                    return std::to_string(d / 3600000000000) + "h";
                }
            };

            auto simplifyPath = [](std::string s) -> std::string {
                auto p = s.rfind('/');
                if (p == std::string::npos) {
                    return s;
                } else {
                    return s.substr(p + 1);
                }
            };

            auto simplifyFunc = [](std::string s) -> std::string {
                auto p = s.rfind('/');
                if (p == std::string::npos) {
                    return s;
                } else {
                    return s.substr(p + 1);
                }
            };

            struct ChartCompare {
                bool operator()(std::pair<Location, ChartEntry> const &a,
                                std::pair<Location, ChartEntry> const &b) {
                    return a.second.sum > b.second.sum;
                }
            };

            std::vector<std::pair<Location, ChartEntry>> sorted(chart.begin(),
                                                                chart.end());
            std::sort(sorted.begin(), sorted.end(), ChartCompare());

            std::size_t funcWidth = 0, pathWidth = 0, nrWidth = 1;
            for (auto it = sorted.begin(); it != sorted.end(); ++it) {
                funcWidth =
                    std::max(funcWidth, simplifyFunc(it->first.func).size());
                pathWidth =
                    std::max(pathWidth, simplifyPath(it->first.file).size());
                nrWidth =
                    std::max(nrWidth, std::to_string(it->second.nr).size());
            }

            std::printf("==== PERF REPORT ====\n");
            std::printf("%*s %*s:%-4s %6s %6s %6s %*s\n", (int)funcWidth,
                        "func", (int)pathWidth, "file", "line", "min", "avg",
                        "max", (int)nrWidth + 1, "nr");
            for (auto it = sorted.begin(); it != sorted.end(); ++it) {
                std::printf("%*s %*s:%-4d %6s %6s %6s %*lux\n", (int)funcWidth,
                            simplifyPath(it->first.func).c_str(),
                            (int)pathWidth,
                            simplifyPath(it->first.file).c_str(),
                            it->first.line, timeFormat(it->second.min).c_str(),
                            timeFormat(it->second.sum / it->second.nr).c_str(),
                            timeFormat(it->second.max).c_str(), (int)nrWidth,
                            it->second.nr);
            }
        }

        std::deque<PerfTableEntry> table;
        std::mutex lock;
        char const *output;
    };

#if __cpp_inline_variables
    static inline PerfGather gathered;

    static void gatherData(std::deque<PerfTableEntry> const &table) {
        std::lock_guard<std::mutex> guard(gathered.lock);
        gathered.table.insert(gathered.table.end(), table.begin(), table.end());
    }
#else
    static PerfGather &gathered() {
        static PerfGather gathered;
        return gathered;
    }

    static void gatherData(std::deque<PerfTableEntry> const &table) {
        PerfGather &g = gathered();
        std::lock_guard<std::mutex> guard(g.lock);
        g.table.insert(g.table.end(), table.begin(), table.end());
    }
#endif

public:
    Perf(Location location = Location())
        : location(location),
          t0(std::chrono::high_resolution_clock::now()
                 .time_since_epoch()
                 .count()) {}

    Perf(const char *func)
        : Perf(Location(func)) {}

    Perf(Perf &&) = delete;

    ~Perf() {
        auto t1 = std::chrono::high_resolution_clock::now()
                      .time_since_epoch()
                      .count();
#if __cpp_inline_variables
        perthread.table.emplace_back(t0, t1, location);
#else
        perthread().table.emplace_back(t0, t1, location);
#endif
    }
};
