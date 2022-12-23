#include "xxx.h"

using namespace std;
using namespace x;

struct pos
{
  pos(size_t r, size_t c)
    : r{ r }
    , c{ c }
  {
  }
  const size_t r, c;
};

bool
operator<(const pos& lhs, const pos& rhs)
{
  return tie(lhs.r, lhs.c) < tie(rhs.r, rhs.c);
}

int
main()
{
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  set<pos> elves;

  constexpr int OFFSET{ 100'000 };

  string line;
  for (auto r{ 0uz }; getline(cin, line); ++r)
    for (auto c{ 0uz }; c < line.size(); ++c)
      if (line[c] == '#')
        elves.emplace(r + OFFSET, c + OFFSET);

  array<pair<int, int>, 8> d8{ pair{ -1, -1 }, { -1, 0 }, { -1, 1 }, { 0, -1 },
                               { 0, 1 },       { 1, -1 }, { 1, 0 },  { 1, 1 } };
  array<pair<int, int>, 4> d4{ pair{ -1, 0 }, { 1, 0 }, { 0, -1 }, { 0, 1 } };
  array<array<pair<int, int>, 3>, 4> d4x3{ {
    { pair{ -1, -1 }, { -1, 0 }, { -1, 1 } },
    { pair{ 1, -1 }, { 1, 0 }, { 1, 1 } },
    { pair{ -1, -1 }, { 0, -1 }, { 1, -1 } },
    { pair{ -1, 1 }, { 0, 1 }, { 1, 1 } },
  } };

  auto EmptyCount = [&]() {
    auto pr = minmax_element(
      elves.begin(), elves.end(), [](auto a, auto b) { return a.r < b.r; });
    auto mnr{ pr.first->r }, mxr{ pr.second->r };
    auto pc = minmax_element(
      elves.begin(), elves.end(), [](auto a, auto b) { return a.c < b.c; });
    auto mnc{ pc.first->c }, mxc{ pc.second->c };
    int count{};
    for (auto r{ mnr }; r <= mxr; ++r)
      for (auto c{ mnc }; c <= mxc; ++c)
        if (!elves.contains(pos{ r, c }))
          ++count;
    return count;
  };

  int round{};
  while (true) {
    map<pos, int> m;
    vector<pair<pos, pos>> n;
    bool any_moved{};
    for (auto [r, c] : elves) {
      bool moved{};
      for (auto [dr, dc] : d8) {
        if (elves.contains(pos{ r + dr, c + dc })) {
          for (int i{}; i < 4; ++i) {
            int j{ (round + i) % 4 };
            if (
              all_of(
                d4x3[j].begin(),
                d4x3[j].end(),
                [row{ r }, col{ c }, &elves](auto d) {
                  return !elves.contains(pos{ row + d.first, col + d.second });
                })) {
              pos next{ r + d4[j].first, c + d4[j].second };
              ++m[next];
              n.emplace_back(pos{ r, c }, next);
              any_moved = moved = true;
              break;
            }
          }
          break;
        }
      }
      if (!moved) {
        n.emplace_back(pos{ r, c }, pos{ r, c });
      }
    }
    if (!any_moved)
      break;

    set<pos> next_elves;
    for (auto [old, next] : n)
      next_elves.insert(m[next] > 1 ? old : next);

    elves = std::move(next_elves);

    if (++round == 10)
      cout << EmptyCount() << endl;
  }

  cout << round + 1 << endl;

  return 0;
}
