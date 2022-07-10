#include <algorithm>
#include <functional>
#include <iostream>

#define NOT_FOUND INT32_MIN

struct Node {
  int data;
  Node* left;
  Node* right;
  Node* parent;
  int hl;
  int hr;

  Node(int data)
      : data{data},
        left{nullptr},
        right{nullptr},
        parent{nullptr},
        hl{0},
        hr{0} {}
};

class AVL {
  Node* root;

 public:
  AVL() : root{nullptr} {}

  int getHeight() const { return getHeight(root); }

  int getRoot() const { return root->data; };

  void insert(int data) {
    if (root == nullptr) {
      root = new Node(data);
    } else {
      insertRecursive(root, data);
    }
  }

  int maximum() const { return maximumNode(root)->data; }

  int minimum() const { return minimumNode(root)->data; }

  void inorder(const std::function<void(int)>& process) const {
    inorderTraversal(root, process);
  }

  std::string inorderString() const {
    std::string str = "";
    inorderTraversal(root,
                     [&](int data) { str += std::to_string(data) + " "; });
    return str;
  }

  int getMaxWeightPathBetweenLeafs() const { return maxSumPath(root); }

  void remove(int key) {
    Node* nodeToRemove = findNode(key, root);
    if (nodeToRemove->left == nullptr && nodeToRemove->right == nullptr) {
      // node has no children
      if (nodeToRemove->data > nodeToRemove->parent->data) {
        nodeToRemove->parent->right = nullptr;
      } else {
        nodeToRemove->parent->left = nullptr;
      }
      fixup(nodeToRemove->parent);
      delete nodeToRemove;
    } else if (nodeToRemove->left == nullptr ||
               nodeToRemove->right == nullptr) {
      // node has one child
      Node* child =
          nodeToRemove->left ? nodeToRemove->left : nodeToRemove->right;
      if (nodeToRemove->parent->data > nodeToRemove->data) {
        nodeToRemove->parent->right = child;
      } else {
        nodeToRemove->parent->left = child;
      }
      child->parent = nodeToRemove->parent;
      fixup(nodeToRemove->parent);
      delete nodeToRemove;
    } else {
      // node has two children
      if (nodeToRemove->hl < nodeToRemove->hr) {
        // reemplazar con predecesor
        Node* predecessor = maximumNode(nodeToRemove->left);

        int temp = predecessor->data;
        predecessor->data = nodeToRemove->data;
        nodeToRemove->data = temp;

        if (temp > predecessor->parent->data) {
          predecessor->parent->right = nullptr;
        } else {
          predecessor->parent->left = nullptr;
        }
        fixup(predecessor->parent);
        delete predecessor;
      } else {
        // reemplazar con sucesor
        Node* sucessor = minimumNode(nodeToRemove->right);

        int temp = sucessor->data;
        sucessor->data = nodeToRemove->data;
        nodeToRemove->data = temp;

        if (temp > sucessor->parent->data) {
          sucessor->parent->right = nullptr;
        } else {
          sucessor->parent->left = nullptr;
        }
        fixup(sucessor->parent);
        delete sucessor;
      }
    }
  }

  int predecessor(int key) const {
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

  int successor(int key) const {
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

  int findKey(int key) const {
    Node* foundNode = findNode(key, root);
    if (foundNode) {
      return foundNode->data;
    } else {
      return NOT_FOUND;
    }
  }

 private:
  ~AVL() noexcept { clear(root); }

  void fixup(Node* _root) {
    while (_root != nullptr) {
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
      _root = _root->parent;
    }
  }

  Node* minimumNode(Node* _root) const {
    while (_root->left != nullptr) {
      _root = _root->left;
    }
    return _root;
  }

  Node* maximumNode(Node* _root) const {
    while (_root->right != nullptr) {
      _root = _root->right;
    }
    return _root;
  }

  // es básicamente el proceso inverso a minimumNode
  // subimos hasta que detectemos que ahora estamos en
  // el subárbol derecho y como hemos estado subiendo
  // siendo hijos izquierdos, sabemos que:
  // Node* r = successorUp(node);
  // node == minimumNode(r);
  // y por lo tanto encontramos al sucesor
  Node* successorUp(Node* _root) const {
    Node* y = _root->parent;
    while (y != nullptr && _root == y->right) {
      _root = y;
      y = y->parent;
    }
    return y;
  }

  Node* predecessorUp(Node* _root) const {
    Node* y = _root->parent;
    while (y != nullptr && _root == y->left) {
      _root = y;
      y = y->parent;
    }
    return y;
  }

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

  void leftRotation(Node* x, Node* y) {
    if (y->left) {
      y->left->parent = x;
    }
    x->right = y->left;
    if (x->parent == nullptr) {
      root = y;
    } else if (x->data < x->parent->data) {
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

  void rightRotation(Node* x, Node* y) {
    if (y->right) {
      y->right->parent = x;
    }
    x->left = y->right;
    if (x->parent == nullptr) {
      root = y;
    } else if (x->data > x->parent->data) {
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

  void insertRecursive(Node*& current, int data) {
    if (data > current->data) {
      if (current->right == nullptr) {
        current->right = new Node(data);
        current->right->parent = current;
        current->hr = getHeight(current->right);
      } else {
        insertRecursive(current->right, data);
        current->hr = getHeight(current->right);
        int balanceFactor = current->hl - current->hr;
        if (balanceFactor < -1) {
          if (data > current->right->data) {
            leftRotation(current, current->right);
          } else {
            rightRotation(current->right, current->right->left);
            leftRotation(current, current->right);
          }
        }
      }
    } else if (data < current->data) {
      if (current->left == nullptr) {
        current->left = new Node(data);
        current->left->parent = current;
        current->hl = getHeight(current->left);
      } else {
        insertRecursive(current->left, data);
        current->hl = getHeight(current->left);
        int balanceFactor = current->hl - current->hr;
        if (balanceFactor > 1) {
          if (data < current->left->data) {
            rightRotation(current, current->left);
          } else {
            leftRotation(current->left, current->left->right);
            rightRotation(current, current->left);
          }
        }
      }
    } else {
      throw "duplicated key";
    }
  }

  inline int getHeight(Node* node) const {
    if (node == nullptr) {
      return 0;
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