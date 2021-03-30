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

  unordered_map<string, vector<pair<int, int>>> classes;

  string line;
  while (getline(cin, line)) {
    if (line.empty())
      break;
    auto split = x::split(line, ":");
    const auto w = x::split(split[1], "- ");
    auto& c = classes[string(split[0])];
    for (size_t i = 0; i < w.size(); i += 3) {
      c.emplace_back(x::to<int>(w[i]), x::to<int>(w[i + 1]));
    }
  }
  getline(cin, line);
  string my_ticket;
  getline(cin, my_ticket);

  getline(cin, line);
  getline(cin, line);

  vector<vector<int>> valid_tickets;

  int rate = 0;
  while (getline(cin, line)) {
    vector<int> ticket;
    bool valid = true;
    for (const auto w : x::split(line)) {
      int x = x::to<int>(w);
      ticket.push_back(x);
      bool ok = false;
      for (const auto& [_, c] : classes) {
        for (const auto& [l, r] : c) {
          ok = ok || (l <= x && x <= r);
        }
      }
      if (!ok) {
        valid = false;
        rate += x;
      }
    }
    if (valid) {
      valid_tickets.push_back(move(ticket));
    }
  }
  cout << rate << endl;

  vector<set<string>> v(classes.size());
  for (auto& s : v) {
    for (const auto& [n, _] : classes) {
      s.insert(n);
    }
  }
  for (const auto ticket : valid_tickets) {
    for (size_t i = 0; i < ticket.size(); ++i) {
      int x = ticket[i];
      vector<string> invalid;
      for (auto cls : v[i]) {
        bool ok = false;
        for (const auto [l, r] : classes[cls]) {
          ok = ok || (l <= x && x <= r);
        }
        if (!ok)
          invalid.push_back(cls);
      }
      for (const auto& s : invalid) {
        v[i].erase(s);
      }
    }
  }
  for (size_t j = 0; j < v.size(); ++j) {
    for (auto& s : v) {
      if (s.size() == 1) {
        for (auto& t : v) {
          if (t.size() > 1) {
            t.erase(*begin(s));
          }
        }
      }
    }
  }

  uint64_t answer = 1;

  const auto mw = x::split(my_ticket);
  for (size_t i = 0; i < v.size(); ++i) {
    auto n = *begin(v[i]);
    if (n.rfind("departure ", 0) == 0) {
      answer *= x::to<uint64_t>(mw[i]);
    }
  }

  cout << answer << endl;
}
