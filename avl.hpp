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

  // TODO: move constructor en data{data}
  Node(int data)
      : data{data},
        left{nullptr},
        right{nullptr},
        parent{nullptr},
        hl{0},
        hr{0} {}
};

// TODO: poner los TODOs de abajo en un issue
// TODO: templatizar
// TODO: is copy constructible type trait, porque se hacen copias
// TODO: T key, U data
// TODO: un lambda que retorne un valor específico dado un U
// TODO: testing exhaustivo
// TODO: crear una clase extra que sea Map o Hash que use el AVL por debajo
// TODO: ver integrar dicha ED con Node.js
class AVL {
  Node* root;
  std::function<int(int, int)> comparator;

 public:
  // Recibe un lambda que toma dos elementos `a` y `b` como
  // parámetro y retorna 0 si `a == b`, -1 si `a < b` y
  // 1 si `a > b`
  AVL(const std::function<int(int, int)>&);
  int getHeight() const;
  int getRoot() const;
  void insert(int data);
  void fastInsert(int data);
  int maximum() const;
  int minimum() const;
  void inorder(const std::function<void(int)>& process) const;
  std::string inorderString() const;
  int getMaxWeightPathBetweenLeafs() const;
  void remove(int key);
  int predecessor(int key) const;
  int successor(int key) const;
  int findKey(int key) const;
  ~AVL() noexcept;

 private:
  void fixup(Node* _root);
  Node* minimumNode(Node* _root) const;
  Node* maximumNode(Node* _root) const;
  // es básicamente el proceso inverso a minimumNode
  // subimos hasta que detectemos que ahora estamos en
  // el subárbol derecho y como hemos estado subiendo
  // siendo hijos izquierdos, sabemos que:
  // Node* r = successorUp(node);
  // node == minimumNode(r);
  // y por lo tanto encontramos al sucesor
  Node* successorUp(Node* _root) const;
  Node* predecessorUp(Node* _root) const;
  Node* findNode(int key, Node* _root) const;
  int maxSumPath(Node* _root) const;
  struct response {
    int maxSumPathForThisNode;
    int maxSumPathToLeave;
    int maxMaxSumPathOfSubtrees;
  };
  response maxSumPathRecursive(Node* _root) const;
  void inorderTraversal(Node* _root,
                        const std::function<void(int)>& process) const;
  void leftRotation(Node* x, Node* y);
  void rightRotation(Node* x, Node* y);
  void insertRecursive(Node*& current, int data);
  inline int getHeight(Node* node) const;
  void clear(Node* _root);
};