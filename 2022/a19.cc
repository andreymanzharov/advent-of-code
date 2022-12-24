#include "xxx.h"

using namespace std;
using namespace x;

struct S
{
  uint32_t ore : 8;
  uint32_t clay : 8;
  uint32_t obsidian : 8;
  uint32_t geode : 8;
};

bool
operator<(const S& lhs, const S& rhs)
{
  return tie(lhs.geode, lhs.obsidian, lhs.clay, lhs.ore) <
         tie(rhs.geode, rhs.obsidian, rhs.clay, rhs.ore);
}

int
main()
{
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int sum{};
  int mul{ 1 };

  int id{};

  string line;
  while (getline(cin, line)) {
    auto w{ split(line) };
    auto ore_ore{ to<uint32_t>(w[6]) };
    auto ore_clay{ to<uint32_t>(w[12]) };
    auto ore_obsidian{ to<uint32_t>(w[18]) };
    auto clay_obsidian{ to<uint32_t>(w[21]) };
    auto ore_geode{ to<uint32_t>(w[27]) };
    auto obsidian_geode{ to<uint32_t>(w[30]) };

    auto max_ore{ max(ore_ore, max(ore_clay, max(ore_obsidian, ore_geode))) };

    set<pair<S, S>> mm;
    set<pair<S, S>> m;
    m.emplace(S{ 0, 0, 0, 0 }, S{ 1, 0, 0, 0 });
    for (int i{}; i < 24; ++i) {
      set<pair<S, S>> m1;

      auto Put = [&](S r, S n) {
        if (mm.contains({ r, n }))
          return;
        mm.emplace(r, n);
        m1.emplace(r, n);
      };

      auto PutOre = [&](S r, S n) {
        r.ore -= ore_ore;
        n.ore += 1;
        Put(r, n);
      };

      auto PutClay = [&](S r, S n) {
        r.ore -= ore_clay;
        n.clay += 1;
        Put(r, n);
      };

      auto PutObsidian = [&](S r, S n) {
        r.ore -= ore_obsidian;
        r.clay -= clay_obsidian;
        n.obsidian += 1;
        Put(r, n);
      };

      auto PutGeode = [&](S r, S n) {
        r.ore -= ore_geode;
        r.obsidian -= obsidian_geode;
        n.geode += 1;
        Put(r, n);
      };

      cout << "i: " << i << ' ' << m.size() << endl;

      for (auto [r, n] : m) {
        r.ore += n.ore;
        r.clay += n.clay;
        r.obsidian += n.obsidian;
        r.geode += n.geode;
        if (r.obsidian - n.ore >= obsidian_geode &&
            r.ore - n.ore >= ore_geode) {
          PutGeode(r, n);
        }
        if (n.obsidian < obsidian_geode && r.clay - n.clay >= clay_obsidian &&
            r.ore - n.ore >= ore_obsidian)
          PutObsidian(r, n);
        if (n.clay < clay_obsidian && r.ore - n.ore >= ore_clay)
          PutClay(r, n);
        if (n.ore < max_ore && r.ore - n.ore >= ore_ore)
          PutOre(r, n);
        if (n.ore < max_ore || n.clay < clay_obsidian ||
            n.obsidian < obsidian_geode)
          Put(r, n);
      }
      mm = std::move(m);
      m = std::move(m1);
    }
    uint32_t mx{};
    for (auto& [r, n] : m) {
      mx = max(mx, r.geode);
    }

    sum += ++id * mx;
    mul *= mx;

    // if (id == 3)
    //   break;
  }

  cout << sum << endl;
  cout << mul << endl;

  return 0;
}
