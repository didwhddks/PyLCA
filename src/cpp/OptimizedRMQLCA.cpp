#include "OptimizedRMQLCA.hpp"

OptimizedRMQLCA::OptimizedRMQLCA(int n) {
  init(n);
}

void OptimizedRMQLCA::init(int n) {
  first.assign(n + 1, -1);
  vis.assign(n + 1, 0);
  adj.assign(n + 1, std::vector<int>());
}

void OptimizedRMQLCA::add_edge(int u, int v) {
  adj[u].emplace_back(v);
  adj[v].emplace_back(u);
}

void OptimizedRMQLCA::dfs(int u, int p) {
  std::vector<std::pair<int, int>> stk;
  stk.emplace_back(u, 0);
  vis[u] = 1;
  while (stk.size()) {
    auto [x, d] = stk.back();
    stk.pop_back();
    euler_path.emplace_back(x);
    depth.emplace_back(d);
    if (first[x] != -1) continue;
    first[x] = euler_path.size() - 1;
    for (auto y : adj[x]) {
      if (vis[y]) continue;
      vis[y] = 1;
      stk.emplace_back(x, d);
      stk.emplace_back(y, d + 1);
    }
  }
}

void OptimizedRMQLCA::precompute_log() {
  M = euler_path.size();
  lg.assign(M + 1, 0);
  for (int i = 2; i <= M; ++i) lg[i] = lg[i / 2] + 1;
}

void OptimizedRMQLCA::build_block() {
  block_size = std::max(1, lg[M] / 2);
  block_cnt = (M + block_size - 1) / block_size;
  block.assign(block_cnt, 0);
  mask.assign(block_cnt, 0);
  for (int i = 0, j = 0, b = 0; i < M; i++, j++) {
    if (j == block_size) j = 0, b++;
    if (j == 0 || depth[i] < depth[block[b]]) block[b] = i;
    if (j > 0 && (i >= M || depth[i] > depth[i - 1])) mask[b] += 1 << (j - 1);
  }
}

void OptimizedRMQLCA::build_sparse_table() {
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

void OptimizedRMQLCA::precompute_in_block_RMQ() {
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

void OptimizedRMQLCA::preprocess() {
  dfs();
  precompute_log();
  build_block();
  build_sparse_table();
  precompute_in_block_RMQ();
}

int OptimizedRMQLCA::getLCA(int u, int v) {
  u = first[u];
  v = first[v];
  if (u > v) std::swap(u, v);

  int u_block = u / block_size, v_block = v / block_size;
  int u_offset = u % block_size, v_offset = v % block_size;

  if (u_block == v_block)
    return euler_path[u_block * block_size +
                      in_block_RMQ[mask[u_block]][u_offset][v_offset]];

  int min1 = u_block * block_size +
            in_block_RMQ[mask[u_block]][u_offset][block_size - 1];
  int min2 = v_block * block_size + in_block_RMQ[mask[v_block]][0][v_offset];
  int lca = depth[min1] < depth[min2] ? min1 : min2;

  if (u_block + 1 < v_block) {
    int sz = lg[v_block - u_block - 1];
    int mn3 = st[u_block + 1][sz], mn4 = st[v_block - (1 << sz)][sz];
    int mn = depth[mn3] < depth[mn4] ? mn3 : mn4;
    lca = depth[lca] < depth[mn] ? lca : mn;
  }

  return euler_path[lca];
}