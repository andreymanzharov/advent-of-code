#include <algorithm>
#include <cctype>
#include <charconv>
#include <iostream>
#include <set>
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

  set<char> questions;

  int counts = 0;

  bool first_in_group = true;

  string line;
  while (getline(cin, line)) {
    if (line.empty()) {
      counts += questions.size();
      questions.clear();
      first_in_group = true;
    } else {
      set<char> person_questions(begin(line), end(line));
      if (first_in_group) {
        questions = person_questions;
        first_in_group = false;
      } else {
        vector<char> intersection;
        set_intersection(begin(questions),
                         end(questions),
                         begin(person_questions),
                         end(person_questions),
                         back_inserter(intersection));
        questions = set<char>(begin(intersection), end(intersection));
      }
    }
  }
  counts += questions.size();
  cout << counts << endl;
}
