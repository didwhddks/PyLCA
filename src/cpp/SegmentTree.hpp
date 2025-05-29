#ifndef SEGMENT_TREE_HPP
#define SEGMENT_TREE_HPP
#include <vector>
#include <limits>
#include <algorithm>

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
  SegmentTree(int n = 0);

  void init(int n);
  void pull(int i);
  void set(int idx, T _info, int i, int L, int R);
  void set(int idx, T _info);
  T rangeQuery(int qL, int qR, int i, int L, int R);
  T rangeQuery(int qL, int qR);

private:
  int n;
  std::vector<T> info;
};

#endif // SEGMENT_TREE_HPP