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

struct A
{
  virtual void m1() { cout << "A" << endl; }
};

struct B : A
{
  void m1() override { cout << "B" << endl; }
};

struct C : A
{
  void m1() { cout << "C" << endl; }
};

int
main()
{
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  string line;
  getline(cin, line);

  auto w = split<string_view>(line, " :=,."sv);

  int mnx = to<int>(w[3]), mxx = to<int>(w[4]);
  int mny = to<int>(w[6]), mxy = to<int>(w[7]);

  auto In = [&](int x, int y) {
    return mnx <= x && x <= mxx && mny <= y && y <= mxy;
  };

  auto Go = [&](int vx, int vy) {
    int x = 0, y = 0;
    int r = y;
    while (!In(x, y) && x <= mxx && y >= mny) {
      x += vx;
      y += vy;
      r = max(r, y);
      vx = max(vx - 1, 0);
      vy -= 1;
    }
    return pair{ In(x, y), r };
  };

  int a = numeric_limits<int>::min();
  int b = 0;
  for (int vx = 0; vx <= mxx; ++vx) {
    for (int vy = mny; vy <= 100; ++vy) {
      if (auto [f, my] = Go(vx, vy); f) {
        a = max(a, my);
        ++b;
      }
    }
  }

  cout << a << endl;
  cout << b << endl;

  return 0;
}
