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

  int p1{}, p2{};

  string s;
  while (getline(cin, s)) {
    if (s == "A X"sv) {
      p1 += 1 + 3;
      p2 += 3 + 0;
    } else if (s == "A Y"sv) {
      p1 += 2 + 6;
      p2 += 1 + 3;
    } else if (s == "A Z"sv) {
      p1 += 3 + 0;
      p2 += 2 + 6;
    } else if (s == "B X"sv) {
      p1 += 1 + 0;
      p2 += 1 + 0;
    } else if (s == "B Y"sv) {
      p1 += 2 + 3;
      p2 += 2 + 3;
    } else if (s == "B Z"sv) {
      p1 += 3 + 6;
      p2 += 3 + 6;
    } else if (s == "C X"sv) {
      p1 += 1 + 6;
      p2 += 2 + 0;
    } else if (s == "C Y"sv) {
      p1 += 2 + 0;
      p2 += 3 + 3;
    } else if (s == "C Z"sv) {
      p1 += 3 + 3;
      p2 += 1 + 6;
    }
  }

  cout << p1 << endl;
  cout << p2 << endl;

  return 0;
}
