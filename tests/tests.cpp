#include <cassert>
#include <chrono>
#include <iostream>
#include <vector>

#include "../src/avl/avl.cpp"
#include "../src/utils/helpers.hpp"

const int NODE_COUNT = 100000;

auto main() -> int {
  auto* avl = new AVL<int, std::string>([](int a, int b) {
    if (a == b) {
      return 0;
    } else if (a < b) {
      return -1;
    } else {
      return 1;
    }
  });

  auto* avl2 = new AVL<int, std::string>([](int a, int b) {
    if (a == b) {
      return 0;
    } else if (a < b) {
      return -1;
    } else {
      return 1;
    }
  });

  // TODO: exec time entre `remove` y `fastRemove`
  // TODO: ¿por qué el iterative es más lento?

  // iterative insert benchmark
  measureTime("avl iterative insert", [&avl]() {
    for (int i = 0; i <= NODE_COUNT; i += 2) {
      avl->iterativeInsert(i, "bruh " + std::to_string(i));
    }
  });

  log("avl height with %d nodes: %d\n", NODE_COUNT, avl->getHeight());

  // recursive insert benchmark
  measureTime("avl recursive insert", [&avl2]() {
    for (int i = 0; i <= NODE_COUNT; i += 2) {
      avl2->insert(i, "bruh " + std::to_string(i));
    }
  });

  std::vector<int> svec = {510, 512, 514,  508, 506,  210, 1000,
                           0,   2,   1600, 4,   7898, 500, 516};

  // iterative find tests
  measureTime("avl iterative find", [&avl, &svec]() {
    for (auto s : svec) {
      auto foundKey = avl->iterativeFindKey(s);
      assert(foundKey.has_value() && foundKey.value() == s);
    }
  });

  // recursive find tests
  measureTime("avl recursive find", [&avl, &svec]() {
    for (auto s : svec) {
      auto foundKey = avl->findKey(s);
      assert(foundKey.has_value() && foundKey.value() == s);
    }
  });

  // predecessor tests
  for (auto s : svec) {
    auto res = avl->predecessor(s);
    assert(res.has_value() || s == 0);
    if (s != 0) {
      auto [key, value] = res.value();
      assert(key == s - 2);
      assert(value == "bruh " + std::to_string(s - 2));
    }
  }

  // successor tests
  for (auto s : svec) {
    auto res = avl->successor(s);
    assert(res.has_value());
    auto [key, value] = res.value();
    assert(key == s + 2);
    assert(value == "bruh " + std::to_string(s + 2));
  }

  // max tests
  {
    auto res = avl->maximum();
    auto [key, value] = res.value();
    assert(key == 100000);
    assert(value == "bruh 100000");
  }

  // min tests
  {
    auto res = avl->minimum();
    auto [key, value] = res.value();
    assert(key == 0);
    assert(value == "bruh 0");
  }

  // remove tests
  for (auto s : svec) {
    assert(avl->find(s).value() == "bruh " + std::to_string(s));
    avl->remove(s);
    assert(avl->findKey(s).has_value() == false);
    std::string avlStr = avl->inorderString();
    assert(isMonotonicallyIncreasing(splitOnSpaces(avlStr)) == 1);
  }

  delete avl;
  delete avl2;

  log("\033[32mAll tests passed!\033[0m\n");

  return 0;
}