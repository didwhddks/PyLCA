#ifndef BINARY_LIFTING_LCA_HPP
#define BINARY_LIFTING_LCA_HPP
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cassert>
#include <cmath>
#include "LCA.hpp"

class BinaryLiftingLCA : public LCA {
public:
  BinaryLiftingLCA(int n = 0);

  void init(int n) override;
  void add_edge(int u, int v) override;
  void dfs(int u = 1, int p = 0);
  void build() override;
  bool is_ancestor(int u, int v);
  int getLCA(int u, int v) override;

private:
  int time = 0, logN;
  std::vector<int> tin, tout;
  std::vector<std::vector<int>> anc;
};

#endif // BINARY_LIFTING_LCA_HPP