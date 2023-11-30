#include <algorithm>
#include <array>
#include <cassert>
#include <cctype>
#include <charconv>
#include <cmath>
#include <deque>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <optional>
#include <queue>
#include <set>
#include <sstream>
#include <string>
#include <string_view>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <variant>
#include <vector>

#define DBG_MACRO_NO_WARNING
#include "dbg.h"

namespace x {

template<typename T>
T
to(std::string_view from)
{
  T r{};
  if (auto [p, ec] = std::from_chars(from.data(), from.data() + from.size(), r);
      p != from.data() + from.size()) {
    std::ostringstream message;
    message << "cannot convert " << from;
    throw std::invalid_argument(message.str());
  }
  return r;
}

template<>
inline std::string_view
to(std::string_view from)
{
  return from;
}

template<>
inline std::string
to(std::string_view from)
{
  return std::string{ from };
}

template<typename T = std::string_view>
std::vector<T>
split(std::string_view sv, std::string_view delimiters = ", ")
{
  std::vector<T> parts;
  while (!sv.empty()) {
    auto p = sv.find_first_of(delimiters);
    if (p > 0)
      parts.push_back(to<T>(sv.substr(0, p)));
    if (p == std::string_view::npos)
      break;
    sv.remove_prefix(p + 1);
  }
  return parts;
}

template<typename It>
class range
{
public:
  range(It begin, It end)
    : begin_{ begin }
    , end_{ end }
  {
  }

  It begin() { return begin_; }
  It end() { return end_; }

private:
  It begin_;
  It end_;
};

}
