#include "TarjanLCA.hpp"

TarjanLCA::TarjanLCA(int n, int q) {
  ans.resize(q);
  init(n);
}

void TarjanLCA::init(int n) {
  adj.assign(n + 1, std::vector<int>());
  anc.assign(n + 1, 0);
  par.assign(n + 1, 0);
  qry.assign(n + 1, std::vector<std::pair<int, int>>());
  vis.assign(n + 1, false);
  act.assign(n + 1, false);
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
  std::vector<int> stk;
  stk.push_back(u);
  stk.push_back(u);
  vis[u] = 1;
  while (stk.size()) {
    auto x = stk.back();
    stk.pop_back();
    if (act[x]) {
      for (auto &[y, idx] : qry[x]) {
        if (!vis[y]) continue;
        ans[idx] = anc[dsu.find(y)];
      }
      act[x] = 0;
      dsu.unite(x, par[x]);
      anc[dsu.find(par[x])] = par[x];
      continue;
    }
    anc[x] = x;
    act[x] = 1;
    for (auto y : adj[x]) {
      if (vis[y]) continue;
      vis[y] = 1;
      par[y] = x;
      stk.push_back(y);
      stk.push_back(y);
    }
  }
}

void TarjanLCA::preprocess() {
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
