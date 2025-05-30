#ifndef OPTIMIZED_RMQLCA_HPP
#define OPTIMIZED_RMQLCA_HPP
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <limits>
#include <cassert>
#include <iostream>
#include "LCA.hpp"

class OptimizedRMQLCA : public LCA {
public:
  OptimizedRMQLCA(int n = 1);
  void init(int n) override;
  void add_edge(int u, int v) override;
  void dfs(int u = 1, int p = 0);
  void precompute_log();
  void build_block();
  void build_sparse_table() ;
  void precompute_in_block_RMQ();
  void build() override;
  int getLCA(int u, int v) override;

private:
  int M, block_size, block_cnt, table_size, ways;
  std::vector<std::vector<std::vector<int>>> in_block_RMQ;
  std::vector<std::vector<int>> st;
  std::vector<int> euler_path, depth, first, lg, block, mask, vis;
};

#endif // OPTIMIZED_RMQLCA_HPP