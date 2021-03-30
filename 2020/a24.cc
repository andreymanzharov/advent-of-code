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

  set<tuple<int, int, int>> grid;

  string line;
  while (getline(cin, line)) {
    tuple p = { 0, 0, 0 };
    string_view sv = line;
    while (!sv.empty()) {
      auto [x, y, z] = p;
      switch (sv.front()) {
        case 'e':
          p = { x - 1, y + 1, z };
          break;
        case 'w':
          p = { x + 1, y - 1, z };
          break;
        case 'n':
          sv.remove_prefix(1);
          if (sv.front() == 'e')
            p = { x, y + 1, z - 1 };
          else
            p = { x + 1, y, z - 1 };
          break;
        case 's':
          sv.remove_prefix(1);
          if (sv.front() == 'e')
            p = { x - 1, y, z + 1 };
          else
            p = { x, y - 1, z + 1 };
          break;
      }
      sv.remove_prefix(1);
    }
    if (const auto it = grid.find(p); it != end(grid)) {
      grid.erase(it);
    } else {
      grid.insert(p);
    }
  }
  cout << grid.size() << endl;

  const set<tuple<int, int, int>> d = { { 0, 1, -1 }, { -1, 1, 0 },
                                        { -1, 0, 1 }, { 0, -1, 1 },
                                        { 1, -1, 0 }, { 1, 0, -1 } };

  for (int k = 0; k < 100; ++k) {
    set<tuple<int, int, int>> nbrs;
    for (auto [x, y, z] : grid) {
      nbrs.insert({ x, y, z });
      for (auto [dx, dy, dz] : d) {
        nbrs.insert({ x + dx, y + dy, z + dz });
      }
    }

    set<tuple<int, int, int>> next_grid;

    for (auto [x, y, z] : nbrs) {
      int n = 0;
      for (auto [dx, dy, dz] : d) {
        if (grid.find({ x + dx, y + dy, z + dz }) != end(grid)) {
          ++n;
        }
      }
      if (grid.find({ x, y, z }) != end(grid) && (n == 1 || n == 2)) {
        next_grid.insert({ x, y, z });
      } else if (n == 2) {
        next_grid.insert({ x, y, z });
      }
    }

    grid = move(next_grid);
  }

  cout << grid.size() << endl;
}
