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

  set<pair<int, int>> east, south;

  int mxr, mxc;

  string line;
  for (int r{ 0 }; getline(cin, line); ++r) {
    for (int c{ 0 }; c < static_cast<int>(line.size()); ++c) {
      if (line[c] == '>')
        east.emplace(r, c);
      else if (line[c] == 'v')
        south.emplace(r, c);
      mxc = c + 1;
    }
    mxr = r + 1;
  }

  bool moved = true;

  auto Move = [&](auto s, const auto& t, int dr, int dc) {
    set<pair<int, int>> x;
    for (auto [r, c] : s) {
      auto nr = (r + dr) % mxr;
      auto nc = (c + dc) % mxc;
      if (!s.contains({ nr, nc }) && !t.contains({ nr, nc })) {
        moved = true;
        x.emplace(nr, nc);
      } else {
        x.emplace(r, c);
      }
    }
    return x;
  };

  int step = 0;
  while (moved) {
    ++step;
    moved = false;
    east = Move(move(east), south, 0, 1);
    south = Move(move(south), east, 1, 0);
  }

  cout << step << endl;

  return 0;
}
