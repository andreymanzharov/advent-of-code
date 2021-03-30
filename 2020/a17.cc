#define DBG_MACRO_NO_WARNING
#include "dbg.h"

#include <algorithm>
#include <array>
#include <cctype>
#include <charconv>
#include <deque>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <string_view>
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
  while (true) {
    auto p = sv.find_first_of(delimiters);
    if (p > 0)
      parts.push_back(sv.substr(0, p));
    if (p == string_view::npos)
      break;
    sv.remove_prefix(p + 1);
  }
  return parts;
}
}

int
main()
{
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  set<tuple<int, int, int, int>> cubes;

  string line;
  int mnx = numeric_limits<int>::max(), mxx = numeric_limits<int>::min();
  int mny = numeric_limits<int>::max(), mxy = numeric_limits<int>::min();
  int mnz = numeric_limits<int>::max(), mxz = numeric_limits<int>::min();
  int mnw = numeric_limits<int>::max(), mxw = numeric_limits<int>::min();
  int y = 0;
  while (getline(cin, line)) {
    for (int x = 0; x < static_cast<int>(line.size()); ++x) {
      if (line[x] == '#') {
        cubes.insert({ x, y, 0, 0 });
        mnx = min(mnx, x);
        mxx = max(mxx, x);
        mny = min(mny, y);
        mxy = max(mxy, y);
        mnz = min(mnz, 0);
        mxz = max(mxz, 0);
        mnw = min(mnw, 0);
        mxw = max(mxw, 0);
      }
    }
    ++y;
  }

  vector<tuple<int, int, int, int>> d;
  for (int x = -1; x <= 1; ++x) {
    for (int y = -1; y <= 1; ++y) {
      for (int z = -1; z <= 1; ++z) {
        for (int w = -1; w <= 1; ++w) {
          if (x != 0 || y != 0 || z != 0 || w != 0) {
            d.emplace_back(x, y, z, w);
          }
        }
      }
    }
  }

  auto Neighbors = [&](int x, int y, int z, int w) {
    int r = 0;
    for (auto [dx, dy, dz, dw] : d) {
      int nx = x + dx;
      int ny = y + dy;
      int nz = z + dz;
      int nw = w + dw;
      if (cubes.find({ nx, ny, nz, nw }) != end(cubes)) {
        ++r;
      }
    }
    return r;
  };

  for (int i = 0; i < 6; ++i) {
    int nmnx = numeric_limits<int>::max(), nmxx = numeric_limits<int>::min();
    int nmny = numeric_limits<int>::max(), nmxy = numeric_limits<int>::min();
    int nmnz = numeric_limits<int>::max(), nmxz = numeric_limits<int>::min();
    int nmnw = numeric_limits<int>::max(), nmxw = numeric_limits<int>::min();
    set<tuple<int, int, int, int>> next_cubes;
    for (int x = mnx - 1; x <= mxx + 1; ++x) {
      for (int y = mny - 1; y <= mxy + 1; ++y) {
        for (int z = mnz - 1; z <= mxz + 1; ++z) {
          for (int w = mnw - 1; w <= mxw + 1; ++w) {
            int neighbours = Neighbors(x, y, z, w);
            if (const auto it = cubes.find({ x, y, z, w }); it != end(cubes)) {
              if (neighbours == 2 || neighbours == 3) {
                next_cubes.insert({ x, y, z, w });
                nmnx = min(nmnx, x);
                nmxx = max(nmxx, x);
                nmny = min(nmny, y);
                nmxy = max(nmxy, y);
                nmnz = min(nmnz, z);
                nmxz = max(nmxz, z);
                nmnw = min(nmnw, w);
                nmxw = max(nmxw, w);
              }
            } else {
              if (neighbours == 3) {
                next_cubes.insert({ x, y, z, w });
                nmnx = min(nmnx, x);
                nmxx = max(nmxx, x);
                nmny = min(nmny, y);
                nmxy = max(nmxy, y);
                nmnz = min(nmnz, z);
                nmxz = max(nmxz, z);
                nmnw = min(nmnw, w);
                nmxw = max(nmxw, w);
              }
            }
          }
        }
      }
    }
    mnx = nmnx;
    mxx = nmxx;
    mny = nmny;
    mxy = nmxy;
    mnz = nmnz;
    mxz = nmxz;
    mnw = nmnw;
    mxw = nmxw;
    cubes = move(next_cubes);

    // for (int z = mnz; z <= mxz; ++z) {
    //   cout << "z=" << z << endl;
    //   for (int y = mny; y <= mxy; ++y) {
    //     for (int x = mnx; x <= mxx; ++x) {
    //       cout << (cubes.find({ x, y, z }) != end(cubes) ? '#' : '.');
    //     }
    //     cout << endl;
    //   }
    // }
  }

  cout << cubes.size() << endl;
}
