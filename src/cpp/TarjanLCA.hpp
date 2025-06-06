#ifndef TARJAN_LCA_HPP
#define TARJAN_LCA_HPP
#include <vector>
#include <numeric>
#include <utility>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include "DSU.hpp"
#include "LCA.hpp"

class TarjanLCA : public LCA {
public:
  TarjanLCA(int n = 1);

  void init(int n = 1) override;
  void add_edge(int u, int v) override;
  void add_query(int u, int v, int idx);
  void dfs(int u = 1);
  void preprocess() override;
  int getLCA(int u, int v) override;

private:
  std::vector<int> anc, ans, par;
  std::vector<std::vector<std::pair<int, int>>> qry;
  std::vector<bool> vis, act;
  DSU dsu;
};

#endif // TARJAN_LCA_HPP