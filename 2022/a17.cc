#include "xxx.h"
#include <bitset>
#include <sstream>

using namespace std;
using namespace x;

int
main()
{
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  string pattern;
  getline(cin, pattern);

  array<vector<pair<int, int>>, 5> figures{
    vector{ pair{ 0, 0 }, { 1, 0 }, { 2, 0 }, { 3, 0 } },
    vector{ pair{ 1, 0 }, { 0, 1 }, { 1, 1 }, { 2, 1 }, { 1, 2 } },
    vector{ pair{ 0, 0 }, { 1, 0 }, { 2, 0 }, { 2, 1 }, { 2, 2 } },
    vector{ pair{ 0, 0 }, { 0, 1 }, { 0, 2 }, { 0, 3 } },
    vector{ pair{ 0, 0 }, { 1, 0 }, { 0, 1 }, { 1, 1 } },
  };

  auto Simulate = [&](uint64_t m) {
    auto Next = [current{ pattern.cbegin() }, &pattern]() mutable {
      auto c = *current++;
      if (current == pattern.cend())
        current = pattern.cbegin();
      return c;
    };

    constexpr int N{ 7 };
    vector<array<bool, N>> tower;

    auto Check = [&](auto x, auto y, const auto& f) {
      for (auto [dx, dy] : f) {
        if (x + dx < 0 || x + dx >= N || y + dy < 0)
          return false;
        if (y + dy < static_cast<int>(tower.size()) && tower[y + dy][x + dx])
          return false;
      }
      return true;
    };

    auto Put = [&](auto x, auto y, const auto& f) {
      for (auto [dx, dy] : f) {
        if (y + dy >= static_cast<int>(tower.size()))
          tower.push_back({});
        tower[y + dy][x + dx] = true;
      }
    };

    uint64_t r{};
    unordered_map<bitset<192>, pair<uint64_t, size_t>> s;
    for (uint64_t i{}; i < m; ++i) {
      if (r == 0 && i > 0 && i % (figures.size() * pattern.size()) == 0) {
        bitset<192> h{};
        for (int i{}; i < 24; ++i) {
          for (int j{}; j < N; ++j) {
            if (tower[tower.size() - i - 1][j]) {
              h.set(8 * i + j);
            }
          }
        }
        if (s.contains(h)) {
          auto [pi, ps] = s[h];
          auto k = (m - i) / (i - pi);
          r = (tower.size() - ps) * k;
          i += (i - pi) * k;
        } else {
          s[h] = { i, tower.size() };
        }
      }

      const auto& figure{ figures[i % figures.size()] };
      int x{ 2 }, y{ static_cast<int>(tower.size()) + 3 };
      while (true) {
        switch (Next()) {
          case '<':
            if (Check(x - 1, y, figure))
              --x;
            break;
          case '>':
            if (Check(x + 1, y, figure))
              ++x;
            break;
        }
        if (!Check(x, y - 1, figure))
          break;
        --y;
      }
      Put(x, y, figure);
    }
    return tower.size() + r;
  };

  cout << Simulate(2022) << endl;
  cout << Simulate(1'000'000'000'000) << endl;

  return 0;
}
