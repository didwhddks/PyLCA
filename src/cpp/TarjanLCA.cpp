#include "TarjanLCA.hpp"

TarjanLCA::TarjanLCA(int n, int q) {
  adj.assign(n + 1, std::vector<int>());
  anc.assign(n + 1, 0);
  ans.assign(q, 0);
  qry.assign(n + 1, std::vector<std::pair<int, int>>());
  vis.assign(n + 1, false);
  dsu = DSU(n);
}

void TarjanLCA::add_edge(int u, int v) {
  adj[u].emplace_back(v);
  adj[v].emplace_back(u);
}

void TarjanLCA::add_query(int u, int v, int idx) {
  qry[u].emplace_back(v, idx);
  qry[v].emplace_back(u, idx);
}

void TarjanLCA::dfs(int u) {
  vis[u] = 1;
  anc[u] = u;
  for (int v : adj[u]) {
    if (vis[v]) continue;
    dfs(v);
    dsu.unite(u, v);
    anc[dsu.find(u)] = u;
  }
  for (auto &[v, idx] : qry[u]) {
    if (!vis[v]) continue;
    ans[idx] = anc[dsu.find(v)];
  }
}

void TarjanLCA::build() {
  dfs();
}

int TarjanLCA::getLCA(int u, int v) {
  assert(vis[u] && vis[v]);
  for (auto &[v_, idx] : qry[u]) {
    if (v_ == v) {
      return ans[idx];
    }
  }
  return -1;
}
