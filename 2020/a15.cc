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

  vector<int> ps(30'000'000 + 1);

  string line;
  getline(cin, line);

  int p = 0;
  auto ws = x::split(line);
  for (size_t i = 0; i < ws.size() - 1; ++i) {
    ps[x::to<int>(ws[i])] = i + 1;
  }
  p = x::to<int>(ws.back());
  int turn = ws.size();
  while (turn < 30'000'000) {
    int spoken;
    if (ps[p] == 0) {
      spoken = 0;
    } else {
      spoken = turn - ps[p];
    }
    ps[p] = turn;
    p = spoken;
    ++turn;
  }
  cout << p << endl;
}
