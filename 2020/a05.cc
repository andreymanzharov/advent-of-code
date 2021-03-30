#include <algorithm>
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

  int highest_seat_id = 0;
  vector<int> seats;

  string s;
  while (cin >> s) {
    int l = 0;
    int r = 128;
    for (int i = 0; i < 7; ++i) {
      int m = (l + r) / 2;
      if (s[i] == 'F') {
        r = m;
      } else {
        l = m;
      }
    }
    int row = l;
    l = 0;
    r = 8;
    for (int i = 0; i < 3; ++i) {
      int m = (l + r) / 2;
      if (s[7 + i] == 'L') {
        r = m;
      } else {
        l = m;
      }
    }
    int column = l;
    int seat_id = row * 8 + column;
    seats.push_back(seat_id);
    highest_seat_id = max(highest_seat_id, seat_id);
  }

  cout << highest_seat_id << endl;

  sort(begin(seats), end(seats));
  for (size_t i = 1; i < seats.size(); ++i) {
    if (seats[i] - 1 != seats[i - 1]) {
      cout << seats[i] - 1 << endl;
    }
  }
}
