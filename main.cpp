#include <cassert>
#include <chrono>
#include <iostream>
#include <vector>

#include "avl.cpp"

#define NODE_COUNT 100000

int main(void) {
  AVL<int, std::string>* avl = new AVL<int, std::string>([](int a, int b) {
    if (a == b)
      return 0;
    if (a < b)
      return -1;
    else
      return 1;
  });

  AVL<int, std::string>* avl2 = new AVL<int, std::string>([](int a, int b) {
    if (a == b)
      return 0;
    if (a < b)
      return -1;
    else
      return 1;
  });

  // TODO: exec time entre `remove` y `fastRemove`
  // TODO: ¿por qué el iterative es más lento?
  measureTime([&avl]() {
    for (int i = 0; i <= NODE_COUNT; i += 2) {
      avl->iterativeInsert(i, "bruh " + std::to_string(i));
    }
  });

  log("avl height with %d nodes: %d\n", NODE_COUNT, avl->getHeight());

  measureTime([&avl2]() {
    for (int i = 0; i <= NODE_COUNT; i += 2) {
      avl2->insert(i, "bruh " + std::to_string(i));
    }
  });

  std::vector<int> svec = {510, 512, 514,  508, 506,  210, 1000,
                           0,   2,   1600, 4,   7898, 500, 516};

  measureTime([&avl, &svec]() {
    for (auto s : svec) {
      auto* foundKey = avl->iterativeFindKey(s);
      assert(foundKey && *foundKey == s);
    }
  });

  measureTime([&avl, &svec]() {
    for (auto s : svec) {
      auto* foundKey = avl->findKey(s);
      assert(foundKey && *foundKey == s);
    }
  });

  for (auto s : svec) {
    avl->remove(s);
    assert(avl->findKey(s) == nullptr);
    std::string avlStr = avl->inorderString();
    assert(isMonotonicallyIncreasing(splitOnSpaces(avlStr)) == 1);
  }

  delete avl;
  delete avl2;

  return 0;
}