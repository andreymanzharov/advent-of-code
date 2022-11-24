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

  vector<string> g;

  string line;
  while (getline(cin, line)) {
    g.push_back(std::move(line));
  }
  int n = g.size();
  int m = g[0].size();

  auto Risk = [&](int n, int m, auto risk) {
    set<pair<int, int>> d;
    priority_queue<tuple<int, int, int>,
                   vector<tuple<int, int, int>>,
                   greater<tuple<int, int, int>>>
      q;
    q.emplace(0, 0, 0);
    while (!q.empty()) {
      auto [x, i, j] = q.top();
      q.pop();
      if (auto [_, inserted] = d.insert({ i, j }); !inserted)
        continue;
      if (i == n - 1 && j == m - 1) {
        return x;
      }
      for (auto [di, dj] : { pair{ -1, 0 }, { 0, 1 }, { 1, 0 }, { 0, -1 } }) {
        int ni = i + di, nj = j + dj;
        if (0 <= ni && ni < n && 0 <= nj && nj < m && !d.contains({ ni, nj })) {
          q.emplace(x + risk(ni, nj), ni, nj);
        }
      }
    }
    return 0;
  };

  cout << Risk(n, m, [&](int i, int j) { return g[i][j] - '0'; }) << endl;
  cout << Risk(5 * n, 5 * m, [&](int i, int j) {
    return (g[i % n][j % m] - '0' + i / n + j / m - 1) % 9 + 1;
  });

  return 0;
}
