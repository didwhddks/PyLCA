#pragma once
#ifndef LCA_HPP
#define LCA_HPP
#include <vector>

class LCA {
public:
  LCA() = default;
  virtual ~LCA() = default;
  virtual void init(int n) = 0;
  virtual void add_edge(int u, int v) = 0;
  virtual void build() = 0;
  virtual int getLCA(int u, int v) = 0;

  std::vector<std::vector<int>> adj;
};

#endif // LCA_HPP