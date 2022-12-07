#include <optional>
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

template<typename It>
class range
{
public:
  range(It begin, It end)
    : begin_{ begin }
    , end_{ end }
  {
  }

  It begin() { return begin_; }
  It end() { return end_; }

private:
  It begin_;
  It end_;
};
} // namespace x

using namespace x;

int
main()
{
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  vector<string> cwd;
  map<vector<string>, int> sizes;

  string line;
  getline(cin, line);
  while (cin) {
    auto args = split<string_view>(line);
    if (args[1] == "cd") {
      if (args[2].front() == '/') {
        cwd = {};
      } else if (args[2] == ".."sv) {
        cwd.pop_back();
      } else {
        cwd.emplace_back(args[2]);
      }
      getline(cin, line);
      continue;
    }
    if (args[1] == "ls"sv) {
      int size{};
      while (getline(cin, line)) {
        auto t = split<string_view>(line);
        if (t[0] == "$")
          break;
        if (t[0] != "dir"sv) {
          size += to<int>(t[0]);
        }
      }
      auto parents = cwd;
      while (!parents.empty()) {
        sizes[parents] += size;
        parents.pop_back();
      }
      sizes[{}] += size;
    }
  }

  int sum{};
  for (auto [dir, size] : sizes) {
    if (size <= 100000)
      sum += size;
  }
  cout << sum << endl;

  auto unused = 70000000 - sizes[{}];
  auto smallest = sizes[{}];
  for (auto [dir, size] : sizes) {
    if (size + unused >= 30000000) {
      smallest = min(smallest, size);
    }
  }
  cout << smallest << endl;

  return 0;
}
