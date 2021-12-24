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

struct Program
{
  array<int64_t, 4> v;
  vector<function<void(const int64_t*&)>> cmds;
  Program()
  {
    string line;
    while (getline(cin, line)) {
      auto s = split<string_view>(line);
      auto cmd = s[0];
      auto a = s[1][0] - 'w';
      if (cmd == "inp") {
        cmds.push_back([&a = v[a]](auto& in) { a = *in++; });
      } else {
        bool ref = 'w' <= s[2][0] && s[2][0] <= 'z';
        auto b = ref ? s[2][0] - 'w' : to<int>(s[2]);
        if (cmd == "add") {
          if (ref)
            cmds.push_back([&a = v[a], &b = v[b]](auto) { a += b; });
          else
            cmds.push_back([&a = v[a], b](auto) { a += b; });
        } else if (cmd == "mul") {
          if (ref)
            cmds.push_back([&a = v[a], &b = v[b]](auto) { a *= b; });
          else
            cmds.push_back([&a = v[a], b](auto) { a *= b; });
        } else if (cmd == "div") {
          if (ref)
            cmds.push_back([&a = v[a], &b = v[b]](auto) { a /= b; });
          else
            cmds.push_back([&a = v[a], b](auto) { a /= b; });
        } else if (cmd == "mod") {
          if (ref)
            cmds.push_back([&a = v[a], &b = v[b]](auto) { a %= b; });
          else
            cmds.push_back([&a = v[a], b](auto) { a %= b; });
        } else if (cmd == "eql") {
          if (ref)
            cmds.push_back(
              [&a = v[a], &b = v[b]](auto) { a = a == b ? 1 : 0; });
          else
            cmds.push_back([&a = v[a], b](auto) { a = a == b ? 1 : 0; });
        }
      }
    }
  }
  template<typename It>
  void run(It in)
  {
    fill(v.begin(), v.end(), 0);
    for (auto& cmd : cmds) {
      cmd(in);
    }
  }
};

int
main()
{
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  // x = z % 26 - a[i];
  // z = z / 26;
  // if (x != w) {
  //   z = z * 26;
  //   z = z + (w + b[i]);
  // }

  Program p;
  //                     0  1  2  3  4  5  6  7  8  9 10 11 12 13
  array<int64_t, 14> mx{ 3, 9, 9, 2, 4, 9, 8, 9, 4, 9, 9, 9, 6, 9 };
  array<int64_t, 14> mn{ 1, 6, 8, 1, 1, 4, 1, 2, 1, 6, 1, 1, 1, 7 };
  //                     1  2  3  3  2  4  5  5  6  6  7  7  4  1
  //                    12 12 12 -9 -9 14 14 -10 15 -2 11 -15 -9 -3
  p.run(mx.cbegin());
  assert(p.v[3] == 0);

  p.run(mn.cbegin());
  assert(p.v[3] == 0);

  auto Out = [](const auto& m) {
    for (int x : m) {
      cout << x;
    }
    cout << endl;
  };

  Out(mx);
  Out(mn);

  return 0;
}
