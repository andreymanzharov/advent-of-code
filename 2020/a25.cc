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

  const int64_t M = 20201227;

  int card_pkey;
  int door_pkey;
  cin >> card_pkey >> door_pkey;

  auto LoopSize = [](int64_t key) {
    int64_t value = 1;
    for (int i = 0;; ++i) {
      value *= 7;
      value %= M;
      if (value == key) {
        return i + 1;
      }
    }
    return -1;
  };

  auto Transform = [](int64_t subject, int n) {
    int64_t value = 1;
    for (int i = 0; i < n; ++i) {
      value *= subject;
      value %= M;
    }
    return value;
  };

  cout << Transform(door_pkey, LoopSize(card_pkey)) << endl;
}
