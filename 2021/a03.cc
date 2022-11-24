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

  vector<string> v;
  string line;
  while (getline(cin, line)) {
    v.push_back(std::move(line));
  }

  auto MostCommon = [&](auto index, auto first, auto last) {
    return 2 * count_if(
                 first, last, [&](const auto& s) { return s[index] == '1'; }) >=
               distance(first, last)
             ? '1'
             : '0';
  };

  auto first = v.begin();
  auto last = v.end();

  {
    uint32_t gamma = 0, epsilon = 0;

    for (size_t i = 0; i < v.front().size(); ++i) {
      gamma <<= 1;
      epsilon <<= 1;
      if (MostCommon(i, first, last) == '1') {
        gamma++;
      } else {
        epsilon++;
      }
    }

    cout << gamma * epsilon << endl;
  }

  {
    auto Rating = [&](auto first, auto last, auto comp) {
      for (size_t index = 0; next(first) != last; ++index) {
        auto x = MostCommon(index, first, last);
        last = partition(
          first, last, [&](const auto& s) { return comp(s[index], x); });
      }
      int result{};
      from_chars(first->data(), first->data() + first->size(), result, 2);
      return result;
    };

    int oxygen = Rating(first, last, [](char x, char y) { return x != y; });
    int co2 = Rating(first, last, [](char x, char y) { return x == y; });
    cout << oxygen * co2 << endl;
  }
}
