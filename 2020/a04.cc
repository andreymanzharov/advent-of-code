#include <cctype>
#include <charconv>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;

template<typename T>
auto
__dbg(T&& t, string_view hint)
{
  cerr << hint << ": " << t << endl;
  return forward<T>(t);
}
#define dbg(x) __dbg(x, #x)

int
main()
{
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  unordered_map<string, string> fields;

  auto CheckRange = [](string_view value, size_t length, int least, int most) {
    int number = 0;
    from_chars(value.data(), value.data() + value.size(), number);
    return value.size() == length && least <= number && number <= most;
  };

  auto CheckHeight = [](string_view value) {
    int number = 0;
    auto [p, ec] =
      from_chars(value.data(), value.data() + value.size(), number);
    auto suffix = string_view(p, value.data() + value.size() - p);
    return (suffix == "cm" && 150 <= number && number <= 193) ||
           (suffix == "in" && 59 <= number && number <= 76);
  };

  auto CheckHairColor = [](string_view value) {
    int number = 0;
    auto [p, ec] =
      from_chars(value.data() + 1, value.data() + value.size(), number, 16);
    return value.size() == 7 && value.front() == '#' &&
           p == value.data() + value.size();
  };

  const unordered_set<string_view> colors = { "amb", "blu", "brn", "gry",
                                              "grn", "hzl", "oth" };
  auto CheckEyeColor = [&](string_view value) {
    return colors.count(value) > 0;
  };

  auto IsValid = [&]() {
    return CheckRange(fields["byr"], 4, 1920, 2002) &&
           CheckRange(fields["iyr"], 4, 2010, 2020) &&
           CheckRange(fields["eyr"], 4, 2020, 2030) &&
           CheckHeight(fields["hgt"]) && CheckHairColor(fields["hcl"]) &&
           CheckEyeColor(fields["ecl"]) &&
           CheckRange(fields["pid"], 9, 0, 999'999'999);
  };

  int valid = 0;
  while (cin) {
    string line;
    while (getline(cin, line)) {
      if (line.empty()) {
        if (IsValid()) {
          ++valid;
        }
        fields.clear();
      } else {
        istringstream is(line);
        string key;
        while (getline(is, key, ':')) {
          string value;
          getline(is, value, ' ');
          fields[move(key)] = move(value);
        }
      }
    }
  }
  if (!fields.empty() && IsValid()) {
    ++valid;
  }
  cout << valid << endl;
}
