#define DBG_MACRO_NO_WARNING
#include "dbg.h"

#include <algorithm>
#include <array>
#include <cassert>
#include <cctype>
#include <charconv>
#include <cmath>
#include <deque>
#include <functional>
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

  int sum1{};

  vector<string> v;

  {
    string s;
    while (getline(cin, s)) {
      v.push_back(std::move(s));
    }
  }

  for (auto& s : v) {
    auto n = s.size();
    array<bool, 52> t{};
    for (size_t i{}; i < n / 2; ++i) {
      if ('a' <= s[i] && s[i] <= 'z')
        t[s[i] - 'a'] = true;
      else
        t[s[i] - 'A' + 26] = true;
    }
    for (size_t i{ n / 2 }; i < n; ++i) {
      if ('a' <= s[i] && s[i] <= 'z' && t[s[i] - 'a']) {
        sum1 += s[i] - 'a' + 1;
        break;
      } else if ('A' <= s[i] && s[i] <= 'Z' && t[s[i] - 'A' + 26]) {
        sum1 += s[i] - 'A' + 27;
        break;
      }
    }
  }

  int sum2{};

  for (size_t i{}; i < v.size() / 3; ++i) {
    array<array<bool, 52>, 3> t{};
    for (size_t j{}; j < 3; ++j) {
      for (char c : v[i * 3 + j]) {
        if ('a' <= c && c <= 'z')
          t[j][c - 'a'] = true;
        else
          t[j][c - 'A' + 26] = true;
      }
    }
    for (size_t j{}; j < 52; ++j) {
      if (t[0][j] && t[1][j] && t[2][j]) {
        sum2 += j + 1;
        break;
      }
    }
  }

  cout << sum1 << endl;
  cout << sum2 << endl;

  return 0;
}
