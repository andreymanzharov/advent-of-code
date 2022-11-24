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

  vector<vector<string>> v;

  string line;
  while (getline(cin, line)) {
    v.push_back(split<string>(line, " |"sv));
    for (auto& s : v.back()) {
      sort(s.begin(), s.end());
    }
  }

  {
    int result = 0;
    for (const auto& w : v) {
      result += count_if(w.rbegin(), w.rbegin() + 4, [](auto s) {
        return s.size() == 2 || s.size() == 4 || s.size() == 3 || s.size() == 7;
      });
    }
    cout << result << endl;
  }

  {
    int result = 0;
    for (const auto& w : v) {
      vector<char> a(7, '.');
      iota(a.begin(), a.end(), 'a');

      unordered_map<string, int> c;
      do {
        array<string, 10> b{
          string{ a[0], a[1], a[2], a[4], a[5], a[6] },
          { a[2], a[5] },
          { a[0], a[2], a[3], a[4], a[6] },
          { a[0], a[2], a[3], a[5], a[6] },
          { a[1], a[2], a[3], a[5] },
          { a[0], a[1], a[3], a[5], a[6] },
          { a[0], a[1], a[3], a[4], a[5], a[6] },
          { a[0], a[2], a[5] },
          { a[0], a[1], a[2], a[3], a[4], a[5], a[6] },
          { a[0], a[1], a[2], a[3], a[5], a[6] },
        };

        unordered_map<string, int> d;
        for (auto i = 0uz; i < b.size(); ++i) {
          sort(b[i].begin(), b[i].end());
          d.emplace(std::move(b[i]), i);
        }

        if (all_of(w.begin(), w.end(), [&](const auto& s) {
              return d.contains(s);
            })) {
          c = std::move(d);
          break;
        }
      } while (next_permutation(a.begin(), a.end()));

      int output = 0;
      for_each(w.end() - 4, w.end(), [&](const auto& s) {
        output *= 10;
        output += c[s];
      });
      result += output;
    }
    cout << result << endl;
  }

  return 0;
}
