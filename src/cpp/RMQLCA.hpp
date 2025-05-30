#ifndef RMQLCA_HPP
#define RMQLCA_HPP
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cassert>
#include <cmath>
#include <limits>
#include "LCA.hpp"
#include "SegmentTree.hpp"

class RMQLCA : public LCA {
public:
  RMQLCA(int n = 1);

  void init(int n) override;
  void add_edge(int u, int v) override;
  void dfs(int u = 1, int p = 0);
  void init_RMQ();
  void build() override;
  int getLCA(int u, int v) override;

private:
  std::vector<int> euler_path, first, depth;
  SegmentTree<Info> seg;
};

#endif // RMQLCA_HPP