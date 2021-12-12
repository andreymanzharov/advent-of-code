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

  unordered_map<string, vector<string>> g;

  string line;
  while (getline(cin, line)) {
    auto w = split<string_view>(line, "-"sv);
    g[string{ w[0] }].emplace_back(w[1]);
    g[string{ w[1] }].emplace_back(w[0]);
  }

  unordered_set<string_view> s;
  function<int(const string&, bool)> Dfs = [&](const string& v, bool dup) {
    if (v == "end") {
      return 1;
    }
    if (s.count(v) > 0) {
      if (!dup || v == "start")
        return 0;
      dup = false;
    }
    int count = 0;
    if (islower(v.front())) {
      auto [it, f] = s.insert(v);
      for (const auto& u : g[v]) {
        count += Dfs(u, dup);
      }
      if (f)
        s.erase(it);
    } else {
      for (const auto& u : g[v]) {
        count += Dfs(u, dup);
      }
    }
    return count;
  };

  cout << Dfs("start", false) << endl;
  cout << Dfs("start", true) << endl;

  return 0;
}
