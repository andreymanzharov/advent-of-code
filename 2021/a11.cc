#define DBG_MACRO_NO_WARNING
#include "dbg.h"

#include <algorithm>
#include <array>
#include <cassert>
#include <cctype>
#include <charconv>
#include <cmath>
#include <deque>
#include <iostream>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <sstream>
#include <string>
#include <string_view>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;

namespace x {

template<typename T>
T
to(string_view from)
{
  T r{};
  if (auto [p, ec] = from_chars(from.data(), from.data() + from.size(), r);
      p != from.data() + from.size()) {
    ostringstream message;
    message << "cannot convert " << from;
    throw invalid_argument(message.str());
  }
  return r;
}

template<>
inline string_view
to(string_view from)
{
  return from;
}

template<>
inline string
to(string_view from)
{
  return string{ from };
}

template<typename T>
vector<T>
split(string_view sv, string_view delimiters = ", "sv)
{
  vector<T> parts;
  while (!sv.empty()) {
    auto p = sv.find_first_of(delimiters);
    if (p > 0)
      parts.push_back(to<T>(sv.substr(0, p)));
    if (p == string_view::npos)
      break;
    sv.remove_prefix(p + 1);
  }
  return parts;
}
} // namespace x

using namespace x;

int
main()
{
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  vector<vector<int>> g;

  string line;
  while (getline(cin, line)) {
    transform(line.begin(),
              line.end(),
              back_inserter(g.emplace_back()),
              [](char c) { return c - '0'; });
  }

  int a = 0;
  int b = 0;

  auto Step = [&]() {
    vector<vector<int>> f(g.size(), vector<int>(g.size(), false));
    for (int i = 0; i < 10; ++i) {
      auto& r = g[i];
      for (int j = 0; j < 10; ++j) {
        if (f[i][j] == 1)
          continue;
        if (++r[j] > 9) {
          queue<pair<int, int>> q;
          q.emplace(i, j);
          while (!q.empty()) {
            auto [i, j] = q.front();
            q.pop();
            if (g[i][j] <= 9)
              continue;
            ++a;
            g[i][j] = 0;
            f[i][j] = 1;
            for (int ni = max(0, i - 1); ni <= min(i + 1, 9); ++ni) {
              for (int nj = max(0, j - 1); nj <= min(j + 1, 9); ++nj) {
                if (f[ni][nj] == 0) {
                  g[ni][nj]++;
                  q.emplace(ni, nj);
                }
              }
            }
          }
        }
      }
    }
    return all_of(f.begin(), f.end(), [](const auto& r) {
      return all_of(r.begin(), r.end(), [](int x) { return x == 1; });
    });
  };

  int k;
  for (k = 1; k <= 100; ++k) {
    Step();
  }
  cout << a << endl;

  for (b = k; !Step(); ++b)
    ;
  cout << b << endl;

  return 0;
}
