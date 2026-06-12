// Copyright 2022 NNTU-CS
#ifndef INCLUDE_TREE_H_
#define INCLUDE_TREE_H_
#include <vector>
class PMTree {
 public:
  struct Node {
    char value;
    std::vector<Node*> branches;
    explicit Node(char v = 0) : value(v) {}
  };
 private:
  Node* root;
  std::vector<char> source;
  Node* construct(const std::vector<char>& remaining);
  void erase(Node* vertex);
 public:
  explicit PMTree(const std::vector<char>& input);
  ~PMTree();
  Node* getRoot() const { return root; }
  const std::vector<char>& getSource() const { return source; }
};
std::vector<std::vector<char>> getAllPerms(PMTree& container);
std::vector<char> getPerm1(PMTree& container, int position);
std::vector<char> getPerm2(PMTree& container, int position);
#endif  // INCLUDE_TREE_H_
