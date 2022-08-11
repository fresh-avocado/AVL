#include "avl.hpp"

#define AVL_GREATER 1
#define AVL_LESS -1
#define AVL_EQUAL 0

template <typename KeyType, typename ValueType>
AVL<KeyType, ValueType>::AVL(
    const std::function<int(const KeyType&, const KeyType&)>& comparator)
    : root{nullptr}, comparator{comparator} {}

template <typename KeyType, typename ValueType>
int AVL<KeyType, ValueType>::getHeight() const {
  return getHeight(root);
}

template <typename KeyType, typename ValueType>
int AVL<KeyType, ValueType>::getRoot() const {
  return root->key;
}

template <typename KeyType, typename ValueType>
void AVL<KeyType, ValueType>::insert(const KeyType& key) {
  if (root == nullptr) {
    root = new Node<KeyType, ValueType>(key);
  } else {
    insertRecursive(root, key);
  }
}

template <typename KeyType, typename ValueType>
void AVL<KeyType, ValueType>::iterativeInsert(const KeyType& key) {
  if (root == nullptr) {
    root = new Node<KeyType, ValueType>(key);
  } else {
    Node<KeyType, ValueType>* parent = nullptr;
    Node<KeyType, ValueType>* current = root;
    while (current) {
      parent = current;
      int comp = comparator(key, current->key);
      if (comp == AVL_GREATER) {
        current = current->right;
      } else if (comp == AVL_LESS) {
        current = current->left;
      } else {
        throw "duplicate key";
      }
    }
    if (comparator(key, parent->key) == AVL_GREATER) {
      parent->right = new Node<KeyType, ValueType>(key);
      parent->right->parent = parent;
    } else {
      parent->left = new Node<KeyType, ValueType>(key);
      parent->left->parent = parent;
    }
    fixup(parent);
  }
}

template <typename KeyType, typename ValueType>
std::tuple<KeyType, ValueType> AVL<KeyType, ValueType>::maximum() const {
  Node<KeyType, ValueType> node = maximumNode(root);
  if (node) {
    return {node->key, node->value};
  } else {
    return {nullptr, nullptr};
  }
}

template <typename KeyType, typename ValueType>
std::tuple<KeyType, ValueType> AVL<KeyType, ValueType>::minimum() const {
  Node<KeyType, ValueType> node = minimumNode(root);
  if (node) {
    return {node->key, node->value};
  } else {
    return {nullptr, nullptr};
  }
}

template <typename KeyType, typename ValueType>
void AVL<KeyType, ValueType>::inorder(
    const std::function<void(const KeyType&, const ValueType&)>& process)
    const {
  inorderTraversal(root, process);
}

template <typename KeyType, typename ValueType>
std::string AVL<KeyType, ValueType>::inorderString() const {
  std::string str = "";
  inorderTraversal(
      root, [&str](const KeyType& key) { str += std::to_string(key) + " "; });
  return str;
}

// template <typename KeyType, typename ValueType>
// int AVL<KeyType, ValueType>::getMaxWeightPathBetweenLeafs() const {
//   return maxSumPath(root);
// }

template <typename KeyType, typename ValueType>
void AVL<KeyType, ValueType>::remove(const KeyType& key) {
  Node<KeyType, ValueType>* nodeToRemove = findNode(key, root);
  if (nodeToRemove == nullptr)
    return;
  Node<KeyType, ValueType>* leftChild = nodeToRemove->left;
  Node<KeyType, ValueType>* rightChild = nodeToRemove->right;
  if (leftChild == nullptr && rightChild == nullptr) {
    log("no child\n");
    // node has no children
    if (nodeToRemove->parent == nullptr) {
      root = nullptr;
    } else {
      if (comparator(nodeToRemove->key, nodeToRemove->parent->key) ==
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
    Node<KeyType, ValueType>* child = leftChild ? leftChild : rightChild;
    if (comparator(nodeToRemove->key, nodeToRemove->parent->key) ==
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
    Node<KeyType, ValueType>* replacement = nodeToRemove->hl < nodeToRemove->hr
                                                ? maximumNode(leftChild)
                                                : minimumNode(rightChild);

    log("node to delete: %d\n", nodeToRemove->key);
    log("node to delete left: %d\n",
        nodeToRemove->left ? nodeToRemove->left->key : -1);
    log("node to delete right: %d\n",
        nodeToRemove->right ? nodeToRemove->right->key : -1);
    log("node to delete parent: %d\n",
        nodeToRemove->parent ? nodeToRemove->parent->key : -1);
    log("replacement: %d\n", replacement->key);
    log("replacement parent: %d\n",
        replacement->parent ? replacement->parent->key : -1);
    log("replacement parent left: %d\n",
        replacement->parent->left ? replacement->parent->left->key : -1);
    log("replacement parent right: %d\n",
        replacement->parent->right ? replacement->parent->right->key : -1);
    if (comparator(replacement->key, replacement->parent->key) == AVL_GREATER) {
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
    nodeToRemove->key = replacement->key;
    fixup(replacement->parent);
    delete replacement;
  }
}

template <typename KeyType, typename ValueType>
std::tuple<const KeyType*, const ValueType*>
AVL<KeyType, ValueType>::predecessor(const KeyType& key) const {
  Node<KeyType, ValueType>* foundNode = findNode(key, root);
  if (foundNode) {
    if (foundNode->left) {
      return maximumNode(foundNode->left)->key;
    } else {
      Node<KeyType, ValueType>* predecessor = predecessorUp(foundNode);
      return {predecessor != nullptr ? &predecessor->key : nullptr,
              predecessor != nullptr ? &predecessor->value : nullptr};
    }
  } else {
    return {nullptr, nullptr};
  }
}

template <typename KeyType, typename ValueType>
std::tuple<const KeyType*, const ValueType*> AVL<KeyType, ValueType>::successor(
    const KeyType& key) const {
  Node<KeyType, ValueType>* foundNode = findNode(key, root);  // O(lg n)
  if (foundNode) {
    if (foundNode->right) {
      return minimumNode(foundNode->right)->key;  // O(lg n)
    } else {
      Node<KeyType, ValueType>* successor = successorUp(foundNode);  // O(lg n)
      return {successor != nullptr ? &successor->key : nullptr,
              successor != nullptr ? &successor->value : nullptr};
    }
  } else {
    return {nullptr, nullptr};
  }
}

template <typename KeyType, typename ValueType>
const KeyType* AVL<KeyType, ValueType>::findKey(const KeyType& key) const {
  Node<KeyType, ValueType>* foundNode = findNode(key, root);
  if (foundNode) {
    return &foundNode->key;
  } else {
    return NOT_FOUND;
  }
}

template <typename KeyType, typename ValueType>
const ValueType* AVL<KeyType, ValueType>::find(const KeyType& key) const {
  Node<KeyType, ValueType>* foundNode = findNode(key, root);
  if (foundNode) {
    return &foundNode->value;
  } else {
    return nullptr;
  }
}

template <typename KeyType, typename ValueType>
const KeyType* AVL<KeyType, ValueType>::iterativeFindKey(
    const KeyType& key) const {
  Node<KeyType, ValueType>* current = root;
  while (current) {
    int comp = comparator(key, current->key);
    if (comp == AVL_EQUAL) {
      return &current->key;
    }
    if (comp == AVL_GREATER)
      current = current->right;
    else
      current = current->left;
  }
  return nullptr;
}

template <typename KeyType, typename ValueType>
void AVL<KeyType, ValueType>::fixup(Node<KeyType, ValueType>* _root) {
  while (_root != nullptr) {
    Node<KeyType, ValueType>* nextParent = _root->parent;
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

template <typename KeyType, typename ValueType>
Node<KeyType, ValueType>* AVL<KeyType, ValueType>::minimumNode(
    Node<KeyType, ValueType>* _root) const {
  while (_root->left != nullptr) {
    _root = _root->left;
  }
  return _root;
}

template <typename KeyType, typename ValueType>
Node<KeyType, ValueType>* AVL<KeyType, ValueType>::maximumNode(
    Node<KeyType, ValueType>* _root) const {
  while (_root->right != nullptr) {
    _root = _root->right;
  }
  return _root;
}

template <typename KeyType, typename ValueType>
Node<KeyType, ValueType>* AVL<KeyType, ValueType>::successorUp(
    Node<KeyType, ValueType>* _root) const {
  Node<KeyType, ValueType>* y = _root->parent;
  while (y != nullptr && _root == y->right) {
    _root = y;
    y = y->parent;
  }
  return y;
}

template <typename KeyType, typename ValueType>
Node<KeyType, ValueType>* AVL<KeyType, ValueType>::predecessorUp(
    Node<KeyType, ValueType>* _root) const {
  Node<KeyType, ValueType>* y = _root->parent;
  while (y != nullptr && _root == y->left) {
    _root = y;
    y = y->parent;
  }
  return y;
}

template <typename KeyType, typename ValueType>
Node<KeyType, ValueType>* AVL<KeyType, ValueType>::findNode(
    const KeyType& key,
    Node<KeyType, ValueType>* _root) const {
  if (_root == nullptr) {
    return nullptr;
  }
  int comp = comparator(key, _root->key);
  if (comp == AVL_GREATER) {
    return findNode(key, _root->right);
  } else if (comp == AVL_LESS) {
    return findNode(key, _root->left);
  } else {
    return _root;
  }
}

// template <typename KeyType, typename ValueType>
// int AVL::maxSumPath(Node<KeyType, ValueType>* _root) const {
//   AVL::response res = maxSumPathRecursive(_root);
//   return std::max({res.maxSumPathForThisNode, res.maxSumPathToLeave,
//                    res.maxMaxSumPathOfSubtrees});
// }

// TODO cual es el key de T
// AVL::response AVL::maxSumPathRecursive(Node<KeyType, ValueType>* _root) const
// {
//   if (_root == nullptr) {
//     response res = {0, 0, 0};
//     return res;
//   }
//   response l = maxSumPathRecursive(_root->left);
//   response r = maxSumPathRecursive(_root->right);

//   response res;
//   res.maxSumPathForThisNode =
//       l.maxSumPathToLeave + r.maxSumPathToLeave + _root->key;
//   res.maxSumPathToLeave =
//       std::max({l.maxSumPathToLeave, r.maxSumPathToLeave}) + _root->key;
//   res.maxMaxSumPathOfSubtrees =
//       std::max({l.maxSumPathForThisNode, r.maxSumPathForThisNode});
//   return res;
// }

template <typename KeyType, typename ValueType>
void AVL<KeyType, ValueType>::inorderTraversal(
    Node<KeyType, ValueType>* _root,
    const std::function<void(const KeyType&, const ValueType&)>& process)
    const {
  if (_root == nullptr) {
    return;
  }
  inorderTraversal(_root->left, process);
  process(_root->key);
  inorderTraversal(_root->right, process);
}

template <typename KeyType, typename ValueType>
void AVL<KeyType, ValueType>::leftRotation(Node<KeyType, ValueType>* x,
                                           Node<KeyType, ValueType>* y) {
  if (y->left) {
    y->left->parent = x;
  }
  x->right = y->left;
  if (x->parent == nullptr) {
    root = y;
  } else if (comparator(x->key, x->parent->key) == AVL_LESS) {
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

template <typename KeyType, typename ValueType>
void AVL<KeyType, ValueType>::rightRotation(Node<KeyType, ValueType>* x,
                                            Node<KeyType, ValueType>* y) {
  if (y->right) {
    y->right->parent = x;
  }
  x->left = y->right;
  if (x->parent == nullptr) {
    root = y;
  } else if (comparator(x->key, x->parent->key) == AVL_GREATER) {
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

template <typename KeyType, typename ValueType>
void AVL<KeyType, ValueType>::insertRecursive(Node<KeyType, ValueType>* current,
                                              const KeyType& key) {
  int comp = comparator(key, current->key);
  if (comp == AVL_GREATER) {
    if (current->right == nullptr) {
      current->right = new Node<KeyType, ValueType>(key);
      current->right->parent = current;
      current->hr = getHeight(current->right);
    } else {
      insertRecursive(current->right, key);
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
      current->left = new Node<KeyType, ValueType>(key);
      current->left->parent = current;
      current->hl = getHeight(current->left);
    } else {
      insertRecursive(current->left, key);
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

template <typename KeyType, typename ValueType>
inline int AVL<KeyType, ValueType>::getHeight(
    Node<KeyType, ValueType>* node) const {
  if (node == nullptr) {
    return 0;
  } else {
    return std::max({node->hl, node->hr}) + 1;
  }
}

template <typename KeyType, typename ValueType>
void AVL<KeyType, ValueType>::clear(Node<KeyType, ValueType>* _root) {
  if (_root == nullptr) {
    return;
  } else {
    clear(_root->left);
    clear(_root->right);
    delete _root;
  }
}

template <typename KeyType, typename ValueType>
AVL<KeyType, ValueType>::~AVL() noexcept {
  clear(root);
}
