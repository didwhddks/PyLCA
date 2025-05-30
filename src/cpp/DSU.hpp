#ifndef DSU_HPP
#define DSU_HPP
#include <vector>
#include <numeric>
#include <algorithm>

class DSU {
public:
  DSU(int n = 1);
  
  int find(int x);
  void unite(int u, int v);

private:
  std::vector<int> par, size;
};

#endif // DSU_HPP