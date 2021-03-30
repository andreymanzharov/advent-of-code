#define DBG_MACRO_NO_WARNING
#include "dbg.h"

#include <algorithm>
#include <cctype>
#include <charconv>
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

  enum class Operation
  {
    acc,
    jmp,
    nop
  };

  vector<pair<Operation, int>> code;

  string line;
  while (getline(cin, line)) {
    auto v = x::split(line);
    if (v[1].front() == '+')
      v[1].remove_prefix(1);
    int arg = x::to<int>(v[1]);
    if (v[0] == "acc") {
      code.emplace_back(Operation::acc, arg);
    } else if (v[0] == "jmp") {
      code.emplace_back(Operation::jmp, arg);
    } else {
      code.emplace_back(Operation::nop, arg);
    }
  }

  auto Execute = [&]() -> pair<bool, int> {
    vector<bool> executed(code.size());
    int accumulator = 0;
    size_t ip = 0;
    while (ip < code.size() && !executed[ip]) {
      executed[ip] = true;
      auto [op, arg] = code[ip];
      if (op == Operation::acc) {
        accumulator += arg;
      } else if (op == Operation::jmp) {
        ip += arg - 1;
      }
      ip += 1;
    }
    return { ip == code.size(), accumulator };
  };

  cout << Execute().second << endl;

  for (auto& [op, arg] : code) {
    auto prev = op;
    if (op == Operation::jmp) {
      op = Operation::nop;
    } else if (op == Operation::nop) {
      op = Operation::jmp;
    }
    if (const auto [terminated, value] = Execute(); terminated) {
      cout << value << endl;
    }
    op = prev;
  }
}
