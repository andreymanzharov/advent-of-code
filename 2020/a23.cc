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

  string cups = "219347865";
  // string cups = "389125467";
  //
  const int n = 1'000'000;

  vector<int> data(n);
  vector<int> next(n);
  vector<int> pos(n + 1);

  for (size_t i = 0; i < cups.size(); ++i) {
    data[i] = cups[i] - '0';
    next[i] = (i + 1) % n;
    pos[data[i]] = i;
  }
  for (size_t i = cups.size(); i < 1'000'000; ++i) {
    data[i] = i + 1;
    next[i] = (i + 1) % n;
    pos[data[i]] = i;
  }

  int current = 0;
  for (int k = 0; k < 10'000'000; ++k) {

    // int j = 0;
    // for (int i = 0; i < n; ++i) {
    //   if (current == j)
    //     cout << '(' << data[j] << ')';
    //   else
    //     cout << ' ' << data[j] << ' ';
    //   j = next[j];
    // }
    // cout << endl;

    int x = data[current];
    int a = data[next[current]];
    int b = data[next[next[current]]];
    int c = data[next[next[next[current]]]];
    while (true) {
      x = (x + n - 2) % n + 1;
      if (x != a && x != b && x != c) {
        break;
      }
    }
    int t = next[current];
    next[current] = next[next[next[next[current]]]];

    next[next[next[t]]] = next[pos[x]];
    next[pos[x]] = t;

    current = next[current];
  }

  // current = next[pos[1]];
  // for (int i = 0; i < n - 1; ++i) {
  //   cout << data[current];
  //   current = next[current];
  // }
  // cout << endl;
  cout << static_cast<uint64_t>(data[next[pos[1]]]) * data[next[next[pos[1]]]]
       << endl;
}
