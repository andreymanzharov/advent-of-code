#include "xxx.h"
#include <unistd.h>

using namespace std;
using namespace x;

int
main()
{
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  string line;

  vector<string> m;
  while (getline(cin, line)) {
    if (line.empty())
      break;
    m.push_back(std::move(line));
  }

  string path;
  getline(cin, path);

  int r{ 0 }, c{ 0 };
  while (c < static_cast<int>(m[r].size()) && m[r][c] != '.')
    ++c;

  // vector<tuple<int, int, int, function<void(int&, int&, int&, int&)>>> vt = {
  //   tuple{ 49, 0, 50, [](int& r, int& c, int& dr, int& dc) { c = 149; } },
  //   tuple{ 150, 0, 50, [](int& r, int& c, int& dr, int& dc) { c = 50; } },
  //   tuple{ 49, 50, 100, [](int& r, int& c, int& dr, int& dc) { c = 99; } },
  //   tuple{ 100, 50, 100, [](int& r, int& c, int& dr, int& dc) { c = 50; } },
  //   tuple{ -1, 100, 150, [](int& r, int& c, int& dr, int& dc) { c = 99; } },
  //   tuple{ 100, 100, 150, [](int& r, int& c, int& dr, int& dc) { c = 0; } },
  //   tuple{ -1, 150, 200, [](int& r, int& c, int& dr, int& dc) { c = 49; } },
  //   tuple{ 50, 150, 200, [](int& r, int& c, int& dr, int& dc) { c = 0; } },
  // };
  // vector<tuple<int, int, int, function<void(int&, int&, int&, int&)>>> ht = {
  //   tuple{ 99, 0, 50, [](int& r, int& c, int& dr, int& dc) { r = 199; } },
  //   tuple{ 200, 0, 50, [](int& r, int& c, int& dr, int& dc) { r = 100; } },
  //   tuple{ -1, 50, 100, [](int& r, int& c, int& dr, int& dc) { r = 149; } },
  //   tuple{ 150, 50, 100, [](int& r, int& c, int& dr, int& dc) { r = 0; } },
  //   tuple{ -1, 100, 150, [](int& r, int& c, int& dr, int& dc) { r = 49; } },
  //   tuple{ 50, 100, 150, [](int& r, int& c, int& dr, int& dc) { r = 0; } },
  // };
  vector<tuple<int, int, int, function<void(int&, int&, int&, int&)>>> vt = {
    tuple{ 49,
           0,
           50,
           [](int& r, int& c, int& dr, int& dc) {
             r = 150 - r - 1;
             c = 0;
             dr = 0;
             dc = 1;
           } },
    tuple{ 150,
           0,
           50,
           [](int& r, int& c, int& dr, int& dc) {
             r = 150 - r - 1;
             c = 99;
             dr = 0;
             dc = -1;
           } },
    tuple{ 49,
           50,
           100,
           [](int& r, int& c, int& dr, int& dc) {
             c = r - 50;
             r = 100;
             dr = 1;
             dc = 0;
           } },
    tuple{ 100,
           50,
           100,
           [](int& r, int& c, int& dr, int& dc) {
             c = r + 50;
             r = 49;
             dr = -1;
             dc = 0;
           } },
    tuple{ -1,
           100,
           150,
           [](int& r, int& c, int& dr, int& dc) {
             r = 150 - r - 1;
             c = 50;
             dr = 0;
             dc = 1;
           } },
    tuple{ 100,
           100,
           150,
           [](int& r, int& c, int& dr, int& dc) {
             r = 50 - (r - 100) - 1;
             c = 149;
             dr = 0;
             dc = -1;
           } },
    tuple{ -1,
           150,
           200,
           [](int& r, int& c, int& dr, int& dc) {
             c = (r - 150) + 50;
             r = 0;
             dr = 1;
             dc = 0;
           } },
    tuple{ 50,
           150,
           200,
           [](int& r, int& c, int& dr, int& dc) {
             c = (r - 150) + 50;
             r = 149;
             dr = -1;
             dc = 0;
           } },
  };
  vector<tuple<int, int, int, function<void(int&, int&, int&, int&)>>> ht = {
    tuple{ 99,
           0,
           50,
           [](int& r, int& c, int& dr, int& dc) {
             r = c + 50;
             c = 50;
             dr = 0;
             dc = 1;
           } },
    tuple{ 200,
           0,
           50,
           [](int& r, int& c, int& dr, int& dc) {
             r = 0;
             c = c + 100;
             dr = 1;
             dc = 0;
           } },
    tuple{ -1,
           50,
           100,
           [](int& r, int& c, int& dr, int& dc) {
             r = c + 100;
             c = 0;
             dr = 0;
             dc = 1;
           } },
    tuple{ 150,
           50,
           100,
           [](int& r, int& c, int& dr, int& dc) {
             r = c + 100;
             c = 49;
             dr = 0;
             dc = -1;
           } },
    tuple{ -1,
           100,
           150,
           [](int& r, int& c, int& dr, int& dc) {
             c = c - 100;
             r = 199;
             dr = -1;
             dc = 0;
           } },
    tuple{ 50,
           100,
           150,
           [](int& r, int& c, int& dr, int& dc) {
             r = c - 50;
             c = 99;
             dr = 0;
             dc = -1;
           } },
  };

  auto i{ path.begin() };

  int dr{ 0 }, dc{ 1 };
  while (i != path.end()) {
    auto j{ i };
    while (j != path.end() && '0' <= *j && *j <= '9')
      ++j;
    int k;
    from_chars(&*i, &*j, k);
    i = j;
    while (k-- > 0) {
      // dbg(r, c, dr, dc);
      assert(m[r][c] != ' ' && m[r][c] != '#');
      m[r][c] = dr == 0 ? (dc < 0 ? '<' : '>') : (dr < 0 ? '^' : 'v');
      if (dr == 0) {
        auto nr = r + dr;
        auto nc = c + dc;
        for (auto& [bc, lr, rr, f] : vt)
          if (nc == bc && lr <= nr && nr < rr) {
            f(nr, nc, dr, dc);
            break;
          }
        if (m[nr][nc] == '#')
          break;
        r = nr;
        c = nc;
      } else {
        auto nr = r + dr;
        auto nc = c + dc;
        for (auto& [br, lc, rc, f] : ht)
          if (nr == br && lc <= nc && nc < rc) {
            f(nr, nc, dr, dc);
            break;
          }
        if (m[nr][nc] == '#')
          break;
        r = nr;
        c = nc;
      }
    }
    m[r][c] = dr == 0 ? (dc < 0 ? '<' : '>') : (dr < 0 ? '^' : 'v');
    if (i == path.end())
      break;
    switch (*i++) {
      case 'R': {
        auto t{ dr };
        dr = dc;
        dc = -t;
      } break;
      case 'L': {
        auto t{ dr };
        dr = -dc;
        dc = t;
      } break;
    }

    for (auto& r : m) {
      for (char c : r) {
        if (c == ' ' || c == '.' || c == '#')
          cout << c;
        else
          cout << "\033[0;31m" << c << "\033[0m";
      }
      cout << '\n';
    }
  }

  int f{};
  if (dr == 0 && dc == 1) {
    f = 0;
  }
  if (dr == 0 && dc == -1) {
    f = 2;
  }
  if (dr == 1 && dc == 0) {
    f = 1;
  }
  if (dr == -1 && dc == 0) {
    f = 3;
  }

  dbg(r, c, f);
  cout << 1000 * (r + 1) + 4 * (c + 1) + f << endl;

  return 0;
}
