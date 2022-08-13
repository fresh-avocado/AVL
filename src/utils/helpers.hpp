#include <chrono>
#include <functional>
#include <string>
#include <vector>

#ifdef DEBUG
#define log(...) printf(__VA_ARGS__)
#else
#define log(...)
#endif

auto measureTime(const char* benchmarkName,
                 const std::function<void(void)>& compute) -> void;
auto splitOnSpaces(const std::string& s) -> std::vector<std::string>;
auto isMonotonicallyIncreasing(const std::vector<std::string>& svec) -> bool;