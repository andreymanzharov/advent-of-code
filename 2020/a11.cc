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
  T r;
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

  vector<string> g;
  string line;
  while (getline(cin, line)) {
    g.push_back(move(line));
  }

  bool changed = true;
  while (changed) {
    changed = false;
    const int n = static_cast<int>(g.size());
    vector<string> ng(n);
    for (int i = 0; i < n; ++i) {
      const int m = static_cast<int>(g[i].size());
      string row = g[i];
      for (int j = 0; j < m; ++j) {
        if (g[i][j] != '.') {
          int occupied = 0;
          vector<pair<int, int>> d = { { -1, -1 }, { 0, -1 }, { 1, -1 },
                                       { -1, 0 },  { 1, 0 },  { -1, 1 },
                                       { 0, 1 },   { 1, 1 } };
          for (auto [di, dj] : d) {
            int ni = i + di;
            int nj = j + dj;
            while (0 <= ni && ni < n && 0 <= nj && nj < m) {
              if (g[ni][nj] != '.') {
                if (g[ni][nj] == '#') {
                  ++occupied;
                }
                break;
              }
              ni += di;
              nj += dj;
            }
          }
          if (g[i][j] == 'L' && occupied == 0) {
            row[j] = '#';
            changed = true;
          } else if (g[i][j] == '#' && occupied >= 5) {
            row[j] = 'L';
            changed = true;
          }
        }
      }
      ng[i] = move(row);
    }
    g = move(ng);
  }

  int seats = 0;
  for (const auto& row : g) {
    for (char x : row) {
      if (x == '#') {
        ++seats;
      }
    }
  }
  cout << seats << endl;
}
