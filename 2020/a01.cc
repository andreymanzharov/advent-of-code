#include <iostream>
#include <vector>

using namespace std;

int main()
{
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  vector<int> a;
  int x;
  while (cin >> x) {
    a.push_back(x);
  }
  for (size_t i = 0; i < a.size(); ++i) {
    for (size_t j = i + 1; j < a.size(); ++j) {
      for (size_t k = j + 1; k < a.size(); ++k) {
        if (a[i] + a[j] + a[k] == 2020) {
          cout << a[i] * a[j] * a[k] << endl;
        }
      }
    }
  }
}
