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
#include <variant>
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

class Number
{
  vector<tuple<optional<int>, size_t, size_t>> tree;
  size_t root;

  bool Explode()
  {
    bool exploded = false;
    optional<size_t> prev;
    optional<int> next;
    function<void(size_t, int)> explode = [&](size_t v, int d) {
      auto& [value, left, right] = tree[v];
      if (exploded) {
        if (!next)
          return;
        if (value) {
          *value += *next;
          next.reset();
        } else {
          explode(left, d + 1);
          explode(right, d + 1);
        }
        return;
      }
      if (value && !exploded) {
        prev = v;
        return;
      }
      if (d < 4 || exploded) {
        explode(left, d + 1);
        explode(right, d + 1);
        return;
      }
      exploded = true;
      if (prev) {
        auto& prev_value = get<0>(tree[*prev]);
        *prev_value += *get<0>(tree[left]);
      }
      next = *get<0>(tree[right]);
      value = 0;
      left = right = 0;
    };
    explode(root, 0);
    return exploded;
  }

  bool Split()
  {
    function<bool(int)> split = [&](size_t v) {
      auto& [value, left, right] = tree[v];
      if (!value)
        return split(left) || split(right);
      if (*value < 10)
        return false;
      tree.emplace_back(*value / 2, 0, 0);
      tree.emplace_back(*value - *value / 2, 0, 0);
      value.reset();
      left = tree.size() - 2;
      right = tree.size() - 1;
      return true;
    };
    return split(root);
  }

  void Reduce()
  {
    while (true) {
      if (Explode()) {
        continue;
      }
      if (Split())
        continue;
      return;
    }
  }

public:
  Number(string_view sv)
  {
    auto it = sv.begin();
    function<size_t()> Parse = [&]() {
      if (*it == '[') {
        ++it;
        auto left = Parse();
        ++it;
        auto right = Parse();
        ++it;
        tree.emplace_back(nullopt, left, right);
      } else {
        tree.emplace_back(*it - '0', 0, 0);
        ++it;
      }
      return tree.size() - 1;
    };
    root = Parse();
    assert(it == sv.end());
  }

  Number& operator+=(const Number& rhs)
  {
    auto offset = tree.size();
    for (auto [value, left, right] : rhs.tree) {
      tree.emplace_back(value, left + offset, right + offset);
    }
    tree.emplace_back(nullopt, root, rhs.root + offset);
    root = tree.size() - 1;
    Reduce();
    return *this;
  }

  int Magnitude() const
  {
    function<int(size_t)> magnitude = [&](size_t v) {
      const auto& [value, left, right] = tree[v];
      return value ? *value : 3 * magnitude(left) + 2 * magnitude(right);
    };
    return magnitude(root);
  }

  friend ostream& operator<<(ostream& out, const Number& n)
  {
    function<void(size_t)> print = [&](size_t v) {
      const auto& [value, left, right] = n.tree[v];
      if (value)
        out << *value;
      else {
        out << '[';
        print(left);
        out << ',';
        print(right);
        out << ']';
      }
    };
    print(n.root);
    return out;
  }
};

Number
operator+(const Number& lhs, const Number& rhs)
{
  Number result{ lhs };
  result += rhs;
  return result;
}

int
main()
{
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  vector<Number> ns;

  string line;
  while (getline(cin, line)) {
    ns.emplace_back(line);
  }

  Number n{ ns.front() };
  for (auto i = 1uz; i < ns.size(); ++i) {
    n += ns[i];
  }
  cout << n.Magnitude() << endl;

  auto max_magnitude = numeric_limits<int>::min();
  for (auto i = ns.begin(); i != ns.end(); ++i) {
    for (auto j = ns.begin(); j != ns.end(); ++j) {
      if (i == j)
        continue;
      max_magnitude = max(max_magnitude, (*i + *j).Magnitude());
    }
  }
  cout << max_magnitude << endl;
  return 0;
}
