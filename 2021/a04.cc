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
} // namespace x

using namespace x;

int
main()
{
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  string line;

  getline(cin, line);
  auto w = split(line);

  vector<int> numbers;
  numbers.reserve(w.size());
  transform(w.begin(), w.end(), back_inserter(numbers), [](auto sv) {
    return to<int>(sv);
  });

  constexpr auto N = 5uz;

  class Board
  {
  public:
    void Put(int x, size_t p)
    {
      pos.emplace(x, p);
      unmarked += x;
    }
    bool Mark(int x)
    {
      if (const auto it = pos.find(x); it != pos.end()) {
        auto row = it->second / N;
        auto col = it->second % N;
        unmarked -= x;
        auto in_row = ++rows[row];
        auto in_col = ++cols[col];
        return in_row == N || in_col == N;
      }
      return false;
    }
    int Unmarked() const { return unmarked; }

  private:
    unordered_map<int, size_t> pos;
    array<int, N> rows;
    array<int, N> cols;
    int unmarked = 0;
  };

  vector<Board> boards;

  int x;
  while (getline(cin, line)) {
    auto& board = boards.emplace_back();
    for (auto i = 0uz; i < N * N; ++i) {
      cin >> x;
      board.Put(x, i);
    }
    getline(cin, line);
  }

  set<const Board*> winners;

  for (int x : numbers) {
    for (auto& board : boards) {
      if (winners.count(&board) > 0)
        continue;
      if (board.Mark(x)) {
        if (winners.empty() || winners.size() == boards.size() - 1)
          cout << board.Unmarked() * x << endl;
        winners.insert(&board);
      }
    }
  }

  return 0;
}
