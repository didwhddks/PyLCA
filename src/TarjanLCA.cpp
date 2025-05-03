#include <vector>
#include <numeric>
#include <utility>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>

class DSU {
public:
  DSU(int n = 0) {
    par.assign(n + 1, 0);
    size.assign(n + 1, 1);
    iota(par.begin(), par.end(), 0);
  }

  int find(int x) {
    while (x != par[x]) {
      par[x] = par[par[x]];
      x = par[x];
    }
    return x;
  }

  void unite(int u, int v) {
    u = find(u);
    v = find(v);
    if (size[u] > size[v]) {
      std::swap(u, v);
    }
    par[u] = v;
    size[v] += size[u];
  }

private:
  std::vector<int> par, size;
};

struct LCA {
public:
  LCA(int n = 0, int q = 0) {
    adj.assign(n + 1, std::vector<int>());
    anc.assign(n + 1, 0);
    ans.assign(q, 0);
    qry.assign(n + 1, std::vector<std::pair<int, int>>());
    vis.assign(n + 1, false);
    dsu = DSU(n);
  }

  void add_edge(int u, int v) {
    adj[u].emplace_back(v);
    adj[v].emplace_back(u);
  }

  void add_query(int u, int v, int idx) {
    qry[u].emplace_back(v, idx);
    qry[v].emplace_back(u, idx);
  }

  void dfs(int u = 1) {
    vis[u] = 1;
    anc[u] = u;
    for (int v : adj[u]) {
      if (vis[v]) {
        continue;
      }
      dfs(v);
      dsu.unite(u, v);
      anc[dsu.find(u)] = u;
    }
    for (auto& [v, idx] : qry[u]) {
      if (!vis[v]) {
        continue;
      }
      ans[idx] = anc[dsu.find(v)];
    }
  }

  void solve() {
    dfs();
  }

  int query(int idx) {
    return ans[idx];
  }

private:
  std::vector<std::vector<int>> adj;
  std::vector<int> anc, ans;
  std::vector<std::vector<std::pair<int, int>>> qry;
  std::vector<bool> vis;
  DSU dsu;
};
