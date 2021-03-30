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

  unordered_map<uint64_t, uint64_t> mem;

  string line;
  string mask;
  while (getline(cin, line)) {
    const auto w = x::split(line, "[] ="sv);
    if (w.size() == 2) {
      mask = w[1];
    } else {
      auto v = x::to<uint64_t>(w[2]);
      function<void(size_t, uint64_t)> R = [&](size_t i, uint64_t a) {
        if (i > 35) {
          mem[a] = v;
        } else {
          if (mask[35 - i] == '0') {
            R(i + 1, a);
          } else if (mask[35 - i] == '1') {
            R(i + 1, a | (1ull << i));
          } else {
            R(i + 1, a & ~(1ull << i));
            R(i + 1, a | (1ull << i));
          }
        }
      };
      R(0, x::to<uint64_t>(w[1]));
    }
  }

  uint64_t r = 0;
  for (auto [_, v] : mem) {
    r += v;
  }
  cout << r << endl;
}
