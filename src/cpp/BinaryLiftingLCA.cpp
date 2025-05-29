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
  tin[u] = ++time;
  anc[u][0] = p;
  for (int i = 1; i <= logN; ++i) {
    anc[u][i] = anc[anc[u][i - 1]][i - 1];
  }
  for (int v : adj[u]) {
    if (v == p) continue;
    dfs(v, u);
  }
  tout[u] = ++time;
}

void BinaryLiftingLCA::build() {
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
