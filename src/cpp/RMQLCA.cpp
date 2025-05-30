#include "RMQLCA.hpp"

RMQLCA::RMQLCA(int n) {
  init(n);
}
 
void RMQLCA::init(int n) {
  first.assign(n + 1, -1);
  depth.assign(n + 1, -1);
  adj.assign(n + 1, std::vector<int>());
}
 
void RMQLCA::add_edge(int u, int v) {
  adj[u].emplace_back(v);
  adj[v].emplace_back(u);
}
 
void RMQLCA::dfs(int u, int p) {
  std::vector<int> stk;
  stk.push_back(u);
  depth[u] = 0;
  while (stk.size()) {
    auto x = stk.back();
    stk.pop_back();
    euler_path.push_back(x);
    if (first[x] != -1) continue;
    first[x] = euler_path.size() - 1;
    for (auto y : adj[x]) {
      if (depth[y] != -1) continue;
      depth[y] = depth[x] + 1;
      stk.push_back(x);
      stk.push_back(y);
    }
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