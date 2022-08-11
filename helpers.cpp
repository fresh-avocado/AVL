#include "helpers.hpp"

void measureTime(const std::function<void(void)>& compute) {
  std::chrono::high_resolution_clock::time_point start =
      std::chrono::high_resolution_clock::now();
  compute();
  std::chrono::high_resolution_clock::time_point end =
      std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> execTime =
      std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
  log("time taken: %lf seconds\n", execTime.count());
}

std::vector<std::string> splitOnSpaces(const std::string& s) {
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

bool isMonotonicallyIncreasing(const std::vector<std::string>& svec) {
  for (size_t i = 0; i < svec.size() - 1; ++i) {
    if (atoi(svec[i].c_str()) < atoi(svec[i + 1].c_str()))
      continue;
    else
      return false;
  }
  return true;
}