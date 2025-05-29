#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cassert>
#include <cmath>
#include <limits>

struct Info {
  int dep_, u_;
  Info(int dep = std::numeric_limits<int>::max(), int u = 0) : dep_(dep), u_(u) {}
  Info operator+(const Info& other) const {
    if (dep_ < other.dep_) return *this;
    return other;
  }
};

template <typename T>
class SegmentTree {
public:
  SegmentTree(int n = 0) {
    init(n);
  }

  void init(int n) {
    this->n = n;
    info.assign(4 * n, Info());
  }

  void pull(int i) {
    info[i] = info[2 * i] + info[2 * i + 1];
  }

  void set(int idx, T _info, int i, int L, int R) {
    if (L == R) {
      info[i] = _info;
      return;
    }
    int mid = (L + R) / 2;
    if (idx <= mid) {
      set(idx, _info, 2 * i, L, mid);
    } else {
      set(idx, _info, 2 * i + 1, mid + 1, R);
    }
    pull(i);
  }

  void set(int idx, T _info) {
    set(idx, _info, 1, 0, n - 1);
  }

  T rangeQuery(int qL, int qR, int i, int L, int R) {
    if (R < qL || qR < L) {
      return Info();
    }
    if (qL <= L && R <= qR) {
      return info[i];
    }
    int mid = (L + R) / 2;
    T a = rangeQuery(qL, qR, 2 * i, L, mid);
    T b = rangeQuery(qL, qR, 2 * i + 1, mid + 1, R);
    return a + b;
  }

  T rangeQuery(int qL, int qR) {
    return rangeQuery(qL, qR, 1, 0, n - 1);
  }

private:
  int n;
  std::vector<T> info;
};

class RMQLCA {
public:
  RMQLCA(int n = 0) {
    init(n);
  }

  void init(int n) {
    first.resize(n + 1);
    depth.assign(n + 1, 0);
    adj.assign(n + 1, std::vector<int>());
  }

  void add_edge(int u, int v) {
    adj[u].emplace_back(v);
    adj[v].emplace_back(u);
  }

  void dfs(int u = 1, int p = 0) {
    first[u] = euler_path.size();
    euler_path.emplace_back(u);
    for (int v : adj[u]) {
      if (v == p) continue;
      depth[v] = depth[u] + 1;
      dfs(v, u);
      euler_path.emplace_back(u);
    }
  }

  void init_RMQ() {
    const int n = euler_path.size();
    seg = SegmentTree<Info>(n);
    for (int i = 0; i < n; i++) {
      int x = euler_path[i];
      seg.set(i, Info(depth[x], x));
    }
  }

  void build() {
    dfs();
    init_RMQ();
  }

  int getLCA(int u, int v) {
    u = first[u];
    v = first[v];
    if (u > v) std::swap(u, v);
    int lca = seg.rangeQuery(u, v).u_;
    return lca;
  }

private:
  std::vector<std::vector<int>> adj;
  std::vector<int> euler_path, first, depth;
  SegmentTree<Info> seg;
};
