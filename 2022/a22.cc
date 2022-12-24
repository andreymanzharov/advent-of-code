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

  vector<tuple<int, int, int, function<void(int&, int&, int&, int&)>>> vt1 = {
    tuple{ 49,
           0,
           50,
           []([[maybe_unused]] int& r,
              int& c,
              [[maybe_unused]] int& dr,
              [[maybe_unused]] int& dc) { c = 149; } },
    tuple{ 150,
           0,
           50,
           []([[maybe_unused]] int& r,
              int& c,
              [[maybe_unused]] int& dr,
              [[maybe_unused]] int& dc) { c = 50; } },
    tuple{ 49,
           50,
           100,
           []([[maybe_unused]] int& r,
              int& c,
              [[maybe_unused]] int& dr,
              [[maybe_unused]] int& dc) { c = 99; } },
    tuple{ 100,
           50,
           100,
           []([[maybe_unused]] int& r,
              int& c,
              [[maybe_unused]] int& dr,
              [[maybe_unused]] int& dc) { c = 50; } },
    tuple{ -1,
           100,
           150,
           []([[maybe_unused]] int& r,
              int& c,
              [[maybe_unused]] int& dr,
              [[maybe_unused]] int& dc) { c = 99; } },
    tuple{ 100,
           100,
           150,
           []([[maybe_unused]] int& r,
              int& c,
              [[maybe_unused]] int& dr,
              [[maybe_unused]] int& dc) { c = 0; } },
    tuple{ -1,
           150,
           200,
           []([[maybe_unused]] int& r,
              int& c,
              [[maybe_unused]] int& dr,
              [[maybe_unused]] int& dc) { c = 49; } },
    tuple{ 50,
           150,
           200,
           []([[maybe_unused]] int& r,
              int& c,
              [[maybe_unused]] int& dr,
              [[maybe_unused]] int& dc) { c = 0; } },
  };
  vector<tuple<int, int, int, function<void(int&, int&, int&, int&)>>> ht1 = {
    tuple{ 99,
           0,
           50,
           [](int& r,
              [[maybe_unused]] int& c,
              [[maybe_unused]] int& dr,
              [[maybe_unused]] int& dc) { r = 199; } },
    tuple{ 200,
           0,
           50,
           [](int& r,
              [[maybe_unused]] int& c,
              [[maybe_unused]] int& dr,
              [[maybe_unused]] int& dc) { r = 100; } },
    tuple{ -1,
           50,
           100,
           [](int& r,
              [[maybe_unused]] int& c,
              [[maybe_unused]] int& dr,
              [[maybe_unused]] int& dc) { r = 149; } },
    tuple{ 150,
           50,
           100,
           [](int& r,
              [[maybe_unused]] int& c,
              [[maybe_unused]] int& dr,
              [[maybe_unused]] int& dc) { r = 0; } },
    tuple{ -1,
           100,
           150,
           [](int& r,
              [[maybe_unused]] int& c,
              [[maybe_unused]] int& dr,
              [[maybe_unused]] int& dc) { r = 49; } },
    tuple{ 50,
           100,
           150,
           [](int& r,
              [[maybe_unused]] int& c,
              [[maybe_unused]] int& dr,
              [[maybe_unused]] int& dc) { r = 0; } },
  };
  vector<tuple<int, int, int, function<void(int&, int&, int&, int&)>>> vt2 = {
    tuple{ 49,
           0,
           50,
           [](int& r, int& c, int& dr, int& dc) {
             assert(dr == 0 && dc == -1);
             r = 150 - r - 1;
             c = 0;
             dr = 0;
             dc = 1;
           } },
    tuple{ 150,
           0,
           50,
           [](int& r, int& c, int& dr, int& dc) {
             assert(dr == 0 && dc == 1);
             r = 150 - r - 1;
             c = 99;
             dr = 0;
             dc = -1;
           } },
    tuple{ 49,
           50,
           100,
           [](int& r, int& c, int& dr, int& dc) {
             assert(dr == 0 && dc == -1);
             c = r - 50;
             r = 100;
             dr = 1;
             dc = 0;
           } },
    tuple{ 100,
           50,
           100,
           [](int& r, int& c, int& dr, int& dc) {
             assert(dr == 0 && dc == 1);
             c = r + 50;
             r = 49;
             dr = -1;
             dc = 0;
           } },
    tuple{ -1,
           100,
           150,
           [](int& r, int& c, int& dr, int& dc) {
             assert(dr == 0 && dc == -1);
             r = 150 - r - 1;
             c = 50;
             dr = 0;
             dc = 1;
           } },
    tuple{ 100,
           100,
           150,
           [](int& r, int& c, int& dr, int& dc) {
             assert(dr == 0 && dc == 1);
             r = 150 - r - 1;
             c = 149;
             dr = 0;
             dc = -1;
           } },
    tuple{ -1,
           150,
           200,
           [](int& r, int& c, int& dr, int& dc) {
             assert(dr == 0 && dc == -1);
             c = (r - 150) + 50;
             r = 0;
             dr = 1;
             dc = 0;
           } },
    tuple{ 50,
           150,
           200,
           [](int& r, int& c, int& dr, int& dc) {
             assert(dr == 0 && dc == 1);
             c = (r - 150) + 50;
             r = 149;
             dr = -1;
             dc = 0;
           } },
  };
  vector<tuple<int, int, int, function<void(int&, int&, int&, int&)>>> ht2 = {
    tuple{ 99,
           0,
           50,
           [](int& r, int& c, int& dr, int& dc) {
             assert(dr == -1 && dc == 0);
             r = c + 50;
             c = 50;
             dr = 0;
             dc = 1;
           } },
    tuple{ 200,
           0,
           50,
           [](int& r, int& c, int& dr, int& dc) {
             assert(dr == 1 && dc == 0);
             r = 0;
             c = c + 100;
             dr = 1;
             dc = 0;
           } },
    tuple{ -1,
           50,
           100,
           [](int& r, int& c, int& dr, int& dc) {
             assert(dr == -1 && dc == 0);
             r = c + 100;
             c = 0;
             dr = 0;
             dc = 1;
           } },
    tuple{ 150,
           50,
           100,
           [](int& r, int& c, int& dr, int& dc) {
             assert(dr == 1 && dc == 0);
             r = c + 100;
             c = 49;
             dr = 0;
             dc = -1;
           } },
    tuple{ -1,
           100,
           150,
           [](int& r, int& c, int& dr, int& dc) {
             assert(dr == -1 && dc == 0);
             c = c - 100;
             r = 199;
             dr = -1;
             dc = 0;
           } },
    tuple{ 50,
           100,
           150,
           [](int& r, int& c, int& dr, int& dc) {
             assert(dr == 1 && dc == 0);
             r = c - 50;
             c = 99;
             dr = 0;
             dc = -1;
           } },
  };

  auto Go = [&path, &m](auto vt, auto ht) {
    int r{ 0 }, c{ 0 };
    while (c < static_cast<int>(m[r].size()) && m[r][c] != '.')
      ++c;

    auto i{ path.begin() };

    int dr{ 0 }, dc{ 1 };
    while (i != path.end()) {
      auto j{ i };
      while (j != path.end() && '0' <= *j && *j <= '9')
        ++j;
      int k;
      auto [p, ec] = from_chars(&*i, &*j, k);
      assert(p == &*j);
      i = j;
      while (k-- > 0) {
        assert(m[r][c] != ' ' && m[r][c] != '#');
        m[r][c] = dr == 0 ? (dc < 0 ? '<' : '>') : (dr < 0 ? '^' : 'v');
        if (dr == 0) {
          auto nr{ r + dr }, nc{ c + dc }, ndr{ dr }, ndc{ dc };
          for (auto& [bc, lr, rr, f] : vt)
            if (nc == bc && lr <= nr && nr < rr)
              f(nr, nc, ndr, ndc);
          if (m[nr][nc] == '#')
            break;
          r = nr;
          c = nc;
          dr = ndr;
          dc = ndc;
        } else {
          auto nr{ r + dr }, nc{ c + dc }, ndr{ dr }, ndc{ dc };
          for (auto& [br, lc, rc, f] : ht)
            if (nr == br && lc <= nc && nc < rc)
              f(nr, nc, ndr, ndc);
          if (m[nr][nc] == '#')
            break;
          r = nr;
          c = nc;
          dr = ndr;
          dc = ndc;
        }
      }
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
        default:
          assert(false);
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

    return 1000 * (r + 1) + 4 * (c + 1) + f;
  };

  cout << Go(vt1, ht1) << endl;
  cout << Go(vt2, ht2) << endl;

  return 0;
}
