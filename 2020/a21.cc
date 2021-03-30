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

vector<string_view>
split(string_view sv, string_view delimiters = ", "sv)
{
  vector<string_view> parts;
  while (!sv.empty()) {
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

  map<string, int> all;
  map<string, set<string>> contains_in;

  string line;
  while (getline(cin, line)) {
    const auto w = x::split(line, "()");

    set<string> ingredients;
    for (auto word : x::split(w[0])) {
      string ingredient(word);
      ingredients.insert(ingredient);
      ++all[ingredient];
    }

    const auto allergens = x::split(w[1]);
    for (size_t i = 1; i < allergens.size(); ++i) {
      string allergen(allergens[i]);
      if (const auto it = contains_in.find(string(allergen));
          it != end(contains_in)) {
        vector<string> intersection;
        set_intersection(begin(ingredients),
                         end(ingredients),
                         begin(it->second),
                         end(it->second),
                         back_inserter(intersection));
        it->second = set<string>(begin(intersection), end(intersection));
      } else {
        contains_in[string(allergen)].insert(begin(ingredients),
                                             end(ingredients));
      }
    }
  }
  map<string, string> m;
  for (size_t i = 0; i < contains_in.size(); ++i) {
    for (const auto& [allergen, ingredients] : contains_in) {
      if (ingredients.size() == 1) {
        m.insert({ allergen, *begin(ingredients) });
        for (auto& [a, is] : contains_in) {
          if (allergen != a) {
            is.erase(*begin(ingredients));
          }
        }
      }
    }
  }
  int s = 0;
  for (const auto& [_, count] : all) {
    s += count;
  }
  for (const auto& [_, ingredient] : m) {
    s -= all[ingredient];
  }

  cout << s << endl;

  bool first = true;
  for (const auto& [_, ingredient] : m) {
    if (first) {
      first = false;
    } else {
      cout << ',';
    }
    cout << ingredient;
  }
  cout << endl;
}
