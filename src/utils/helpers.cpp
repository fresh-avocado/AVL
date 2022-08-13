#include "helpers.hpp"

auto measureTime(const char* benchmarkName,
                 const std::function<void(void)>& compute) -> void {
  // NOLINTNEXTLINE
  std::chrono::high_resolution_clock::time_point start =
      std::chrono::high_resolution_clock::now();
  compute();
  // NOLINTNEXTLINE
  std::chrono::high_resolution_clock::time_point end =
      std::chrono::high_resolution_clock::now();

  log("%s took: %lf seconds\n", benchmarkName,
      std::chrono::duration_cast<std::chrono::duration<double>>(end - start)
          .count());
}

auto splitOnSpaces(const std::string& s) -> std::vector<std::string> {
  std::vector<std::string> svec;
  std::string word;
  size_t s_size = s.length();
  for (size_t i = 0; i < s_size; i++) {
    if (s[i] == ' ') {
      if (!word.empty()) {
        svec.emplace_back(word);
        word.clear();
      }
      continue;
    }
    word += s[i];
  }
  if (!word.empty()) {
    svec.emplace_back(word);
  }
  return svec;
}

auto isMonotonicallyIncreasing(const std::vector<std::string>& svec) -> bool {
  for (size_t i = 0; i < svec.size() - 1; ++i) {
    if (atoi(svec[i].c_str()) < atoi(svec[i + 1].c_str())) {
      continue;
    }
    return false;
  }
  return true;
}