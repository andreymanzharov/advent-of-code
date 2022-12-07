#include "xxx.h"

using namespace std;
using namespace x;

int
main()
{
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  vector<string> cwd;
  map<vector<string>, int> sizes;

  string line;
  getline(cin, line);
  while (cin) {
    auto args = split<string_view>(line);
    if (args[1] == "cd") {
      if (args[2].front() == '/') {
        cwd = {};
      } else if (args[2] == ".."sv) {
        cwd.pop_back();
      } else {
        cwd.emplace_back(args[2]);
      }
      getline(cin, line);
      continue;
    }
    if (args[1] == "ls"sv) {
      int size{};
      while (getline(cin, line)) {
        auto t = split<string_view>(line);
        if (t[0] == "$")
          break;
        if (t[0] != "dir"sv) {
          size += to<int>(t[0]);
        }
      }
      auto parents = cwd;
      while (!parents.empty()) {
        sizes[parents] += size;
        parents.pop_back();
      }
      sizes[{}] += size;
    }
  }

  int sum{};
  for (auto [dir, size] : sizes) {
    if (size <= 100000)
      sum += size;
  }
  cout << sum << endl;

  auto unused = 70000000 - sizes[{}];
  auto smallest = sizes[{}];
  for (auto [dir, size] : sizes) {
    if (size + unused >= 30000000) {
      smallest = min(smallest, size);
    }
  }
  cout << smallest << endl;

  return 0;
}
