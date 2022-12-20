
#include "xxx.h"

using namespace std;
using namespace x;

int
main()
{
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  vector<int> nums;
  int x;
  size_t zero{};
  while (cin >> x) {
    if (x == 0)
      zero = nums.size();
    nums.push_back(x);
  }
  const auto n = nums.size();
  vector<size_t> next(n);
  vector<size_t> prev(n);

  auto Init = [&]() {
    for (size_t i{ 1 }; i < n; ++i) {
      prev[i] = i - 1;
      next[i - 1] = i;
    }
    prev[0] = n - 1;
    next[n - 1] = 0;
  };

  auto Move = [&prev, &next, n, &nums](size_t p, long m = 1) {
    long x = nums[p] * m;
    auto q = p;
    if (x == 0)
      return;
    if (x > 0) {
      auto k = x % static_cast<long>(n - 1);
      for (auto j{ 0 }; j < k; ++j) {
        q = next[q];
      }
    } else {
      auto k = (-x) % static_cast<long>(n - 1);
      for (auto j{ 0 }; j <= k; ++j) {
        q = prev[q];
      }
    }

    next[prev[p]] = next[p];
    prev[next[p]] = prev[p];
    prev[next[q]] = p;
    next[p] = next[q];
    next[q] = p;
    prev[p] = q;
  };

  auto Sum = [&nums, &next](size_t p, long m = 1) {
    long sum{};
    for (auto i{ 1 }; i <= 3000; ++i) {
      p = next[p];
      if (i % 1000 == 0) {
        sum += nums[p] * m;
      }
    }
    return sum;
  };

  Init();
  for (auto i{ 0uz }; i < n; ++i)
    Move(i);
  cout << Sum(zero) << endl;

  const long key = 811589153;

  Init();
  for (auto t{ 0 }; t < 10; ++t)
    for (auto i{ 0uz }; i < n; ++i)
      Move(i, key);
  cout << Sum(zero, key) << endl;

  return 0;
}
