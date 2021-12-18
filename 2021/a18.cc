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

  auto Parse = [](string_view s) {
    vector<int> v;
    for (char c : s) {
      if ('0' <= c && c <= '9') {
        v.push_back(c - '0');
      } else if (c == '[') {
        v.push_back(-1);
      } else if (c == ']') {
        v.push_back(-2);
      }
    }
    return v;
  };

  auto Reduce = [](const vector<int>& v) {
    vector<int> r;
    int nested = 0;
    for (auto it = v.begin(); it != v.end(); ++it) {
      if (*it == -1)
        nested++;
      else if (*it == -2)
        nested--;
      if (nested > 4) {
        int left = *(it + 1);
        int right = *(it + 2);
        auto to_left = r.rbegin();
        while (to_left != r.rend() && (*to_left == -1 || *to_left == -2))
          ++to_left;
        if (to_left != r.rend()) {
          *to_left += left;
        }
        r.push_back(0);
        it = it + 4;
        while (it != v.end() && (*it == -1 || *it == -2)) {
          r.push_back(*it++);
        }
        if (it != v.end()) {
          r.push_back(*it + right);
          while (++it != v.end()) {
            r.push_back(*it);
          }
        }
        return pair{ r, true };
      }
      r.push_back(*it);
    }
    r.clear();
    for (auto it = v.begin(); it != v.end(); ++it) {
      if (*it >= 10) {
        r.push_back(-1);
        r.push_back(*it / 2);
        r.push_back(*it - *it / 2);
        r.push_back(-2);
        while (++it != v.end()) {
          r.push_back(*it);
        }
        return pair{ r, true };
      }
      r.push_back(*it);
    }
    return pair{ r, false };
  };

  auto Sum = [&](const vector<int>& v1, const vector<int>& v2) {
    vector<int> v;
    v.reserve(2 + v1.size() + v2.size());
    v.push_back(-1);
    v.insert(v.end(), v1.begin(), v1.end());
    v.insert(v.end(), v2.begin(), v2.end());
    v.push_back(-2);
    while (true) {
      auto [r, f] = Reduce(v);
      if (!f)
        return r;
      v = move(r);
    }
  };

  function<pair<int, vector<int>::const_iterator>(vector<int>::const_iterator)>
    Magnitude = [&](vector<int>::const_iterator it) {
      if (*it == -1) {
        auto [l, il] = Magnitude(it + 1);
        auto [r, ir] = Magnitude(il);
        return pair{ 3 * l + 2 * r, ir + 1 };
      } else {
        return pair{ *it, it + 1 };
      }
    };

  vector<vector<int>> vs;

  string line;
  while (getline(cin, line)) {
    vs.emplace_back(Parse(line));
  }

  vector<int>& v{ vs.front() };
  for (auto i = 1uz; i < vs.size(); ++i) {
    v = Sum(v, vs[i]);
  }

  auto [a, it] = Magnitude(v.begin());
  assert(it == v.end());

  cout << a << endl;

  auto b = numeric_limits<int>::min();
  for (auto i = 0uz; i < vs.size(); ++i)
    for (auto j = 0uz; j < vs.size(); ++j)
      if (i != j) {
        auto [m, _] = Magnitude(Sum(vs[i], vs[j]).begin());
        b = max(b, m);
      }
  cout << b << endl;

  return 0;
}
