#include <cassert>
#include <iostream>
#include "avl.hpp"

int main(void) {
  AVL* avl = new AVL();

  for (int i = 0; i <= 1000; i += 2) {
    avl->insert(i);
  }

  assert(avl->findKey(0) == 0);
  assert(avl->findKey(250) == 250);
  assert(avl->findKey(500) == 500);
  assert(avl->findKey(750) == 750);
  assert(avl->findKey(1000) == 1000);

  assert(avl->successor(0) == 2);
  assert(avl->successor(250) == 252);
  assert(avl->successor(500) == 502);
  assert(avl->successor(750) == 752);
  assert(avl->successor(1000) == NOT_FOUND);

  assert(avl->predecessor(0) == NOT_FOUND);
  assert(avl->predecessor(250) == 248);
  assert(avl->predecessor(500) == 498);
  assert(avl->predecessor(750) == 748);
  assert(avl->predecessor(1000) == 998);

  int key1 = avl->getRoot();
  int key2 = 1000;
  int key3 = 552;

  avl->remove(key1);
  assert(avl->findKey(key1) == NOT_FOUND);
  avl->remove(key2);
  assert(avl->findKey(key2) == NOT_FOUND);
  avl->remove(key3);
  assert(avl->findKey(key3) == NOT_FOUND);

  int key4 = avl->getRoot();
  avl->remove(key4);
  assert(avl->findKey(key4) == NOT_FOUND);

  avl->inorder([](int data) { printf("%d ", data); });
  printf("\n");

  printf("ALL TESTS PASSED\n");

  return 0;
}