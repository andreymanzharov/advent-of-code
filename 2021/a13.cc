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

  set<pair<int, int>> s;

  string line;
  while (getline(cin, line) && !line.empty()) {
    auto w = split<int>(line);
    s.emplace(w[0], w[1]);
  }

  auto Fold = [](const auto& s, int fx, int fy) {
    set<pair<int, int>> t;
    for (auto [x, y] : s) {
      if (fx > 0 && x > fx) {
        x = fx - (x - fx);
      } else if (fy > 0 && y > fy) {
        y = fy - (y - fy);
      }
      t.emplace(x, y);
    }
    return t;
  };

  bool first = true;
  while (getline(cin, line)) {
    auto fold = split<string_view>(line, "=");
    int x = 0, y = 0;
    if (fold[0].back() == 'x') {
      x = to<int>(fold[1]);
    } else {
      y = to<int>(fold[1]);
    }
    s = Fold(s, x, y);
    if (first) {
      cout << s.size() << endl;
      first = false;
    }
  }
  auto [mnx, mxx] =
    minmax_element(s.begin(), s.end(), [](const auto& p1, const auto& p2) {
      return p1.first < p2.first;
    });
  auto [mny, mxy] =
    minmax_element(s.begin(), s.end(), [](const auto& p1, const auto& p2) {
      return p1.second < p2.second;
    });

  for (int y = mny->second; y <= mxy->second; ++y) {
    for (int x = mnx->first; x <= mxx->first; ++x) {
      cout << (s.contains({ x, y }) ? '#' : ' ');
    }
    cout << endl;
  }

  return 0;
}
