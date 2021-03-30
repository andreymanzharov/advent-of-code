#define DBG_MACRO_NO_WARNING
#include "dbg.h"

#include <algorithm>
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

  uint64_t x;

  vector<uint64_t> a;
  vector<uint64_t> sums;
  unordered_map<uint64_t, int> numbers;
  deque<uint64_t> preamble;
  cin >> x;
  sums.push_back(0);
  for (int i = 0; i < 25; ++i) {
    cin >> x;
    a.push_back(x);
    sums.push_back(sums.back() + x);
    for (auto y : preamble) {
      ++numbers[x + y];
    }
    preamble.push_back(x);
  }

  uint64_t invalid = 0;

  while (cin >> x) {
    a.push_back(x);
    sums.push_back(sums.back() + x);
    if (numbers[x] == 0) {
      invalid = x;
      break;
    }
    for (size_t i = 1; i < preamble.size(); ++i) {
      --numbers[preamble[0] + preamble[i]];
      ++numbers[x + preamble[i]];
    }
    preamble.pop_front();
    preamble.push_back(x);
  }

  cout << invalid << endl;

  for (size_t i = 0; i < sums.size() - 1; ++i) {
    for (size_t j = i + 2; j < sums.size(); ++j) {
      if (sums[j] - sums[i] == invalid) {
        auto [mn, mx] = minmax_element(begin(a) + i, begin(a) + j);
        cout << *mn + *mx << endl;
        break;
      }
    }
  }
}
