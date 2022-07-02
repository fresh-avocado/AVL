#include <algorithm>
#include <functional>
#include <iostream>

struct Node {
  int data;
  Node* left;
  Node* right;
  int hl;
  int hr;

  Node(int data) : data{data}, left{nullptr}, right{nullptr}, hl{0}, hr{0} {}
};

class AVL {
  Node* root;

 public:
  AVL() : root{nullptr} {}

  void insert(int data) {
    if (root == nullptr) {
      root = new Node(data);
    } else {
      Node* parentOfRoot = nullptr;
      insertRecursive(parentOfRoot, root, data);
    }
  }

  void inorder(const std::function<void(int)>& process) const {
    inorderTraversal(root, process);
  }

  int getMaxWeightPathBetweenLeafs() const { return maxSumPath(root); }

  // TODO: predecessor
  // TODO: delete

  int successor(int key) const {
    Node* foundNode = findNode(key, root);  // O(lg n)
    if (foundNode) {
      if (foundNode->right) {
        return minimumNode(foundNode->right)->data;  // O(lg n)
      } else {
        Node* successor = nullptr;
        int lock = 0;
        successorUp(key, nullptr, root, &successor, &lock);  // O(lg n)
        return successor != nullptr ? successor->data : INT32_MIN;
      }
    } else {
      return INT32_MIN;
    }
  }

  int findKey(int key) const {
    Node* foundNode = findNode(key, root);
    if (foundNode) {
      return foundNode->data;
    } else {
      return INT32_MIN;
    }
  }

  ~AVL() noexcept { clear(root); }

 private:
  Node* minimumNode(Node* _root) const {
    while (_root->left != nullptr) {
      _root = _root->left;
    }
    return _root;
  }

  void successorUp(int key,
                   Node* _root,
                   Node* node,
                   Node** successor,
                   int* lock) const {
    if (node == nullptr) {
      return;
    }
    if (key > node->data) {
      successorUp(key, node, node->right, successor, lock);
    } else if (key < node->data) {
      successorUp(key, node, node->left, successor, lock);
    }
    if (_root == nullptr) {
      return;
    } else {
      if (_root->left == node && *lock != 1) {
        *successor = _root;
        *lock = 1;
        return;
      }
    }
  }

  // Node* successorDown(int key, Node* _root) const {
  //   if (_root == nullptr) {
  //     return nullptr;
  //   }
  //   if (key > _root->data) {
  //     return successorDown(key, _root->right);
  //   } else if (key < _root->data) {
  //     return successorDown(key, _root->left);
  //   } else {
  //     return minimumNode(_root->right);
  //   }
  // }

  Node* findNode(int key, Node* _root) const {
    if (_root == nullptr) {
      return nullptr;
    }
    if (key > _root->data) {
      return findNode(key, _root->right);
    } else if (key < _root->data) {
      return findNode(key, _root->left);
    } else {
      return _root;
    }
  }

  struct response {
    int maxSumPathForThisNode;
    int maxSumPathToLeave;
    int maxMaxSumPathOfSubtrees;
  };

  int maxSumPath(Node* _root) const {
    response res = maxSumPathRecursive(_root);
    return std::max({res.maxSumPathForThisNode, res.maxSumPathToLeave,
                     res.maxMaxSumPathOfSubtrees});
  }

  response maxSumPathRecursive(Node* _root) const {
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

  void inorderTraversal(Node* _root,
                        const std::function<void(int)>& process) const {
    if (_root == nullptr) {
      return;
    }
    inorderTraversal(_root->left, process);
    process(_root->data);
    inorderTraversal(_root->right, process);
  }

  void leftRotation(Node* parent, Node* x, Node* y) {
    x->right = y->left;
    if (parent == nullptr) {
      root = y;
    } else if (x->data < parent->data) {
      parent->left = y;
    } else {
      parent->right = y;
    }
    y->left = x;

    x->hr = getHeight(x->right);
    y->hl = getHeight(y->left);
  }

  void rightRotation(Node* parent, Node* y, Node* x) {
    y->left = x->right;
    if (parent == nullptr) {
      root = x;
    } else if (y->data > parent->data) {
      parent->right = x;
    } else {
      parent->left = x;
    }
    x->right = y;

    y->hl = getHeight(y->left);
    x->hr = getHeight(x->right);
  }

  void insertRecursive(Node*& parent, Node*& current, int data) {
    if (data > current->data) {
      if (current->right == nullptr) {
        current->right = new Node(data);
        current->hr = getHeight(current->right);
      } else {
        insertRecursive(current, current->right, data);
        current->hr = getHeight(current->right);
        int balanceFactor = current->hl - current->hr;
        if (balanceFactor < -1) {
          if (data > current->right->data) {
            leftRotation(parent, current, current->right);
          } else {
            rightRotation(current, current->right, current->right->left);
            leftRotation(parent, parent->right, parent->right->right);
          }
        }
      }
    } else if (data < current->data) {
      if (current->left == nullptr) {
        current->left = new Node(data);
        current->hl = getHeight(current->left);
      } else {
        insertRecursive(current, current->left, data);
        current->hl = getHeight(current->left);
        int balanceFactor = current->hl - current->hr;
        if (balanceFactor > 1) {
          if (data < current->left->data) {
            rightRotation(parent, current, current->left);
          } else {
            leftRotation(current, current->left, current->left->right);
            rightRotation(parent, parent->left, parent->left->left);
          }
        }
      }
    } else {
      return;
    }
  }

  inline int getHeight(Node*& node) const {
    if (node == nullptr) {
      return -1;
    } else {
      return std::max({node->hl, node->hr}) + 1;
    }
  }

  void clear(Node* _root) {
    if (_root == nullptr) {
      return;
    } else {
      clear(_root->left);
      clear(_root->right);
      delete _root;
    }
  }
};