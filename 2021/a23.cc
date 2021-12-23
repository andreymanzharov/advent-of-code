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
  map<pair<int, int>, char> p;

  string line;
  for (int r{ 0 }; getline(cin, line); ++r) {
    for (int c{ 0 }; c < static_cast<int>(line.size()); ++c) {
      if (line[c] == '#')
        g.emplace(r, c);
      else if ('A' <= line[c] && line[c] <= 'D')
        p.emplace(pair{ r, c }, line[c]);
    }
  }

  using State = tuple<int, map<pair<int, int>, char>>;

  set<map<pair<int, int>, char>> s;

  auto Organized = [](const auto& amphipods) {
    for (int r = 2; r <= 3; ++r) {
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
  // queue<State> q;
  q.emplace(0, p);

  constexpr array<int, 4> energies{ 1, 10, 100, 1000 };

  auto Move = [&](int energy, const auto& amphipods, int r, int c) {
    auto move_energy = energies[amphipods.at({ r, c }) - 'A'];

    int mnc = c;
    while (!g.contains({ 1, mnc - 1 }) && !amphipods.contains({ 1, mnc - 1 }))
      mnc--;
    int mxc = c;
    while (!g.contains({ 1, mxc + 1 }) && !amphipods.contains({ 1, mxc + 1 }))
      mxc++;

    if ((r == 3 && !amphipods.contains({ r - 1, c })) || r == 2) {
      int nr = 1;
      for (int nc = mnc; nc <= mxc; ++nc) {
        if (!g.contains({ nr + 1, nc }))
          continue;
        auto next_amphipods = amphipods;
        auto node = next_amphipods.extract({ r, c });
        node.key() = { nr, nc };
        next_amphipods.insert(move(node));
        // if (auto [it, f] = s.insert(next_amphipods); f) {
        q.emplace(energy + move_energy * (abs(r - nr) + abs(c - nc)),
                  move(next_amphipods));
        // }
      }
    } else if (r == 1) {
      if (energy == 12519 && r == 1 && c == 2) {
        dbg(energy);
      }
      for (int nc = mnc; nc <= mxc; ++nc) {
        // if (g.contains({ r + 1, nc }))
        //   continue;
        int nr = r + 1;
        if (!amphipods.contains({ nr, nc })) {
          auto next_amphipods = amphipods;
          auto node = next_amphipods.extract({ r, c });
          node.key() = { nr, nc };
          next_amphipods.insert(move(node));
          // if (auto [it, f] = s.insert(next_amphipods); f) {
          q.emplace(energy + move_energy * (abs(r - nr) + abs(c - nc)),
                    move(next_amphipods));
          // }
          nr++;
          if (!amphipods.contains({ nr, nc })) {
            auto next_amphipods = amphipods;
            auto node = next_amphipods.extract({ r, c });
            node.key() = { nr, nc };
            next_amphipods.insert(move(node));
            // if (auto [it, f] = s.insert(next_amphipods); f) {
            q.emplace(energy + move_energy * (abs(r - nr) + abs(c - nc)),
                      move(next_amphipods));
            // }
          }
        }
      }
    }
  };

  while (!q.empty()) {
    const auto [energy, amphipods] = q.top();
    // dbg(energy, s.size(), q.size());
    // for (int r = 0; r < 5; ++r) {
    //   for (int c = 0; c < 13; ++c) {
    //     if (auto it = amphipods.find({ r, c }); it != amphipods.end()) {
    //       cout << it->second;
    //     } else if (g.contains({ r, c })) {
    //       cout << '#';
    //     } else {
    //       cout << '.';
    //     }
    //   }
    //   cout << '\n';
    // }
    // cout << energy << endl;
    q.pop();
    if (auto [it, f] = s.insert(amphipods); !f) {
      continue;
    }

    if (Organized(amphipods)) {
      cout << energy << endl;
      break;
    }
    for (auto [p, a] : amphipods) {
      auto [r, c] = p;
      if (c == 3 + (a - 'A') * 2 && r == 3)
        continue;
      if (c == 3 + (a - 'A') * 2 && r == 2)
        if (auto it = amphipods.find({ r + 1, c });
            it != amphipods.end() && it->second == a)
          continue;
      Move(energy, amphipods, r, c);
    }
  }

  return 0;
}
