#include "xxx.h"

using namespace std;
using namespace x;

struct pos
{
  pos(int r, int c)
    : r{ r }
    , c{ c }
  {
  }
  int r, c;
};

bool
operator<(const pos& lhs, const pos& rhs)
{
  return tie(lhs.r, lhs.c) < tie(rhs.r, rhs.c);
}

bool
operator==(const pos& lhs, const pos& rhs)
{
  return lhs.r == rhs.r && lhs.c == rhs.c;
}

ostream&
operator<<(ostream& out, const pos& p)
{
  return out << "{" << p.r << ", " << p.c << "}";
}

int
main()
{
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  multimap<pos, pair<int, int>> b;
  int m{}, n{};

  pos start{ 0, 0 }, finish{ 0, 0 };

  string line;
  for (int r{ 0 }; getline(cin, line); ++r) {
    for (int c{ 0 }; c < static_cast<int>(line.size()); ++c)
      switch (line[c]) {
        case '^':
          b.emplace(pos{ r, c }, pair{ -1, 0 });
          break;
        case 'v':
          b.emplace(pos{ r, c }, pair{ 1, 0 });
          break;
        case '<':
          b.emplace(pos{ r, c }, pair{ 0, -1 });
          break;
        case '>':
          b.emplace(pos{ r, c }, pair{ 0, 1 });
          break;
      }
    auto e =
      static_cast<int>(find(line.begin(), line.end(), '.') - line.begin());
    if (r == 0)
      start = pos{ 0, e };
    finish = pos{ r, e };

    m = r + 1;
    n = line.size();
  }

  vector<multimap<pos, pair<int, int>>> bs;
  bs.push_back(std::move(b));
  for (int i{ 1 }; i < m * n; ++i) {
    multimap<pos, pair<int, int>> b1;
    for (auto [p, d] : bs[i - 1]) {
      auto r = (p.r + d.first - 1 + m - 2) % (m - 2) + 1;
      auto c = (p.c + d.second - 1 + n - 2) % (n - 2) + 1;
      b1.emplace(pos{ r, c }, d);
    }
    if (b1 == bs.front()) {
      break;
    }
    bs.push_back(std::move(b1));
  }

  auto Go = [&bs, &m, &n](int t, pos start, pos finish) {
    constexpr array<pair<int, int>, 4> d4{
      { pair{ -1, 0 }, { 1, 0 }, { 0, -1 }, { 0, 1 } }
    };

    set<tuple<int, int, int>> v;
    priority_queue<tuple<int, int, int>,
                   vector<tuple<int, int, int>>,
                   greater<tuple<int, int, int>>>
      q;
    q.emplace(t, start.r, start.c);
    while (!q.empty()) {
      auto [k, r, c] = q.top();
      q.pop();
      auto [it, f] = v.emplace(r, c, k % static_cast<int>(bs.size()));
      if (!f)
        continue;
      auto nk{ (k + 1) % bs.size() };
      auto& b{ bs[nk] };
      for (auto [dr, dc] : d4) {
        pos np{ r + dr, c + dc };
        if (np.r == finish.r && np.c == finish.c) {
          return k + 1;
        }
        if (v.contains(tuple{ np.r, np.c, static_cast<int>(nk) }))
          continue;
        if (np.r > 0 && np.r < m - 1 && np.c > 0 && np.c < n - 1) {
          if (!b.contains(np)) {
            q.emplace(k + 1, np.r, np.c);
          }
        }
      }
      if (!b.contains(pos{ r, c })) {
        q.emplace(k + 1, r, c);
      }
    }
    return -1;
  };
  auto t1{ Go(0, start, finish) };
  cout << t1 << endl;
  auto t2{ Go(t1, finish, start) };
  auto t3{ Go(t2, start, finish) };
  cout << t3 << endl;

  return 0;
}
