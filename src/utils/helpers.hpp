#include <chrono>
#include <functional>
#include <string>
#include <vector>

#ifdef DEBUG
#define log(...) printf(__VA_ARGS__)
#else
#define log(...)
#endif

void measureTime(const std::function<void(void)>& compute);
std::vector<std::string> splitOnSpaces(const std::string& s);
bool isMonotonicallyIncreasing(const std::vector<std::string>& svec);