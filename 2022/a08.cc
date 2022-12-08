#include "xxx.h"

using namespace std;
using namespace x;

int
main()
{
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  string line;

  vector<string> map;
  while (getline(cin, line)) {
    map.push_back(std::move(line));
  }

  set<pair<size_t, size_t>> visible;

  auto Scan = [&](size_t i, size_t j, ssize_t di, ssize_t dj) {
    char mx{ 0 };
    while (i < map.size() && j < map[i].size()) {
      if (map[i][j] > mx) {
        mx = map[i][j];
        visible.emplace(i, j);
      }
      i += di;
      j += dj;
    }
  };

  for (size_t i{}; i < map.size(); ++i) {
    Scan(i, 0, 0, 1);
  }
  for (size_t j{}; j < map[0].size(); ++j) {
    Scan(0, j, 1, 0);
  }
  for (size_t i{}; i < map.size(); ++i) {
    Scan(i, map[i].size() - 1, 0, -1);
  }
  for (size_t j{}; j < map[map.size() - 1].size(); ++j) {
    Scan(map.size() - 1, j, -1, 0);
  }

  cout << visible.size() << endl;

  int highest{};
  for (size_t i{ 1 }; i < map.size() - 1; ++i) {
    for (size_t j{ 1 }; j < map[i].size() - 1; ++j) {
      int score = 1;
      for (auto [di, dj] : { pair{ 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } }) {
        int k{ 1 };
        for (size_t si{ i + di }, sj{ j + dj };
             0 < si && si < map.size() - 1 && 0 < sj &&
             sj < map[si].size() - 1 && map[si][sj] < map[i][j];
             si += di, sj += dj) {
          k += 1;
        }
        score *= k;
      }
      highest = max(highest, score);
    }
  }

  cout << highest << endl;

  return 0;
}
