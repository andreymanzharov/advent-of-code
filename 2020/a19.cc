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

int
main()
{
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  using Rule = variant<char, vector<vector<int>>>;
  unordered_map<int, Rule> rules;

  string line;
  while (getline(cin, line)) {
    if (line.empty())
      break;
    const auto w = x::split(line, ": "sv);
    int index = x::to<int>(w[0]);
    if (w[1][0] == '"' && w[1][2] == '"') {
      rules[index] = w[1][1];
    } else {
      vector<vector<int>> alts;
      vector<int> rule;
      for (size_t i = 1; i < w.size(); ++i) {
        if (w[i] == "|") {
          alts.push_back(move(rule));
          rule.clear();
        } else {
          rule.push_back(x::to<int>(w[i]));
        }
      }
      alts.push_back(move(rule));
      rules[index] = move(alts);
    }
  }

  rules[8] = vector<vector<int>>{ { 42, 8 }, { 42 } };
  rules[11] = vector<vector<int>>{ { 42, 11, 31 }, { 42, 31 } };

  auto Match = [&](string_view s) {
    auto _match = [&](auto _match,
                      int rule_index,
                      string_view s) -> pair<bool, string_view> {
      auto& rule = rules[rule_index];
      if (holds_alternative<char>(rule)) {
        if (!s.empty() && s.front() == get<char>(rule)) {
          return { true, s.substr(1) };
        } else {
          return { false, s };
        }
      }
      const auto& alternatives = get<vector<vector<int>>>(rule);
      for (const auto& alt : alternatives) {
        bool match = true;
        string_view ss = s;
        for (size_t idx : alt) {
          auto [submatch, rest] = _match(_match, idx, ss);
          if (!submatch) {
            match = false;
            break;
          }
          ss = rest;
        }
        if (match) {
          return { true, ss };
        }
      }
      return { false, s };
    };
    auto [match, rest] = _match(_match, 42, s);
    if (!match)
      return false;
    while (!rest.empty()) {
      auto [match11, xxx] = _match(_match, 11, rest);
      if (match11 && xxx.empty())
        return true;
      auto [match42, yyy] = _match(_match, 42, rest);
      if (!match42)
        return false;
      rest = yyy;
    }
    return false;
  };

  // auto Gen = [&](int index) {
  //   auto _gen = [&](auto _gen, int index) -> string {
  //     const auto& rule = rules[index];
  //     if (holds_alternative<char>(rule)) {
  //       return string(1, get<char>(rule));
  //     }
  //     string result;
  //     for (const auto& alt : get<vector<vector<int>>>(rule)) {
  //       string s;
  //       for (auto idx : alt) {
  //         s += _gen(_gen, idx);
  //       }
  //       result += '(' + s + ')';
  //     }
  //     return result;
  //   };
  //   return _gen(_gen, index);
  // };

  // cout << Gen(0) << endl;

  int result = 0;
  while (getline(cin, line)) {
    // dbg(line);
    if (Match(line)) {
      ++result;
    }
  }
  cout << result << endl;
}
