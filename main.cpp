#include <iostream>
#include "avl.hpp"

int main(void) {
  AVL* avl = new AVL();

  for (int i = 1; i <= 1000; i++) {
    avl->insert(i);
  }

  avl->inorder([](int data) { printf("%d ", data); });

  printf("\nmaxSumPath: %d\n", avl->getMaxWeightPathBetweenLeafs());

  printf("findKey(%d): %d\n", 57, avl->findKey(57));

  printf("successor(%d): %d\n", 3, avl->successor(3));
  printf("successor(%d): %d\n", 10, avl->successor(10));
  printf("successor(%d): %d\n", 4, avl->successor(4));

  delete avl;

  return 0;
}