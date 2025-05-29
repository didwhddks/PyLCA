#include "RMQLCA.hpp"

RMQLCA::RMQLCA(int n) {
  init(n);
}

void RMQLCA::init(int n) {
  first.resize(n + 1);
  depth.assign(n + 1, 0);
  adj.assign(n + 1, std::vector<int>());
}

void RMQLCA::add_edge(int u, int v) {
  adj[u].emplace_back(v);
  adj[v].emplace_back(u);
}

void RMQLCA::dfs(int u, int p) {
  first[u] = euler_path.size();
  euler_path.emplace_back(u);
  for (int v : adj[u]) {
    if (v == p) continue;
    depth[v] = depth[u] + 1;
    dfs(v, u);
    euler_path.emplace_back(u);
  }
}

void RMQLCA::init_RMQ() {
  const int n = euler_path.size();
  seg = SegmentTree<Info>(n);
  for (int i = 0; i < n; i++) {
    int x = euler_path[i];
    seg.set(i, Info(depth[x], x));
  }
}

void RMQLCA::build() {
  dfs();
  init_RMQ();
}

int RMQLCA::getLCA(int u, int v) {
  u = first[u];
  v = first[v];
  if (u > v) std::swap(u, v);
  int lca = seg.rangeQuery(u, v).u_;
  return lca;
}
