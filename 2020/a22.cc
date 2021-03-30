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
}

int
main()
{
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  deque<int> p1, p2;

  string line;
  getline(cin, line);
  while (getline(cin, line)) {
    if (line.empty())
      break;
    p1.push_back(x::to<int>(line));
  }
  getline(cin, line);
  while (getline(cin, line)) {
    if (line.empty())
      break;
    p2.push_back(x::to<int>(line));
  }

  auto Game = [&](deque<int> d1, deque<int> d2) {
    auto _Game =
      [&](auto _Game, int game, deque<int> d1, deque<int> d2) -> int {
      set<pair<deque<int>, deque<int>>> was;

      while (!d1.empty() && !d2.empty()) {
        if (was.find({ d1, d2 }) != end(was)) {
          d2.clear();
          break;
        }
        was.insert({ d1, d2 });

        int c1 = d1.front();
        int c2 = d2.front();
        d1.pop_front();
        d2.pop_front();

        int winner = 0;
        if (c1 <= static_cast<int>(d1.size()) &&
            c2 <= static_cast<int>(d2.size())) {
          winner = _Game(_Game,
                         game + 1,
                         { begin(d1), begin(d1) + c1 },
                         { begin(d2), begin(d2) + c2 });
        } else {
          winner = c1 > c2 ? 1 : 2;
        }
        if (winner == 1) {
          d1.push_back(c1);
          d1.push_back(c2);
        } else {
          d2.push_back(c2);
          d2.push_back(c1);
        }
      }
      int winner = d2.empty() ? 1 : 2;
      if (game == 1) {
        auto& d = d2.empty() ? d1 : d2;
        int r = 0, k = d.size();
        for (int x : d) {
          r += x * k;
          --k;
        }
        cout << r << endl;
      }
      return winner;
    };
    return _Game(_Game, 1, d1, d2);
  };

  Game(p1, p2);
}
