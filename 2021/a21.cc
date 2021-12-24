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

  struct Player
  {
    int n, p;
  };

  struct State
  {
    Player p1, p2;
    bool first;

    bool operator<(const State& other) const
    {
      return tuple{ p1.p, p1.n, p2.p, p2.n, first } < tuple{
        other.p1.p, other.p1.n, other.p2.p, other.p2.n, other.first
      };
    }
  };

  auto Step = [](auto s, int x) {
    auto t{ s };
    if (s.first) {
      t.p1.p = (t.p1.p + x - 1) % 10 + 1;
      t.p1.n += t.p1.p;
    } else {
      t.p2.p = (t.p2.p + x - 1) % 10 + 1;
      t.p2.n += t.p2.p;
    }
    t.first = !s.first;
    return t;
  };

  map<State, pair<long, long>> d;
  function<pair<long, long>(State)> WinCount = [&](State s) {
    if (s.p1.n >= 21)
      return pair{ 1L, 0L };
    if (s.p2.n >= 21)
      return pair{ 0L, 1L };
    pair<long, long> r{ 0, 0 };
    for (int a = 1; a <= 3; ++a) {
      for (int b = 1; b <= 3; ++b) {
        for (int c = 1; c <= 3; ++c) {
          auto t = Step(s, a + b + c);
          long w1, w2;
          if (auto it = d.find(t); it != d.end()) {
            w1 = it->second.first;
            w2 = it->second.second;
          } else {
            auto p = WinCount(t);
            d.emplace(t, p);
            w1 = p.first;
            w2 = p.second;
          }
          r.first += w1;
          r.second += w2;
        }
      }
    }
    return r;
  };

  array<Player, 2> players;

  string line;
  while (getline(cin, line)) {
    auto w = split<string_view>(line);
    players[to<size_t>(w[1]) - 1] = Player{ .n = 0, .p = to<int>(w[4]) };
  }

  State s{ players[0], players[1], true };
  int i{ 0 };
  for (int d = 1; s.p1.n < 1000 && s.p2.n < 1000;) {
    int x{ 0 };
    for (int k{ 0 }; k < 3; ++k) {
      x += d;
      ++i;
      if (++d > 100)
        d = 1;
    }
    s = Step(s, x);
  }
  cout << i * min(s.p1.n, s.p2.n) << endl;

  auto [w1, w2] = WinCount({ players[0], players[1], true });

  cout << max(w1, w2) << endl;

  return 0;
}
