#include "xxx.h"

using namespace std;
using namespace x;

int
main()
{
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  uint64_t sum{};

  string s;
  while (getline(cin, s)) {
    uint64_t n{};
    for (char c : s) {
      n *= 5;
      if (c == '=')
        n -= 2;
      else if (c == '-')
        n -= 1;
      else
        n += c - '0';
    }
    sum += n;
  }

  string t{};
  while (sum > 0) {
    auto x = sum % 5;
    sum /= 5;
    if (x < 3)
      t.push_back('0' + x);
    else {
      ++sum;
      if (x == 3)
        t.push_back('=');
      else
        t.push_back('-');
    }
  }
  reverse(t.begin(), t.end());
  cout << t << endl;

  return 0;
}
