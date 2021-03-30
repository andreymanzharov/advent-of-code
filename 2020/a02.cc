#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

int
main()
{
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int valid = 0;

  string line;
  while (getline(cin, line)) {
    istringstream is(line);
    int l;
    is >> l;
    is.ignore(1);
    int r;
    is >> r;
    char ch;
    is >> ch;
    is.ignore(1);
    string password;
    is >> password;

    if ((password[l - 1] == ch) != (password[r - 1] == ch)) {
      ++valid;
    }
  }

  cout << valid << endl;
}
