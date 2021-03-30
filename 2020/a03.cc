#include <iostream>
#include <string>
#include <utility>
#include <vector>

using namespace std;

int
main()
{
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  vector<string> m;
  string line;
  while (getline(cin, line)) {
    m.push_back(move(line));
  }
  int ans = 1;
  vector<pair<int, int>> d = {
    { 1, 1 }, { 3, 1 }, { 5, 1 }, { 7, 1 }, { 1, 2 }
  };
  for (auto [dc, dr] : d) {
    size_t r = 0;
    size_t c = 0;
    int tree = 0;
    while (r < m.size()) {
      if (m[r][c] == '#') {
        ++tree;
      }
      c = (c + dc) % m[0].size();
      r = r + dr;
    }
    cout << tree << endl;
    ans *= tree;
  }
  cout << ans << endl;
}
