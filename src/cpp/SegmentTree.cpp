#include "SegmentTree.hpp"

template <typename T>
SegmentTree<T>::SegmentTree(int n) {
  init(n);
}

template <typename T>
void SegmentTree<T>::init(int n) {
  this->n = n;
  info.assign(4 * n, Info());
}

template <typename T>
void SegmentTree<T>::pull(int i) {
  info[i] = info[2 * i] + info[2 * i + 1];
}

template <typename T>
void SegmentTree<T>::set(int idx, T _info, int i, int L, int R) {
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

template <typename T>
void SegmentTree<T>::set(int idx, T _info) {
  set(idx, _info, 1, 0, n - 1);
}

template <typename T>
T SegmentTree<T>::rangeQuery(int qL, int qR, int i, int L, int R) {
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

template <typename T>
T SegmentTree<T>::rangeQuery(int qL, int qR) {
  return rangeQuery(qL, qR, 1, 0, n - 1);
}

template class SegmentTree<Info>;