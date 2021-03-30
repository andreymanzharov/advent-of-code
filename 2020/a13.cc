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

  size_t depart;
  cin >> depart;
  string line;
  getline(cin, line);
  getline(cin, line);

  const auto w = x::split(line);
  vector<pair<size_t, size_t>> xs;
  for (size_t i = 0; i < w.size(); ++i) {
    if (w[i] != "x") {
      xs.emplace_back(x::to<size_t>(w[i]), i);
    }
  }

  size_t min_wait = 1'000'000'000;
  size_t answer = 0;
  for (auto [x, _] : xs) {
    size_t wait = (depart + x - 1) / x * x - depart;
    if (wait < min_wait) {
      min_wait = wait;
      answer = wait * x;
    }
  }

  cout << answer << endl;

  size_t step = xs.front().first;
  size_t y = 0;

  for (size_t i = 1; i < xs.size(); ++i) {
    while ((y + xs[i].second) % xs[i].first != 0) {
      y += step;
    }
    auto y1 = y + step;
    while ((y1 + xs[i].second) % xs[i].first != 0) {
      y1 += step;
    }
    step = y1 - y;
  }
  cout << y << endl;
}
