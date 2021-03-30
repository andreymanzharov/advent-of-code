#define DBG_MACRO_NO_WARNING
#include "dbg.h"

#include <algorithm>
#include <array>
#include <cassert>
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

class Tokenizer
{
public:
  explicit Tokenizer(string_view source)
    : source(source)
    , pos(0)
  {
    Next();
  }
  using Token = variant<int, char>;
  Token Next()
  {
    while (pos < source.size() && isblank(source[pos])) {
      ++pos;
    }
    if (pos >= source.size()) {
      return current = '#';
    }
    if (isdigit(source[pos])) {
      return current = source[pos++] - '0';
    }
    return current = source[pos++];
  }
  Token Current() const { return current; }
  Token Consume()
  {
    auto result = current;
    Next();
    return result;
  }
  Token Consume(char c)
  {
    assert(get<char>(current) == c);
    return Consume();
  }

private:
  const string_view source;
  size_t pos;
  Token current;
};

uint64_t
E(Tokenizer& ts);

uint64_t
P(Tokenizer& ts)
{
  if (holds_alternative<int>(ts.Current())) {
    return get<int>(ts.Consume());
  }
  ts.Consume('(');
  auto result = E(ts);
  ts.Consume(')');
  return result;
}

uint64_t
T(Tokenizer& ts)
{
  auto result = P(ts);
  while (true) {
    auto t = ts.Current();
    if (holds_alternative<char>(t)) {
      switch (get<char>(t)) {
        case '+':
          ts.Consume('+');
          result += P(ts);
          break;
        default:
          return result;
      }
    }
  }
}

uint64_t
E(Tokenizer& ts)
{
  auto result = T(ts);
  while (true) {
    auto t = ts.Current();
    if (holds_alternative<char>(t)) {
      switch (get<char>(t)) {
        case '*':
          ts.Consume('*');
          result *= T(ts);
          break;
        default:
          return result;
      }
    }
  }
}

int
main()
{
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  uint64_t sum = 0;
  string line;
  while (getline(cin, line)) {
    Tokenizer ts(line);
    auto x = E(ts);
    sum += x;
  }
  cout << sum << endl;
}
