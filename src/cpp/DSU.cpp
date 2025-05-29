#include "DSU.hpp"

DSU::DSU(int n) {
  par.assign(n + 1, 0);
  size.assign(n + 1, 1);
  iota(par.begin(), par.end(), 0);
}

int DSU::find(int x) {
  while (x != par[x]) {
    par[x] = par[par[x]];
    x = par[x];
  }
  return x;
}

void DSU::unite(int u, int v) {
  u = find(u);
  v = find(v);
  if (size[u] > size[v]) std::swap(u, v);
  par[u] = v;
  size[v] += size[u];
}
