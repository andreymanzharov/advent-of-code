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

  string s;
  getline(cin, s);
  auto a = split<int>(s);

  auto [mn, mx] = minmax_element(a.begin(), a.end());

  auto r1 = numeric_limits<int>::max();
  auto r2 = numeric_limits<int>::max();
  for (auto z = *mn; z <= *mx; ++z) {
    int s1 = transform_reduce(
      a.begin(), a.end(), 0, plus<>(), [&](auto x) { return abs(x - z); });
    r1 = min(r1, s1);
    int s2 = transform_reduce(a.begin(), a.end(), 0, plus<>(), [&](auto x) {
      auto n = abs(x - z);
      return (1 + n) * n / 2;
    });
    r2 = min(r2, s2);
  }

  cout << r1 << endl;
  cout << r2 << endl;

  return 0;
}
