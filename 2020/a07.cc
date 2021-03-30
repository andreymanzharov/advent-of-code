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

  unordered_map<string, vector<string>> contain_in;
  unordered_map<string, vector<pair<int, string>>> contain;

  string line;
  while (getline(cin, line)) {
    auto v = x::split(line);
    string in = string(v[0]) + ' ' + string(v[1]);
    for (size_t i = 4; i < v.size() && v[i] != "no"; i += 4) {
      string bag = string(v[i + 1]) + ' ' + string(v[i + 2]);
      contain_in[bag].push_back(in);
      contain[in].emplace_back(x::to<int>(v[i]), bag);
    }
  }

  unordered_set<string_view> counted;

  function<int(const string&)> Count = [&](const string& bag) {
    if (counted.find(bag) != end(counted))
      return 0;
    counted.insert(bag);
    int r = 0;
    for (const auto& other : contain_in[bag]) {
      r += Count(other);
    }
    return r + 1;
  };

  function<int(const string&)> Required = [&](const string& bag) {
    int r = 0;
    for (const auto& [count, name] : contain[bag]) {
      r += count * Required(name);
    }
    return r + 1;
  };

  cout << Count("shiny gold") - 1 << endl;
  cout << Required("shiny gold") - 1 << endl;
}
