#include <bits/stdc++.h>
#include "SegmentTree.hpp"
#include "RMQLCA.hpp"
using namespace std;
 
int main() {
  ios::sync_with_stdio(0);
  cin.tie(0);
 
  string in = "../../testcases/3.txt";
  if (freopen(in.c_str(), "r", stdin) == nullptr) {
    cerr << "Error opening input file: " << in << endl;
    return 1;
  }

  string out = "../../testcases/answer/3.out";
  if (freopen(out.c_str(), "w", stdout) == nullptr) {
    cerr << "Error opening output file: " << out << endl;
    return 1;
  }

  int N, Q;
  cin >> N >> Q;
 
  RMQLCA lca;
  lca.init(N);

  for (int i = 0; i < N - 1; i++) {
    int a, b;
    cin >> a >> b;
    lca.add_edge(a, b);
  }

  lca.build();
 
  for (int i = 0; i < Q; ++i) {
    int u, v;
    cin >> u >> v;
    cout << lca.getLCA(u, v) << "\n";
  }
 
  return 0;
}