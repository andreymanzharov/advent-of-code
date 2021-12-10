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

int
main()
{
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int a = 0;
  vector<int64_t> b;

  unordered_map<char, int> m1{
    { ')', 3 }, { ']', 57 }, { '}', 1197 }, { '>', 25137 }
  };
  unordered_map<char, int> m2{ { '(', 1 }, { '[', 2 }, { '{', 3 }, { '<', 4 } };

  string line;
  while (getline(cin, line)) {
    vector<char> s;
    for (char c : line) {
      if (c == '(' || c == '[' || c == '{' || c == '<') {
        s.push_back(c);
      } else {
        assert(!s.empty());
        if ((c == ')' && s.back() != '(') || (c == ']' && s.back() != '[') ||
            (c == '}' && s.back() != '{') || (c == '>' && s.back() != '<')) {
          a += m1[c];
          s.clear();
          break;
        }
        s.pop_back();
      }
    }
    if (!s.empty()) {
      reverse(s.begin(), s.end());
      int64_t score = 0;
      for (char c : s) {
        score = score * 5 + m2[c];
      }
      b.push_back(score);
    }
  }
  sort(b.begin(), b.end());

  cout << a << endl;
  cout << b[b.size() / 2] << endl;

  return 0;
}
