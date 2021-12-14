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

  string line;

  map<pair<char, char>, int64_t> p;

  getline(cin, line);
  char last = line.back();

  for (auto it = next(line.begin()); it != line.end(); ++it) {
    ++p[{ *prev(it), *it }];
  }

  getline(cin, line);

  map<pair<char, char>, char> m;
  while (getline(cin, line)) {
    auto w = split<string_view>(line, " ->");
    m[{ w.front().front(), w.front().back() }] = w.back().front();
  }

  auto Step = [&m](const auto& p) {
    map<pair<char, char>, int64_t> r = p;
    for (auto [x, k] : p) {
      auto [c1, c2] = x;
      if (auto it = m.find({ c1, c2 }); it != m.end()) {
        r[{ c1, c2 }] -= k;
        r[{ c1, it->second }] += k;
        r[{ it->second, c2 }] += k;
      }
    }
    return r;
  };

  auto Count = [last](const auto& p) {
    unordered_map<char, int64_t> c;
    for (auto [p, k] : p) {
      c[p.first] += k;
    }
    ++c[last];
    auto [mn, mx] = minmax_element(c.begin(), c.end(), [](auto p1, auto p2) {
      return p1.second < p2.second;
    });
    return mx->second - mn->second;
  };

  int k = 1;
  for (; k <= 10; ++k) {
    p = Step(p);
  }
  cout << Count(p) << endl;

  for (; k <= 40; ++k) {
    p = Step(p);
  }
  cout << Count(p) << endl;

  return 0;
  return 0;
}
