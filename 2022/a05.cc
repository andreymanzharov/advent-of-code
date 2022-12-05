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

  vector<string> desc;
  string line;
  while (getline(cin, line) && !line.empty()) {
    desc.push_back(std::move(line));
  }

  auto Parse = [&desc]() {
    auto last = desc.back();
    auto n = split<string_view>(last).size();
    vector<vector<char>> stacks(n, vector<char>());
    for (auto line : range{ desc.rbegin() + 1, desc.rend() }) {
      for (size_t i{}, j{ 1 }; i < n && j < line.size(); ++i, j += 4) {
        if (isalpha(line[j]))
          stacks[i].push_back(line[j]);
      }
    }
    return stacks;
  };

  vector<vector<char>> s1, s2;
  s1 = s2 = Parse();

  while (getline(cin, line)) {
    auto w = split<string_view>(line);
    auto n = to<size_t>(w[1]);
    auto i = to<size_t>(w[3]) - 1;
    auto j = to<size_t>(w[5]) - 1;
    copy(s1[i].rbegin(), s1[i].rbegin() + n, back_inserter(s1[j]));
    copy(s2[i].end() - n, s2[i].end(), back_inserter(s2[j]));
    s1[i].erase(s1[i].end() - n, s1[i].end());
    s2[i].erase(s2[i].end() - n, s2[i].end());
  }

  for (auto& stack : s1) {
    cout << stack.back();
  }
  cout << endl;
  for (auto& stack : s2) {
    cout << stack.back();
  }
  cout << endl;

  return 0;
}
