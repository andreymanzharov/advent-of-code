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
#include <variant>
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

  string s;
  getline(cin, s);

  set<pair<int, int>> g;

  string line;
  getline(cin, line);
  int mxr = 0, mxc = 0;
  for (int i = 0; getline(cin, line); ++i) {
    for (int j = 0; j < static_cast<int>(line.size()); ++j) {
      if (line[j] == '#') {
        g.emplace(i, j);
      }
      mxc = j;
    }
    mxr = i;
  }

  auto Apply = [&](set<pair<int, int>> g, int k, bool outside) {
    set<pair<int, int>> h;
    for (int i = -k; i <= mxr + k; ++i)
      for (int j = -k; j <= mxc + k; ++j) {
        auto index = 0uz;
        for (int r = i - 1; r <= i + 1; ++r) {
          for (int c = j - 1; c <= j + 1; ++c) {
            index =
              index * 2 + (r <= -k || r >= mxr + k || c <= -k || c >= mxc + k
                             ? outside
                             : g.contains({ r, c }));
          }
        }
        if (s[index] == '#')
          h.emplace(i, j);
      }

    return h;
  };

  bool outside = false;
  for (int k = 1; k <= 50; ++k) {
    g = Apply(std::move(g), k, outside);
    outside = outside ? s[(1 << 9) - 1] == '#' : s[0] == '#';

    if (k == 2 || k == 50) {
      assert(!outside);
      cout << g.size() << endl;
    }
  }

  return 0;
}
