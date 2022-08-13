#include <algorithm>
#include <functional>
#include <iostream>
#include <optional>
#include <type_traits>

#include "../utils/helpers.hpp"

template <typename T>
concept MoveAssignable = std::is_move_assignable<T>::value;

template <MoveAssignable KeyType, MoveAssignable ValueType>
struct Node {
  KeyType key;
  ValueType value;
  Node* left;
  Node* right;
  Node* parent;
  int hl;
  int hr;

  explicit Node(const KeyType& key, const ValueType& value)
      : key{key},
        value{value},
        left{nullptr},
        right{nullptr},
        parent{nullptr},
        hl{0},
        hr{0} {}
};

// TODO: crear una clase extra que sea Map o Hash que use el AVL por debajo
// TODO: ver integrar dicha ED con Node.js
template <MoveAssignable KeyType, MoveAssignable ValueType>
class AVL {
  Node<KeyType, ValueType>* root;
  std::function<int(const KeyType&, const KeyType&)> comparator;

 public:
  // Recibe un lambda que toma dos elementos `a` y `b` como
  // parámetro y retorna -1 si `a < b`, 1 si `a > b` y 0 si `a == b`.
  // Si no se cumple esta regla, el comportamiento del AVL es indefinido.
  explicit AVL(
      const std::function<int(const KeyType&, const KeyType&)>& comparator);
  AVL(const AVL&) = delete;                     // copy constructor
  auto operator=(const AVL&) -> AVL& = delete;  // copy assignment operator
  AVL(AVL&&) = delete;                          // move constructor
  auto operator=(AVL&&) -> AVL& = delete;       // move assignment operator
  [[nodiscard]] inline auto getHeight() const -> int;
  [[nodiscard]] auto getRoot() const -> std::optional<KeyType>;
  auto insert(const KeyType& key, const ValueType& value) -> void;
  auto iterativeInsert(const KeyType& key, const ValueType& value) -> void;
  auto maximum() const -> std::optional<std::tuple<KeyType, ValueType>>;
  auto minimum() const -> std::optional<std::tuple<KeyType, ValueType>>;
  auto inorder(const std::function<void(const KeyType&, const ValueType&)>&
                   process) const -> void;
  [[nodiscard]] auto inorderString() const -> std::string;
  auto remove(const KeyType& key) -> void;
  auto predecessor(const KeyType& key) const
      -> std::optional<std::tuple<KeyType, ValueType>>;
  auto successor(const KeyType& key) const
      -> std::optional<std::tuple<KeyType, ValueType>>;
  auto find(const KeyType& key) const -> std::optional<ValueType>;
  auto findKey(const KeyType& key) const -> std::optional<KeyType>;
  auto iterativeFindKey(const KeyType& key) const -> std::optional<KeyType>;
  ~AVL() noexcept;

 private:
  auto fixup(Node<KeyType, ValueType>* _root) -> void;
  auto minimumNode(Node<KeyType, ValueType>* _root) const
      -> Node<KeyType, ValueType>*;
  auto maximumNode(Node<KeyType, ValueType>* _root) const
      -> Node<KeyType, ValueType>*;
  auto successorUp(Node<KeyType, ValueType>* _root) const
      -> Node<KeyType, ValueType>*;
  auto predecessorUp(Node<KeyType, ValueType>* _root) const
      -> Node<KeyType, ValueType>*;
  auto findNode(const KeyType& key, Node<KeyType, ValueType>* _root) const
      -> Node<KeyType, ValueType>*;
  auto inorderTraversal(
      Node<KeyType, ValueType>* _root,
      const std::function<void(const KeyType&, const ValueType&)>& process)
      const -> void;
  auto leftRotation(Node<KeyType, ValueType>* x, Node<KeyType, ValueType>* y)
      -> void;
  auto rightRotation(Node<KeyType, ValueType>* x, Node<KeyType, ValueType>* y)
      -> void;
  auto insertRecursive(Node<KeyType, ValueType>* current,
                       const KeyType& key,
                       const ValueType& value) -> void;
  inline auto getHeight(Node<KeyType, ValueType>* node) const -> int;
  auto clear(Node<KeyType, ValueType>* _root) -> void;
};

const int AVL_GREATER = 1;
const int AVL_LESS = -1;
const int AVL_EQUAL = 0;

template <MoveAssignable KeyType, MoveAssignable ValueType>
AVL<KeyType, ValueType>::AVL(
    const std::function<int(const KeyType&, const KeyType&)>& comparator)
    : root{nullptr}, comparator{comparator} {}

template <MoveAssignable KeyType, MoveAssignable ValueType>
inline auto AVL<KeyType, ValueType>::getHeight() const -> int {
  return getHeight(root);
}

template <MoveAssignable KeyType, MoveAssignable ValueType>
auto AVL<KeyType, ValueType>::getRoot() const -> std::optional<KeyType> {
  if (root) {
    return root->key;
  } else {
    return {};
  }
}

template <MoveAssignable KeyType, MoveAssignable ValueType>
void AVL<KeyType, ValueType>::insert(const KeyType& key,
                                     const ValueType& value) {
  if (root == nullptr) {
    root = new Node<KeyType, ValueType>(key, value);
  } else {
    insertRecursive(root, key, value);
  }
}

template <MoveAssignable KeyType, MoveAssignable ValueType>
void AVL<KeyType, ValueType>::iterativeInsert(const KeyType& key,
                                              const ValueType& value) {
  if (root == nullptr) {
    root = new Node<KeyType, ValueType>(key, value);
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
      parent->right = new Node<KeyType, ValueType>(key, value);
      parent->right->parent = parent;
    } else {
      parent->left = new Node<KeyType, ValueType>(key, value);
      parent->left->parent = parent;
    }
    fixup(parent);
  }
}

template <MoveAssignable KeyType, MoveAssignable ValueType>
auto AVL<KeyType, ValueType>::maximum() const
    -> std::optional<std::tuple<KeyType, ValueType>> {
  Node<KeyType, ValueType>* node = maximumNode(root);
  if (node) {
    return std::make_tuple(node->key, node->value);
  }
  return {};
}

template <MoveAssignable KeyType, MoveAssignable ValueType>
auto AVL<KeyType, ValueType>::minimum() const
    -> std::optional<std::tuple<KeyType, ValueType>> {
  Node<KeyType, ValueType>* node = minimumNode(root);
  if (node) {
    return std::make_tuple(node->key, node->value);
  }
  return {};
}

template <MoveAssignable KeyType, MoveAssignable ValueType>
auto AVL<KeyType, ValueType>::inorder(
    const std::function<void(const KeyType&, const ValueType&)>& process) const
    -> void {
  inorderTraversal(root, process);
}

template <MoveAssignable KeyType, MoveAssignable ValueType>
auto AVL<KeyType, ValueType>::inorderString() const -> std::string {
  std::string str;
  inorderTraversal(root, [&str](const KeyType& key, const ValueType& value) {
    str += std::to_string(key) + " ";
  });
  return str;
}

template <MoveAssignable KeyType, MoveAssignable ValueType>
auto AVL<KeyType, ValueType>::remove(const KeyType& key) -> void {
  Node<KeyType, ValueType>* nodeToRemove = findNode(key, root);
  if (nodeToRemove == nullptr) {
    return;
  }
  Node<KeyType, ValueType>* leftChild = nodeToRemove->left;
  Node<KeyType, ValueType>* rightChild = nodeToRemove->right;
  if (leftChild == nullptr && rightChild == nullptr) {
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
    nodeToRemove->key = std::move(replacement->key);
    nodeToRemove->value = std::move(replacement->value);
    fixup(replacement->parent);
    delete replacement;
  }
}

template <MoveAssignable KeyType, MoveAssignable ValueType>
auto AVL<KeyType, ValueType>::predecessor(const KeyType& key) const
    -> std::optional<std::tuple<KeyType, ValueType>> {
  Node<KeyType, ValueType>* foundNode = findNode(key, root);
  if (foundNode) {
    if (foundNode->left) {
      Node<KeyType, ValueType>* predecessor = maximumNode(foundNode->left);
      return std::make_tuple(predecessor->key, predecessor->value);
    }
    Node<KeyType, ValueType>* predecessor = predecessorUp(foundNode);
    if (predecessor) {
      return std::make_tuple(predecessor->key, predecessor->value);
    } else {
      return {};
    }
  }
  return {};
}

template <MoveAssignable KeyType, MoveAssignable ValueType>
auto AVL<KeyType, ValueType>::successor(const KeyType& key) const
    -> std::optional<std::tuple<KeyType, ValueType>> {
  Node<KeyType, ValueType>* foundNode = findNode(key, root);
  if (foundNode) {
    if (foundNode->right) {
      Node<KeyType, ValueType>* predecessor = minimumNode(foundNode->right);
      return std::make_tuple(predecessor->key, predecessor->value);
    }
    Node<KeyType, ValueType>* successor = successorUp(foundNode);
    if (successor) {
      return std::make_tuple(successor->key, successor->value);
    } else {
      return {};
    }
  }
  return {};
}

template <MoveAssignable KeyType, MoveAssignable ValueType>
auto AVL<KeyType, ValueType>::findKey(const KeyType& key) const
    -> std::optional<KeyType> {
  // NOLINTNEXTLINE
  Node<KeyType, ValueType>* foundNode = findNode(key, root);
  if (foundNode) {
    return foundNode->key;
  }
  return {};
}

template <MoveAssignable KeyType, MoveAssignable ValueType>
auto AVL<KeyType, ValueType>::find(const KeyType& key) const
    -> std::optional<ValueType> {
  Node<KeyType, ValueType>* foundNode = findNode(key, root);
  if (foundNode) {
    return foundNode->value;
  }
  return {};
}

template <MoveAssignable KeyType, MoveAssignable ValueType>
auto AVL<KeyType, ValueType>::iterativeFindKey(const KeyType& key) const
    -> std::optional<KeyType> {
  Node<KeyType, ValueType>* current = root;
  while (current) {
    int comp = comparator(key, current->key);
    if (comp == AVL_EQUAL) {
      return current->key;
    }
    if (comp == AVL_GREATER) {
      current = current->right;
    } else {
      current = current->left;
    }
  }
  return {};
}

template <MoveAssignable KeyType, MoveAssignable ValueType>
auto AVL<KeyType, ValueType>::fixup(Node<KeyType, ValueType>* _root) -> void {
  while (_root != nullptr) {
    Node<KeyType, ValueType>* nextParent = _root->parent;
    _root->hl = getHeight(_root->left);
    _root->hr = getHeight(_root->right);
    int balanceFactor = _root->hl - _root->hr;

    if (balanceFactor < -1) {
      // right heavy
      // NOLINTNEXTLINE
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
      // NOLINTNEXTLINE
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

template <MoveAssignable KeyType, MoveAssignable ValueType>
auto AVL<KeyType, ValueType>::minimumNode(Node<KeyType, ValueType>* _root) const
    -> Node<KeyType, ValueType>* {
  while (_root->left != nullptr) {
    _root = _root->left;
  }
  return _root;
}

template <MoveAssignable KeyType, MoveAssignable ValueType>
auto AVL<KeyType, ValueType>::maximumNode(Node<KeyType, ValueType>* _root) const
    -> Node<KeyType, ValueType>* {
  while (_root->right != nullptr) {
    _root = _root->right;
  }
  return _root;
}

template <MoveAssignable KeyType, MoveAssignable ValueType>
auto AVL<KeyType, ValueType>::successorUp(Node<KeyType, ValueType>* _root) const
    -> Node<KeyType, ValueType>* {
  Node<KeyType, ValueType>* y = _root->parent;
  while (y != nullptr && _root == y->right) {
    _root = y;
    y = y->parent;
  }
  return y;
}

template <MoveAssignable KeyType, MoveAssignable ValueType>
auto AVL<KeyType, ValueType>::predecessorUp(
    Node<KeyType, ValueType>* _root) const -> Node<KeyType, ValueType>* {
  Node<KeyType, ValueType>* y = _root->parent;
  while (y != nullptr && _root == y->left) {
    _root = y;
    y = y->parent;
  }
  return y;
}

template <MoveAssignable KeyType, MoveAssignable ValueType>
auto AVL<KeyType, ValueType>::findNode(const KeyType& key,
                                       Node<KeyType, ValueType>* _root) const
    -> Node<KeyType, ValueType>* {
  if (_root == nullptr) {
    return nullptr;
  }
  int comp = comparator(key, _root->key);
  if (comp == AVL_GREATER) {
    // NOLINTNEXTLINE
    return findNode(key, _root->right);
  } else if (comp == AVL_LESS) {
    // NOLINTNEXTLINE
    return findNode(key, _root->left);
  } else {
    return _root;
  }
}

template <MoveAssignable KeyType, MoveAssignable ValueType>
void AVL<KeyType, ValueType>::inorderTraversal(
    Node<KeyType, ValueType>* _root,
    const std::function<void(const KeyType&, const ValueType&)>& process)
    const {
  if (_root == nullptr) {
    return;
  }
  inorderTraversal(_root->left, process);
  process(_root->key, _root->value);
  inorderTraversal(_root->right, process);
}

template <MoveAssignable KeyType, MoveAssignable ValueType>
void AVL<KeyType, ValueType>::leftRotation(Node<KeyType, ValueType>* x,
                                           Node<KeyType, ValueType>* y) {
  // NOLINTNEXTLINE
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

template <MoveAssignable KeyType, MoveAssignable ValueType>
void AVL<KeyType, ValueType>::rightRotation(Node<KeyType, ValueType>* x,
                                            Node<KeyType, ValueType>* y) {
  // NOLINTNEXTLINE
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

template <MoveAssignable KeyType, MoveAssignable ValueType>
void AVL<KeyType, ValueType>::insertRecursive(Node<KeyType, ValueType>* current,
                                              const KeyType& key,
                                              const ValueType& value) {
  int comp = comparator(key, current->key);
  if (comp == AVL_GREATER) {
    if (current->right == nullptr) {
      current->right = new Node<KeyType, ValueType>(key, value);
      current->right->parent = current;
      current->hr = getHeight(current->right);
    } else {
      insertRecursive(current->right, key, value);
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
      current->left = new Node<KeyType, ValueType>(key, value);
      current->left->parent = current;
      current->hl = getHeight(current->left);
    } else {
      insertRecursive(current->left, key, value);
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

template <MoveAssignable KeyType, MoveAssignable ValueType>
inline auto AVL<KeyType, ValueType>::getHeight(
    Node<KeyType, ValueType>* node) const -> int {
  if (node == nullptr) {
    return 0;
  }
  return std::max({node->hl, node->hr}) + 1;
}

template <MoveAssignable KeyType, MoveAssignable ValueType>
void AVL<KeyType, ValueType>::clear(Node<KeyType, ValueType>* _root) {
  if (_root == nullptr) {
    return;
  }
  clear(_root->left);
  clear(_root->right);
  delete _root;
}

template <MoveAssignable KeyType, MoveAssignable ValueType>
AVL<KeyType, ValueType>::~AVL() noexcept {
  clear(root);
}
