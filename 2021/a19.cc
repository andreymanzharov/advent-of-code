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

  using Pos = array<int, 3>;

  vector<vector<Pos>> initial;

  string line;
  while (getline(cin, line)) {
    initial.emplace_back();
    while (getline(cin, line) && !line.empty()) {
      vector<int> xyz = split<int>(line);
      initial.back().push_back({ xyz[0], xyz[1], xyz[2] });
    }
  }

  vector<vector<vector<Pos>>> cache(initial.size());
  auto Orientations = [&](size_t index) -> const vector<vector<Pos>>& {
    if (!cache[index].empty())
      return cache[index];
    auto& r = cache[index];
    Pos a{ 0, 1, 2 };
    do {
      for (int k : { -1, 1 }) {
        vector<array<int, 3>> r0, r90, r180, r270;
        for (const auto& p : initial[index]) {
          r0.push_back({ p[a[0]], p[a[1]], k * p[a[2]] });
          r90.push_back({ p[a[1]], -p[a[0]], k * p[a[2]] });
          r180.push_back({ -p[a[0]], -p[a[1]], k * p[a[2]] });
          r270.push_back({ -p[a[1]], p[a[0]], k * p[a[2]] });
        }
        r.push_back(move(r0));
        r.push_back(move(r90));
        r.push_back(move(r180));
        r.push_back(move(r270));
      }
    } while (next_permutation(a.begin(), a.end()));
    return r;
  };

  struct Scanner
  {
    array<int, 3> pos;
    vector<Pos> beacons;
    bool detected = false;
  };

  vector<Scanner> scanners(initial.size());

  queue<size_t> q;
  scanners[0] = { .pos = array{ 0, 0, 0 },
                  .beacons = move(initial[0]),
                  .detected = true };
  q.push(0);

  while (!q.empty()) {
    auto i = q.front();
    q.pop();

    const auto& scanner_i = scanners[i];
    auto [six, siy, siz] = scanner_i.pos;

    for (auto j = 1uz; j < initial.size(); ++j) {
      auto& scanner_j = scanners[j];
      if (scanner_j.detected)
        continue;
      for (const auto& orientation : Orientations(j)) {
        map<Pos, int> count;
        for (const auto [pix, piy, piz] : scanner_i.beacons) {
          auto vx = pix + six, vy = piy + siy, vz = piz + siz;
          for (const auto [px, py, pz] : orientation) {
            ++count[Pos{ vx - px, vy - py, vz - pz }];
          }
        }
        if (auto mx = max_element(
              count.begin(),
              count.end(),
              [](auto& p1, auto& p2) { return p1.second < p2.second; });
            mx->second >= 12) {
          scanners[j] = { .pos = mx->first,
                          .beacons = orientation,
                          .detected = true };
          q.push(j);
          break;
        }
      }
    }
  }

  set<Pos> beacons;
  for (const auto& scanner : scanners) {
    const auto [sx, sy, sz] = scanner.pos;
    for (const auto [x, y, z] : scanner.beacons) {
      beacons.insert(array{ sx + x, sy + y, sz + z });
    }
  }

  cout << beacons.size() << endl;

  int largest_distance = numeric_limits<int>::min();
  for (const auto& scanner1 : scanners) {
    const auto [s1x, s1y, s1z] = scanner1.pos;
    for (const auto& scanner2 : scanners) {
      const auto [s2x, s2y, s2z] = scanner2.pos;
      largest_distance =
        max(largest_distance, abs(s1x - s2x) + abs(s1y - s2y) + abs(s1z - s2z));
    }
  }
  cout << largest_distance << endl;

  return 0;
}
