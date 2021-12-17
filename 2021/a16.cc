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
#include <queue>
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

  class BitStream
  {
  public:
    BitStream(const string& hex)
    {
      transform(hex.begin(), hex.end(), back_inserter(data), [](char c) {
        if ('0' <= c && c <= '9')
          return c - '0';
        return c - 'A' + 10;
      });
      cur = data.begin();
    }

    uint32_t next(uint8_t bits = 1)
    {
      uint32_t r = 0;
      while (bits > 0) {
        auto len = min(bits, rem);
        auto val = (*cur >> (rem - len)) & ((1 << len) - 1);
        r = (r << len) + val;
        bits -= len;
        if ((rem -= len) == 0) {
          ++cur;
          rem = 4;
        }
      }
      return r;
    }

  private:
    vector<uint8_t> data;
    vector<uint8_t>::const_iterator cur;
    uint8_t rem = 4;
  };

  string line;
  getline(cin, line);

  BitStream bs(line);

  auto a = 0u;

  function<pair<size_t, uint64_t>()> ParsePacket = [&]() {
    a += bs.next(3);
    auto id = bs.next(3);
    auto size = 6uz;
    uint64_t v = 0ull;
    if (id == 4) {
      uint8_t bit;
      do {
        bit = bs.next();
        v = (v << 4) + bs.next(4);
        size += 5;
      } while (bit != 0);
    } else {
      auto length_type = bs.next();
      vector<uint64_t> vs;
      if (length_type == 0) {
        auto bits = bs.next(15);
        size += 16 + bits;
        while (bits > 0) {
          auto [s, v] = ParsePacket();
          bits -= s;
          vs.push_back(v);
        }
      } else {
        auto n = bs.next(11);
        vs.reserve(n);
        size += 12;
        while (n-- > 0) {
          auto [s, v] = ParsePacket();
          size += s;
          vs.push_back(v);
        }
      }
      switch (id) {
        case 0:
          v = reduce(vs.begin(), vs.end());
          break;
        case 1:
          v = reduce(vs.begin(), vs.end(), 1ull, multiplies<>());
          break;
        case 2:
          v = *min_element(vs.begin(), vs.end());
          break;
        case 3:
          v = *max_element(vs.begin(), vs.end());
          break;
        case 5:
          assert(vs.size() == 2);
          v = vs[0] > vs[1] ? 1 : 0;
          break;
        case 6:
          assert(vs.size() == 2);
          v = vs[0] < vs[1] ? 1 : 0;
          break;
        case 7:
          assert(vs.size() == 2);
          v = vs[0] == vs[1] ? 1 : 0;
          break;
      }
    }
    return pair{ size, v };
  };

  auto [_, b] = ParsePacket();

  cout << a << endl;
  cout << b << endl;

  return 0;
}
