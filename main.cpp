#include <cassert>
#include <chrono>
#include <iostream>
#include <vector>

#include "avl.hpp"

#define NODE_COUNT 100000

int main(void) {
  AVL* avl = new AVL([](int a, int b) {
    if (a == b)
      return 0;
    if (a < b)
      return -1;
    else
      return 1;
  });

  AVL* avl2 = new AVL([](int a, int b) {
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
      avl->iterativeInsert(i);
    }
  });

  log("avl height with %d nodes: %d\n", NODE_COUNT, avl->getHeight());

  measureTime([&avl2]() {
    for (int i = 0; i <= NODE_COUNT; i += 2) {
      avl2->insert(i);
    }
  });

  std::vector<int> svec = {510, 512, 514,  508, 506,  210, 1000,
                           0,   2,   1600, 4,   7898, 500, 516};

  measureTime([&avl, &svec]() {
    for (auto s : svec) {
      assert(avl->iterativeFindKey(s) == s);
    }
  });

  measureTime([&avl, &svec]() {
    for (auto s : svec) {
      assert(avl->findKey(s) == s);
    }
  });

  for (auto s : svec) {
    avl->remove(s);
    assert(avl->findKey(s) == NOT_FOUND);
    std::string avlStr = avl->inorderString();
    assert(isMonotonicallyIncreasing(splitOnSpaces(avlStr)) == 1);
  }

  delete avl;
  delete avl2;

  return 0;
}