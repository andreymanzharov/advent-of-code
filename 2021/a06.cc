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
} // namespace x

using namespace x;

int
main()
{
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  array<int64_t, 9> a{};

  string s;
  getline(cin, s);
  for (auto x : split(s)) {
    a[to<int>(x)]++;
  }

  auto Answer = [&]() { return accumulate(a.begin(), a.end(), int64_t{}); };

  for (int i = 0; i < 256; ++i) {
    if (i == 80) {
      cout << Answer() << endl;
    }

    rotate(a.begin(), a.begin() + 1, a.end());
    a[6] += a[8];
  }

  cout << Answer() << endl;

  return 0;
}
