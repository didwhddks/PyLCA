#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cassert>
#include <cmath>
#include <limits>

template <typename T>
class SegmentTree {
public:
  SegmentTree(int n = 0) {
    init(n);
  }

  SegmentTree(const std::vector<T>& vec) {
    init(vec);
  }

  void init(int n) {
    this->n = n;
    val.assign(4 * n, std::numeric_limits<T>::max());
  }

  void init(const std::vector<T>& vec) {
    this->n = vec.size();
    val.assign(4 * n, std::numeric_limits<T>::max());
    for (int i = 0; i < n; ++i) {
      set(i, vec[i]);
    }
  }

  void pull(int i) {
    val[i] = std::min(val[2 * i], val[2 * i + 1]);
  }

  void set(int idx, T _val, int i, int L, int R) {
    if (L == R) {
      val[i] = _val;
      return;
    }
    int mid = (L + R) / 2;
    if (idx <= mid) {
      set(idx, _val, 2 * i, L, mid);
    } else {
      set(idx, _val, 2 * i + 1, mid + 1, R);
    }
    pull(i);
  }

  void set(int idx, T _val) {
    set(idx, _val, 1, 0, n - 1);
  }

  T rangeQuery(int qL, int qR, int i, int L, int R) {
    if (R < qL || qR < L) {
      return std::numeric_limits<T>::max();
    }
    if (qL <= L && R <= qR) {
      return val[i];
    }
    int mid = (L + R) / 2;
    T a = rangeQuery(qL, qR, 2 * i, L, mid);
    T b = rangeQuery(qL, qR, 2 * i + 1, mid + 1, R);
    return std::min(a, b);
  }

  T rangeQuery(int qL, int qR) {
    return rangeQuery(qL, qR, 1, 0, n - 1);
  }

private:
  int n;
  std::vector<T> val;
};

class OptimizedRMQLCA {
public:
  OptimizedRMQLCA(int n = 0) {
    init(n);
  }

  void init(int n) {
    first_vis.assign(n + 1, 0);
    adj.assign(n + 1, std::vector<int>());
  }

  void add_edge(int u, int v) {
    adj[u].emplace_back(v);
    adj[v].emplace_back(u);
  }

  void dfs(int u = 1, int p = 0, int d = 0) {
    first_vis[u] = euler_path.size();
    euler_path.emplace_back(u);
    depth.emplace_back(d);
    for (int v : adj[u]) {
      if (v == p) continue;
      dfs(v, u, d + 1);
      euler_path.emplace_back(u);
      depth.emplace_back(d);
    }
  }

  void precompute_log() {
    M = euler_path.size();
    lg.assign(M + 1, 0);
    for (int i = 2; i <= M; ++i) lg[i] = lg[i / 2] + 1;
  }
  
  void build_block() {
    block_size = std::max(1, lg[M] / 2);
    block_cnt = (M + block_size - 1) / block_size;
    block.assign(block_cnt, 0);
    mask.assign(block_cnt, 0);
    for (int i = 0, j = 0, b = 0; i < M; ++i, ++j) {
      if (j == block_size) j = 0, ++b;
      if (j == 0 || depth[i] < depth[block[b]]) block[b] = i;
      if (j > 0 && (i >= M || depth[i] > depth[i - 1])) mask[b] += 1 << (j - 1);
    }
  }
  void build_sparse_table() {
    table_size = lg[block_cnt] + 1;
    st.assign(block_cnt, std::vector<int>(table_size));
    for (int i = 0; i < block_cnt; ++i) st[i][0] = block[i];
    for (int j = 1; j < table_size; ++j) {
      for (int i = 0; i < block_cnt; ++i) {
        int ni = i + (1 << (j - 1));
        if (ni >= block_cnt)
          st[i][j] = st[i][j - 1];
        else
          st[i][j] = depth[st[i][j - 1]] < depth[st[ni][j - 1]] ? st[i][j - 1]
                                                                : st[ni][j - 1];
      }
    }
  }
  void precompute_in_block_RMQ() {
    ways = 1 << (block_size - 1);
    in_block_RMQ.assign(ways, std::vector<std::vector<int>>());
    for (int i = 0; i < block_cnt; ++i) {
      if (!in_block_RMQ[mask[i]].empty()) continue;
      in_block_RMQ[mask[i]].assign(block_size, std::vector<int>(block_size));
      for (int j = 0; j < block_size; ++j) {
        in_block_RMQ[mask[i]][j][j] = j;
        for (int k = j + 1; k < block_size; ++k) {
          in_block_RMQ[mask[i]][j][k] = in_block_RMQ[mask[i]][j][k - 1];
          int idx = i * block_size + k;
          if (idx < M &&
              depth[idx] < depth[i * block_size + in_block_RMQ[mask[i]][j][k]])
            in_block_RMQ[mask[i]][j][k] = k;
        }
      }
    }
  }
  void build() {
    dfs();
    precompute_log();
    build_block();
    build_sparse_table();
    precompute_in_block_RMQ();
  }
  int query(int u, int v) {
    u = first_vis[u], v = first_vis[v];
    if (u > v) std::swap(u, v);

    int u_block = u / block_size, v_block = v / block_size,
        u_offset = u % block_size, v_offset = v % block_size;

    if (u_block == v_block)
      return euler_path[u_block * block_size +
                        in_block_RMQ[mask[u_block]][u_offset][v_offset]];

    int mn1 = u_block * block_size +
              in_block_RMQ[mask[u_block]][u_offset][block_size - 1];
    int mn2 = v_block * block_size + in_block_RMQ[mask[v_block]][0][v_offset];
    int lca = depth[mn1] < depth[mn2] ? mn1 : mn2;

    if (u_block + 1 < v_block) {
      int sz = lg[v_block - u_block - 1];
      int mn3 = st[u_block + 1][sz], mn4 = st[v_block - (1 << sz)][sz];
      int mn = depth[mn3] < depth[mn4] ? mn3 : mn4;
      lca = depth[lca] < depth[mn] ? lca : mn;
    }

    return euler_path[lca];
  }

private:
  int M, block_size, block_cnt, table_size, ways;
  std::vector<std::vector<std::vector<int>>> in_block_RMQ;
  std::vector<std::vector<int>> adj, st;
  std::vector<int> euler_path, depth, first_vis, lg, block, mask;
};
