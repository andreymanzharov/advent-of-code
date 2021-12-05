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
} // namespace x

using namespace x;

int
main()
{
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  vector<tuple<int, int, int, int>> a;
  string line;
  while (getline(cin, line)) {
    auto w = split(line, ", ->"sv);
    a.emplace_back(to<int>(w[0]), to<int>(w[1]), to<int>(w[2]), to<int>(w[3]));
  }

  auto CountOverlaps = [](const auto& a, bool diagonal) {
    map<pair<int, int>, int> g;
    int result = 0;
    for (const auto& [x1, y1, x2, y2] : a) {
      if (!diagonal && x1 != x2 && y1 != y2)
        continue;
      int dx = (x1 < x2) - (x1 > x2);
      int dy = (y1 < y2) - (y1 > y2);
      for (int x = x1, y = y1; x != x2 + dx || y != y2 + dy; x += dx, y += dy) {
        int& c = g[{ x, y }];
        if (c++ == 1) {
          result++;
        }
      }
    }
    return result;
  };

  cout << CountOverlaps(a, false) << endl;
  cout << CountOverlaps(a, true) << endl;

  return 0;
}
