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
  for (string line; getline(cin, line);) {
    g.push_back(std::move(line));
  }

  const array<pair<int, int>, 4> d{
    pair{ -1, 0 }, { 0, -1 }, { 1, 0 }, { 0, 1 }
  };

  auto Bounds = [&](int i, int j) {
    return 0 <= i && i < (int)g.size() && 0 <= j && j < (int)g[i].size();
  };

  auto Size = [&](int i, int j) {
    int s = 0;
    deque<pair<int, int>> q;
    q.emplace_back(i, j);
    while (!q.empty()) {
      auto [i, j] = q.front();
      q.pop_front();
      for (auto [di, dj] : d) {
        int ni = i + di, nj = j + dj;
        if (Bounds(ni, nj) && g[ni][nj] < '9') {
          g[ni][nj] = '9';
          q.emplace_back(ni, nj);
          s++;
        }
      }
    }
    return s;
  };

  priority_queue<int> x;

  int a = 0;
  for (int i = 0; i < (int)g.size(); ++i) {
    const auto& r = g[i];
    for (int j = 0; j < (int)r.size(); ++j) {
      if (all_of(d.begin(), d.end(), [&](const auto x) {
            auto [di, dj] = x;
            auto ni = i + di, nj = j + dj;
            return !Bounds(ni, nj) || g[ni][nj] > r[j];
          })) {
        a += r[j] - '0' + 1;
        x.push(Size(i, j));
      }
    }
  }
  int b = 1;
  for (int i = 0; i < 3 && !x.empty(); ++i) {
    b *= x.top();
    x.pop();
  }

  cout << a << endl;
  cout << b << endl;

  return 0;
}
