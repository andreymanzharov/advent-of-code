#include "xxx.h"

using namespace std;
using namespace x;

int
main()
{
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  using expr = tuple<char, string, string>;
  using value = variant<long, expr>;

  map<string, value> monkeys;

  string line;
  while (getline(cin, line)) {
    auto w = split(line);
    string name(w[0].begin(), prev(w[0].end()));
    if (w.size() == 2) {
      monkeys[std::move(name)] = to<long>(w[1]);
    } else {
      monkeys[std::move(name)] =
        expr{ w[2].front(), string(w[1]), string(w[3]) };
    }
  }

  auto CalculateAndOptimize = [&]() {
    function<pair<long, bool>(string)> Inner = [&](auto name) {
      auto& v = monkeys.at(name);
      if (holds_alternative<long>(v))
        return pair{ get<long>(v), false };
      auto [op, lhs, rhs] = get<expr>(v);
      auto [arg1, d1] = Inner(lhs);
      auto [arg2, d2] = Inner(rhs);
      long r;
      switch (op) {
        case '+':
          r = arg1 + arg2;
          break;
        case '-':
          r = arg1 - arg2;
          break;
        case '*':
          r = arg1 * arg2;
          break;
        case '/':
          r = arg1 / arg2;
          break;
        default:
          cout << op << endl;
      }
      auto d = d1 || d2 || lhs == "humn" || rhs == "humn";
      if (!d)
        v = r;
      return pair{ r, d };
    };
    return Inner("root").first;
  };

  cout << CalculateAndOptimize() << endl;

  function<long(string)> Calculate = [&](const string& name) {
    auto& v = monkeys.at(name);
    if (holds_alternative<long>(v))
      return get<long>(v);
    auto [op, lhs, rhs] = get<expr>(v);
    switch (op) {
      case '+':
        return Calculate(lhs) + Calculate(rhs);
      case '-':
        return Calculate(lhs) - Calculate(rhs);
      case '*':
        return Calculate(lhs) * Calculate(rhs);
      case '/':
        return Calculate(lhs) / Calculate(rhs);
    }
    return 0L;
  };

  auto& [op, lhs, rhs] = get<expr>(monkeys["root"]);
  op = '-';

  auto left{ -4'000'000'000'000 };
  auto right{ 4'000'000'000'000 };

  while (left + 2 < right) {
    auto m1 = left + (right - left) / 3;
    auto m2 = left + (right - left) / 3 * 2;
    monkeys["humn"] = m1;
    auto x1 = Calculate("root");
    if (x1 == 0) {
      cout << m1 << endl;
      break;
    }
    monkeys["humn"] = m2;
    auto x2 = Calculate("root");
    if (x2 == 0) {
      cout << m2 << endl;
      break;
    }
    if (abs(x1) <= abs(x2)) {
      right = m2;
    } else {
      left = m1;
    }
  }

  return 0;
}
