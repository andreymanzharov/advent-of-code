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

vector<string_view>
split(string_view sv, string_view delimiters = ", "sv)
{
  vector<string_view> parts;
  while (!sv.empty()) {
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

constexpr int N = 7 * 2;

struct S
{
  int e;
  array<array<bool, N>, 4> fs;
  int t;
};

ostream&
operator<<(ostream& os, const S& s)
{
  os << "Step: " << s.t << '\n';
  for (int n = 4; n-- > 0;) {
    os << 'F' << n + 1 << ' ';
    os << (s.e == n ? "E  " : ".  ");
    for (int i = 0; i < N; ++i) {
      if (s.fs[n][i]) {
        os << "TPSpRED"[i / 2] << "GM"[i % 2] << ' ';
      } else {
        os << ".  ";
      }
    }
    os << '\n';
  }

  return os;
}

int
main()
{
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  // TPSpR

  S initial{ .e = 0, .fs = {}, .t = 0 };
  initial.fs[0][0] = true;
  initial.fs[0][1] = true;
  initial.fs[0][2] = true;
  initial.fs[0][4] = true;
  initial.fs[0][10] = true;
  initial.fs[0][11] = true;
  initial.fs[0][12] = true;
  initial.fs[0][13] = true;
  initial.fs[1][3] = true;
  initial.fs[1][5] = true;
  initial.fs[2][6] = true;
  initial.fs[2][7] = true;
  initial.fs[2][8] = true;
  initial.fs[2][9] = true;

  dbg(initial);

  set<pair<int, array<array<bool, N>, 4>>> states;
  deque<S> q;
  q.push_back(move(initial));

  auto Good = [](const S& s, int f1, int f2) {
    for (int n : { f1, f2 }) {
      const auto& f = s.fs[n];
      for (int m = 1; m < N; m += 2) {
        if (!f[m])
          continue;
        if (f[m - 1])
          continue;
        for (int g = 0; g < N; g += 2) {
          if (g != m - 1 && f[g])
            return false;
        }
      }
    }
    return true;
  };

  while (!q.empty()) {
    S s = move(q.front());
    q.pop_front();

    if (s.e == 3 &&
        all_of(s.fs[3].begin(), s.fs[3].end(), [](bool x) { return x; })) {
      cout << s.t << endl;
      dbg(s);
      break;
    }

    for (int de : { -1, 1 }) {
      int ne = s.e + de;
      if (ne < 0 || ne > 3)
        continue;
      for (int a = 0; a < N; ++a) {
        if (!s.fs[s.e][a])
          continue;
        S ns = s;
        ns.e = ne;
        ns.fs[s.e][a] = false;
        ns.fs[ne][a] = true;
        ns.t = s.t + 1;
        if (states.find({ ns.e, ns.fs }) == states.end() && Good(ns, s.e, ne)) {
          q.push_back(move(ns));
          states.insert({ ns.e, ns.fs });
        }
        for (int b = a + 1; b < N; ++b) {
          if (!s.fs[s.e][b])
            continue;
          S ns = s;
          ns.e = ne;
          ns.fs[s.e][a] = false;
          ns.fs[s.e][b] = false;
          ns.fs[ne][a] = true;
          ns.fs[ne][b] = true;
          ns.t = s.t + 1;
          if (states.find({ ns.e, ns.fs }) == states.end() &&
              Good(ns, s.e, ne)) {
            q.push_back(move(ns));
            states.insert({ ns.e, ns.fs });
          }
        }
      }
    }
  }
}
