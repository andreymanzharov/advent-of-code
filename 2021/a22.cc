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

  struct Range
  {
    int64_t lo, hi;
  };

  struct Cube
  {
    Range x, y, z;
  };

  vector<Cube> cubes;

  auto NoIntersect = [](const auto& cube, auto rx, auto ry, auto rz) {
    return cube.x.hi < rx.lo || rx.hi < cube.x.lo || cube.y.hi < ry.lo ||
           ry.hi < cube.y.lo || cube.z.hi < rz.lo || rz.hi < cube.z.lo;
  };

  string line;
  while (getline(cin, line)) {
    auto w = split<string_view>(line, " =.,"sv);
    bool on = w[0] == "on";
    Range x{ to<int64_t>(w[2]), to<int64_t>(w[3]) };
    Range y{ to<int64_t>(w[5]), to<int64_t>(w[6]) };
    Range z{ to<int64_t>(w[8]), to<int64_t>(w[9]) };

    vector<Cube> next;
    for (const auto& cube : cubes) {
      if (NoIntersect(cube, x, y, z)) {
        next.push_back(cube);
      } else {
        Range mx{ max(cube.x.lo, x.lo), min(cube.x.hi, x.hi) };
        Range my{ max(cube.y.lo, y.lo), min(cube.y.hi, y.hi) };
        Range mz{ max(cube.z.lo, z.lo), min(cube.z.hi, z.hi) };
        if (cube.x.lo < mx.lo)
          next.push_back({ Range{ cube.x.lo, mx.lo - 1 }, cube.y, cube.z });
        if (mx.hi < cube.x.hi)
          next.push_back({ Range{ mx.hi + 1, cube.x.hi }, cube.y, cube.z });
        if (cube.y.lo < my.lo)
          next.push_back({ mx, Range{ cube.y.lo, my.lo - 1 }, cube.z });
        if (my.hi < cube.y.hi)
          next.push_back({ mx, Range{ my.hi + 1, cube.y.hi }, cube.z });
        if (cube.z.lo < mz.lo)
          next.push_back({ mx, my, Range{ cube.z.lo, mz.lo - 1 } });
        if (mz.hi < cube.z.hi)
          next.push_back({ mx, my, Range{ mz.hi + 1, cube.z.hi } });
      }
    }
    if (on) {
      next.push_back({ x, y, z });
    }
    cubes = move(next);
  }

  auto Count = [&](auto volume) {
    return transform_reduce(
      cubes.begin(), cubes.end(), int64_t{}, plus<int64_t>(), volume);
  };

  auto VolumeA = [](const auto& cube) {
    if (cube.x.lo > 50 || cube.x.hi < -50 || cube.y.lo > 50 ||
        cube.y.hi < -50 || cube.z.lo > 50 || cube.z.hi < -50)
      return int64_t{};
    return (max(-50ll, cube.x.hi) - min(50ll, cube.x.lo) + 1) *
           (max(-50ll, cube.y.hi) - min(50ll, cube.y.lo) + 1) *
           (max(-50ll, cube.z.hi) - min(50ll, cube.z.lo) + 1);
  };

  auto VolumeB = [](const auto& cube) {
    return (cube.x.hi - cube.x.lo + 1) * (cube.y.hi - cube.y.lo + 1) *
           (cube.z.hi - cube.z.lo + 1);
  };

  cout << Count(VolumeA) << endl;
  cout << Count(VolumeB) << endl;

  return 0;
}
