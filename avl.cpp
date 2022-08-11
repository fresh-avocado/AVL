#include "avl.hpp"

#define AVL_GREATER 1
#define AVL_LESS -1
#define AVL_EQUAL 0

AVL::AVL(const std::function<int(int, int)>& comparator)
    : root{nullptr}, comparator{comparator} {}

int AVL::getHeight() const {
  return getHeight(root);
}

int AVL::getRoot() const {
  return root->data;
}

void AVL::insert(int data) {
  if (root == nullptr) {
    root = new Node(data);
  } else {
    insertRecursive(root, data);
  }
}

void AVL::iterativeInsert(int data) {
  if (root == nullptr) {
    root = new Node(data);
  } else {
    Node* parent = nullptr;
    Node* current = root;
    while (current) {
      parent = current;
      int comp = comparator(data, current->data);
      if (comp == AVL_GREATER) {
        current = current->right;
      } else if (comp == AVL_LESS) {
        current = current->left;
      } else {
        throw "duplicate key";
      }
    }
    if (comparator(data, parent->data) == AVL_GREATER) {
      parent->right = new Node(data);
      parent->right->parent = parent;
    } else {
      parent->left = new Node(data);
      parent->left->parent = parent;
    }
    fixup(parent);
  }
}

int AVL::maximum() const {
  return maximumNode(root)->data;
}

int AVL::minimum() const {
  return minimumNode(root)->data;
}

void AVL::inorder(const std::function<void(int)>& process) const {
  inorderTraversal(root, process);
}

std::string AVL::inorderString() const {
  std::string str = "";
  inorderTraversal(root,
                   [&str](int data) { str += std::to_string(data) + " "; });
  return str;
}

int AVL::getMaxWeightPathBetweenLeafs() const {
  return maxSumPath(root);
}

void AVL::remove(int key) {
  Node* nodeToRemove = findNode(key, root);
  if (nodeToRemove == nullptr)
    return;
  Node* leftChild = nodeToRemove->left;
  Node* rightChild = nodeToRemove->right;
  if (leftChild == nullptr && rightChild == nullptr) {
    log("no child\n");
    // node has no children
    if (nodeToRemove->parent == nullptr) {
      root = nullptr;
    } else {
      if (comparator(nodeToRemove->data, nodeToRemove->parent->data) ==
          AVL_GREATER) {
        nodeToRemove->parent->right = nullptr;
      } else {
        nodeToRemove->parent->left = nullptr;
      }
      fixup(nodeToRemove->parent);
    }
    delete nodeToRemove;
  } else if (leftChild == nullptr || rightChild == nullptr) {
    log("one child\n");
    // node has one child
    Node* child = leftChild ? leftChild : rightChild;
    if (comparator(nodeToRemove->data, nodeToRemove->parent->data) ==
        AVL_GREATER) {
      nodeToRemove->parent->right = child;
    } else {
      nodeToRemove->parent->left = child;
    }
    child->parent = nodeToRemove->parent;
    fixup(nodeToRemove->parent);
    delete nodeToRemove;
  } else {
    // node has two children
    Node* replacement = nodeToRemove->hl < nodeToRemove->hr
                            ? maximumNode(leftChild)
                            : minimumNode(rightChild);

    log("node to delete: %d\n", nodeToRemove->data);
    log("node to delete left: %d\n",
        nodeToRemove->left ? nodeToRemove->left->data : -1);
    log("node to delete right: %d\n",
        nodeToRemove->right ? nodeToRemove->right->data : -1);
    log("node to delete parent: %d\n",
        nodeToRemove->parent ? nodeToRemove->parent->data : -1);
    log("replacement: %d\n", replacement->data);
    log("replacement parent: %d\n",
        replacement->parent ? replacement->parent->data : -1);
    log("replacement parent left: %d\n",
        replacement->parent->left ? replacement->parent->left->data : -1);
    log("replacement parent right: %d\n",
        replacement->parent->right ? replacement->parent->right->data : -1);
    if (comparator(replacement->data, replacement->parent->data) ==
        AVL_GREATER) {
      if (replacement->left) {
        replacement->parent->right = replacement->left;
        replacement->left->parent = replacement->parent;
      } else {
        replacement->parent->right = nullptr;
      }
    } else {
      if (replacement->right) {
        replacement->parent->left = replacement->right;
        replacement->right->parent = replacement->parent;
      } else {
        replacement->parent->left = nullptr;
      }
    }
    nodeToRemove->data = replacement->data;
    fixup(replacement->parent);
    delete replacement;
  }
}

int AVL::predecessor(int key) const {
  Node* foundNode = findNode(key, root);
  if (foundNode) {
    if (foundNode->left) {
      return maximumNode(foundNode->left)->data;
    } else {
      Node* predecessor = predecessorUp(foundNode);
      return predecessor != nullptr ? predecessor->data : NOT_FOUND;
    }
  } else {
    return NOT_FOUND;
  }
}

int AVL::successor(int key) const {
  Node* foundNode = findNode(key, root);  // O(lg n)
  if (foundNode) {
    if (foundNode->right) {
      return minimumNode(foundNode->right)->data;  // O(lg n)
    } else {
      Node* successor = successorUp(foundNode);  // O(lg n)
      return successor != nullptr ? successor->data : NOT_FOUND;
    }
  } else {
    return NOT_FOUND;
  }
}

int AVL::findKey(int key) const {
  Node* foundNode = findNode(key, root);
  if (foundNode) {
    return foundNode->data;
  } else {
    return NOT_FOUND;
  }
}

int AVL::iterativeFindKey(int key) const {
  Node* current = root;
  while (current) {
    int comp = comparator(key, current->data);
    if (comp == AVL_EQUAL) {
      return key;
    }
    if (comp == AVL_GREATER)
      current = current->right;
    else
      current = current->left;
  }
  return NOT_FOUND;
}

void AVL::fixup(Node* _root) {
  while (_root != nullptr) {
    Node* nextParent = _root->parent;
    _root->hl = getHeight(_root->left);
    _root->hr = getHeight(_root->right);
    int balanceFactor = _root->hl - _root->hr;

    if (balanceFactor < -1) {
      // right heavy
      int balanceFactorRight = _root->right->hl - _root->right->hr;
      if (balanceFactorRight >= 0) {
        // RL rotation
        rightRotation(_root->right, _root->right->left);
        leftRotation(_root, _root->right);
      } else {
        // L rotation
        leftRotation(_root, _root->right);
      }
    } else if (balanceFactor > 1) {
      // left heavy
      int balanceFactorLeft = _root->left->hl - _root->left->hr;
      if (balanceFactorLeft <= 0) {
        // LR rotation
        leftRotation(_root->left, _root->left->right);
        rightRotation(_root, _root->left);
      } else {
        // R rotation
        rightRotation(_root, _root->left);
      }
    }
    _root = nextParent;
  }
}

Node* AVL::minimumNode(Node* _root) const {
  while (_root->left != nullptr) {
    _root = _root->left;
  }
  return _root;
}

Node* AVL::maximumNode(Node* _root) const {
  while (_root->right != nullptr) {
    _root = _root->right;
  }
  return _root;
}

Node* AVL::successorUp(Node* _root) const {
  Node* y = _root->parent;
  while (y != nullptr && _root == y->right) {
    _root = y;
    y = y->parent;
  }
  return y;
}

Node* AVL::predecessorUp(Node* _root) const {
  Node* y = _root->parent;
  while (y != nullptr && _root == y->left) {
    _root = y;
    y = y->parent;
  }
  return y;
}

Node* AVL::findNode(int key, Node* _root) const {
  if (_root == nullptr) {
    return nullptr;
  }
  int comp = comparator(key, _root->data);
  if (comp == AVL_GREATER) {
    return findNode(key, _root->right);
  } else if (comp == AVL_LESS) {
    return findNode(key, _root->left);
  } else {
    return _root;
  }
}

int AVL::maxSumPath(Node* _root) const {
  AVL::response res = maxSumPathRecursive(_root);
  return std::max({res.maxSumPathForThisNode, res.maxSumPathToLeave,
                   res.maxMaxSumPathOfSubtrees});
}

// TODO cual es el key de T
AVL::response AVL::maxSumPathRecursive(Node* _root) const {
  if (_root == nullptr) {
    response res = {0, 0, 0};
    return res;
  }
  response l = maxSumPathRecursive(_root->left);
  response r = maxSumPathRecursive(_root->right);

  response res;
  res.maxSumPathForThisNode =
      l.maxSumPathToLeave + r.maxSumPathToLeave + _root->data;
  res.maxSumPathToLeave =
      std::max({l.maxSumPathToLeave, r.maxSumPathToLeave}) + _root->data;
  res.maxMaxSumPathOfSubtrees =
      std::max({l.maxSumPathForThisNode, r.maxSumPathForThisNode});
  return res;
}

void AVL::inorderTraversal(Node* _root,
                           const std::function<void(int)>& process) const {
  if (_root == nullptr) {
    return;
  }
  inorderTraversal(_root->left, process);
  process(_root->data);
  inorderTraversal(_root->right, process);
}

void AVL::leftRotation(Node* x, Node* y) {
  if (y->left) {
    y->left->parent = x;
  }
  x->right = y->left;
  if (x->parent == nullptr) {
    root = y;
  } else if (comparator(x->data, x->parent->data) == AVL_LESS) {
    x->parent->left = y;
  } else {
    x->parent->right = y;
  }
  y->left = x;
  y->parent = x->parent;
  x->parent = y;

  x->hr = getHeight(x->right);
  y->hl = getHeight(y->left);
}

void AVL::rightRotation(Node* x, Node* y) {
  if (y->right) {
    y->right->parent = x;
  }
  x->left = y->right;
  if (x->parent == nullptr) {
    root = y;
  } else if (comparator(x->data, x->parent->data) == AVL_GREATER) {
    x->parent->right = y;
  } else {
    x->parent->left = y;
  }
  y->right = x;
  y->parent = x->parent;
  x->parent = y;

  x->hl = getHeight(x->left);
  y->hr = getHeight(y->right);
}

void AVL::insertRecursive(Node* current, int data) {
  int comp = comparator(data, current->data);
  if (comp == AVL_GREATER) {
    if (current->right == nullptr) {
      current->right = new Node(data);
      current->right->parent = current;
      current->hr = getHeight(current->right);
    } else {
      insertRecursive(current->right, data);
      current->hr = getHeight(current->right);
      int balanceFactor = current->hl - current->hr;
      if (balanceFactor < -1) {
        int balanceFactorRight = current->right->hl - current->right->hr;
        if (balanceFactorRight >= 0) {
          // RL rotation
          rightRotation(current->right, current->right->left);
          leftRotation(current, current->right);
        } else {
          // L rotation
          leftRotation(current, current->right);
        }
      }
    }
  } else if (comp == AVL_LESS) {
    if (current->left == nullptr) {
      current->left = new Node(data);
      current->left->parent = current;
      current->hl = getHeight(current->left);
    } else {
      insertRecursive(current->left, data);
      current->hl = getHeight(current->left);
      int balanceFactor = current->hl - current->hr;
      if (balanceFactor > 1) {
        // left heavy
        int balanceFactorLeft = current->left->hl - current->left->hr;
        if (balanceFactorLeft <= 0) {
          // LR rotation
          leftRotation(current->left, current->left->right);
          rightRotation(current, current->left);
        } else {
          // R rotation
          rightRotation(current, current->left);
        }
      }
    }
  } else {
    throw "duplicated key";
  }
}

inline int AVL::getHeight(Node* node) const {
  if (node == nullptr) {
    return 0;
  } else {
    return std::max({node->hl, node->hr}) + 1;
  }
}

void AVL::clear(Node* _root) {
  if (_root == nullptr) {
    return;
  } else {
    clear(_root->left);
    clear(_root->right);
    delete _root;
  }
}

AVL::~AVL() noexcept {
  clear(root);
}
