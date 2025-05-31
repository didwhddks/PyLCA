#include "BinaryLiftingLCA.hpp"

BinaryLiftingLCA::BinaryLiftingLCA(int n) {
  init(n);
}

void BinaryLiftingLCA::init(int n) {
  logN = std::log2(n);
  anc.assign(n + 1, std::vector<int>(logN + 1, 0));
  tin.assign(n + 1, 0);
  tout.assign(n + 1, 0);
  adj.assign(n + 1, std::vector<int>());
}

void BinaryLiftingLCA::add_edge(int u, int v) {
  adj[u].emplace_back(v);
  adj[v].emplace_back(u);
}

void BinaryLiftingLCA::dfs(int u, int p) {
  std::vector<int> stk;
  stk.push_back(u);
  stk.push_back(u);
  anc[u][0] = p;
  while (stk.size()) {
    auto x = stk.back();
    stk.pop_back();
    if (tin[x] > 0) {
      tout[x] = ++time;
      continue;
    }
    tin[x] = ++time;
    for (int i = 1; i <= logN; i++) {
        anc[x][i] = anc[anc[x][i - 1]][i - 1];
    }
    for (int y : adj[x]) {
        if (y == anc[x][0]) continue;
        anc[y][0] = x;
        stk.push_back(y);
        stk.push_back(y);
    }
  }
}

void BinaryLiftingLCA::preprocess() {
  dfs();
  tout[0] = ++time;
}

bool BinaryLiftingLCA::is_ancestor(int u, int v) {
  return tin[u] <= tin[v] && tout[v] <= tout[u];
}

int BinaryLiftingLCA::getLCA(int u, int v) {
  if (is_ancestor(u, v)) return u;
  if (is_ancestor(v, u)) return v;
  for (int i = logN; i >= 0; i--) {
    if (!is_ancestor(anc[u][i], v)) {
      u = anc[u][i];
    }
  }
  return anc[u][0];
}
