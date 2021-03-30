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

  vector<int> js;
  int x;
  js.push_back(0);
  while (cin >> x) {
    js.push_back(x);
  }
  sort(begin(js), end(js));
  js.push_back(js.back() + 3);
  array<int, 4> d{ 0 };
  for (size_t i = 1; i < js.size(); ++i) {
    ++d[js[i] - js[i - 1]];
  }
  cout << d[1] * d[3] << endl;

  vector<uint64_t> dp(js.back() + 1, 0);
  dp[0] = 1;
  for (int j : js) {
    for (int i = 1; i <= 3; ++i) {
      if (j - i >= 0) {
        dp[j] += dp[j - i];
      }
    }
  }

  cout << dp[js.back()] << endl;
}
