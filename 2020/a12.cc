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
  T r;
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

  int x = 0, y = 0;
  int wx = 10, wy = 1;

  char d;
  int n;
  while (cin >> d >> n) {
    if (d == 'N') {
      wy += n;
    } else if (d == 'S') {
      wy -= n;
    } else if (d == 'W') {
      wx -= n;
    } else if (d == 'E') {
      wx += n;
    } else if (d == 'L') {
      for (int i = 0; i < n; i += 90) {
        int owx = wx, owy = wy;
        wx = -owy;
        wy = owx;
      }
    } else if (d == 'R') {
      for (int i = 0; i < n; i += 90) {
        int owx = wx, owy = wy;
        wx = owy;
        wy = -owx;
      }
    } else if (d == 'F') {
      x += wx * n;
      y += wy * n;
    }
  }

  cout << abs(x) + abs(y) << endl;
}
