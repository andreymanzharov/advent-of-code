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

vector<string_view>
split(string_view sv, string_view delimiters = ", "sv)
{
  vector<string_view> parts;
  while (!sv.empty()) {
    auto p = sv.find_first_of(delimiters);
    if (p > 0)
      parts.push_back(sv.substr(0, p));
    if (p == string_view::npos)
      break;
    sv.remove_prefix(p + 1);
  }
  return parts;
}
}

int
main()
{
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  string line;
  getline(cin, line);
  getline(cin, line);

  int gx = 0;
  int gy = 0;

  map<pair<int, int>, pair<int, int>> grid;
  while (getline(cin, line)) {
    auto w = x::split(line, " -xyT%");
    int x = x::to<int>(w[1]);
    int y = x::to<int>(w[2]);
    grid[{ x, y }] = { x::to<int>(w[3]), x::to<int>(w[4]) };
    if (y == 0) {
      gx = max(gx, x);
    }
  }

  int r = 0;
  for (auto& [xy1, a] : grid) {
    for (auto& [xy2, b] : grid) {
      if (xy1 == xy2)
        continue;
      auto used = get<1>(a);
      auto avail = get<0>(b) - get<1>(b);
      if (used > 0 && used <= avail) {
        r += 1;
      }
    }
  }
  cout << r << endl;

  using Grid = map<pair<int, int>, pair<int, int>>;

  auto Check = [](Grid g, int& t, int gx, int gy) {
    while (gx != 0 || gy != 0) {
      auto& [empty_size, empty_used] = g[{ gx - 1, gy }];
      auto& [goal_size, goal_used] = g[{ gx, gy }];
      assert(empty_used == 0);
      if (empty_size < goal_used)
        return false;
      gx -= 1;
      swap(empty_used, goal_used);
      t++;
      if (gx == 0 && gy == 0)
        break;

      if (g.at({ gx + 1, gy }).first < g.at({ gx + 1, gy + 1 }).second)
        return false;
      swap(g[{ gx + 1, gy }].second, g[{ gx + 1, gy + 1 }].second);
      if (g.at({ gx + 1, gy + 1 }).first < g.at({ gx, gy + 1 }).second)
        return false;
      swap(g[{ gx + 1, gy + 1 }].second, g[{ gx, gy + 1 }].second);
      if (g.at({ gx, gy + 1 }).first < g.at({ gx - 1, gy + 1 }).second)
        return false;
      swap(g[{ gx, gy + 1 }].second, g[{ gx - 1, gy + 1 }].second);
      if (g.at({ gx - 1, gy + 1 }).first < g.at({ gx - 1, gy }).second)
        return false;
      swap(g[{ gx - 1, gy + 1 }].second, g[{ gx - 1, gy }].second);
      t += 4;
    }
    return true;
  };

  deque<tuple<int, pair<int, int>, Grid>> q;
  set<pair<int, int>> s;
  for (auto [xy, node] : grid) {
    auto [size, used] = node;
    if (used == 0) {
      q.emplace_back(0, xy, grid);
      s.insert(xy);
    }
  }
  while (!q.empty()) {
    const auto [t, xy, grid] = move(q.front());
    q.pop_front();
    if (xy == pair{ gx - 1, gy }) {
      int r = t;
      if (Check(grid, r, gx, gy)) {
        cout << r << endl;
        break;
      }
    }
    auto [x, y] = xy;
    auto [size, used] = grid.at(xy);
    assert(used == 0);
    for (auto [dx, dy] : { pair{ -1, 0 }, { 0, -1 }, { 1, 0 }, { 0, 1 } }) {
      pair<int, int> nxy = { x + dx, y + dy };
      if (s.find(nxy) != s.end())
        continue;
      if (auto it = grid.find(nxy); it != grid.end()) {
        auto [nsize, nused] = it->second;
        if (nused <= size) {
          auto ngrid = grid;
          ngrid[xy] = { size, nused };
          ngrid[nxy] = { nsize, 0 };
          q.emplace_back(t + 1, nxy, move(ngrid));
          s.insert(nxy);
        }
      }
    }
  }
}
