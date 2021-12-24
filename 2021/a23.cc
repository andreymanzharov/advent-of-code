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

  set<pair<int, int>> g;
  map<pair<int, int>, char, greater<pair<int, int>>> p;

  string line;
  for (int r{ 0 }; getline(cin, line); ++r) {
    for (int c{ 0 }; c < static_cast<int>(line.size()); ++c) {
      if (line[c] == '#')
        g.emplace(r, c);
      else if ('A' <= line[c] && line[c] <= 'D')
        p.emplace(pair{ r, c }, line[c]);
    }
  }

  using State =
    tuple<int, int, map<pair<int, int>, char, greater<pair<int, int>>>>;

  map<map<pair<int, int>, char, greater<pair<int, int>>>, int> s;

  auto Organized = [](const auto& amphipods) {
    for (int r = 2; r <= 5; ++r) {
      for (int a = 'A'; a <= 'D'; ++a) {
        int c = 3 + (a - 'A') * 2;
        if (auto it = amphipods.find({ r, c });
            it == amphipods.end() || it->second != a)
          return false;
      }
    }
    return true;
  };

  priority_queue<State, vector<State>, greater<State>> q;
  q.emplace(0, 0, p);

  constexpr array<int, 4> energies{ 1, 10, 100, 1000 };

  auto Move = [&](int energy, const auto& amphipods, int r, int c, int k) {
    auto a = amphipods.at({ r, c });
    auto move_energy = energies[a - 'A'];

    int mnc = c;
    while (!g.contains({ 1, mnc - 1 }) && !amphipods.contains({ 1, mnc - 1 }))
      mnc--;
    int mxc = c;
    while (!g.contains({ 1, mxc + 1 }) && !amphipods.contains({ 1, mxc + 1 }))
      mxc++;

    if ((r == 5 && !amphipods.contains({ r - 1, c }) &&
         !amphipods.contains({ r - 2, c }) &&
         !amphipods.contains({ r - 3, c })) ||
        (r == 4 && !amphipods.contains({ r - 1, c }) &&
         !amphipods.contains({ r - 2, c })) ||
        (r == 3 && !amphipods.contains({ r - 1, c })) || r == 2) {
      int nr = 1;
      for (int nc = mnc; nc <= mxc; ++nc) {
        if (!g.contains({ nr + 1, nc }))
          continue;
        auto next_amphipods = amphipods;
        auto node = next_amphipods.extract({ r, c });
        node.key() = { nr, nc };
        next_amphipods.insert(move(node));
        auto e = energy + move_energy * (abs(r - nr) + abs(c - nc));
        q.emplace(k + 1, e, move(next_amphipods));
      }
    } else if (r == 1) {
      for (int nc = mnc; nc <= mxc; ++nc) {
        if (g.contains({ r + 1, nc }))
          continue;
        int nr = r + 4;
        bool f = nc == 3 + (a - 'A') * 2,
             r2 = amphipods.contains({ r + 1, nc }),
             r3 = amphipods.contains({ r + 2, nc }),
             r4 = amphipods.contains({ r + 3, nc }),
             r5 = amphipods.contains({ r + 4, nc });
        if (f && !r5 && !r4 && !r3 && !r2) {
          auto next_amphipods = amphipods;
          auto node = next_amphipods.extract({ r, c });
          node.key() = { nr, nc };
          next_amphipods.insert(move(node));
          auto e = energy + move_energy * (abs(r - nr) + abs(c - nc));
          q.emplace(k + 1, e, move(next_amphipods));
        }
        nr--;
        if (f && r5 && !r4 && !r3 && !r2) {
          auto next_amphipods = amphipods;
          auto node = next_amphipods.extract({ r, c });
          node.key() = { nr, nc };
          next_amphipods.insert(move(node));
          auto e = energy + move_energy * (abs(r - nr) + abs(c - nc));
          q.emplace(k + 1, e, move(next_amphipods));
        }
        nr--;
        if (f && r5 && r4 && !r3 && !r2) {
          auto next_amphipods = amphipods;
          auto node = next_amphipods.extract({ r, c });
          node.key() = { nr, nc };
          next_amphipods.insert(move(node));
          auto e = energy + move_energy * (abs(r - nr) + abs(c - nc));
          q.emplace(k + 1, e, move(next_amphipods));
        }
        nr--;
        if (f && r5 && r4 && r3 && !r2) {
          auto next_amphipods = amphipods;
          auto node = next_amphipods.extract({ r, c });
          node.key() = { nr, nc };
          next_amphipods.insert(move(node));
          auto e = energy + move_energy * (abs(r - nr) + abs(c - nc));
          q.emplace(k + 1, e, move(next_amphipods));
        }
      }
    }
  };

  while (!q.empty()) {
    const auto [k, energy, amphipods] = q.top();
    q.pop();
    if (auto [it, f] = s.emplace(amphipods, energy); !f) {
      continue;
    }

    if (Organized(amphipods)) {
      cout << energy << endl;
      break;
    }
    for (auto [p, a] : amphipods) {
      auto [r, c] = p;
      if (c == 3 + (a - 'A') * 2 && r > 1) {
        bool f = true;
        for (int nr = r; f && nr <= 5; ++nr) {
          if (auto it = amphipods.find({ nr, c });
              it == amphipods.end() || it->second != a)
            f = false;
        }
        if (f)
          continue;
      }

      Move(energy, amphipods, r, c, k);
    }
  }

  return 0;
}
