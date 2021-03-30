#define DBG_MACRO_NO_WARNING
#include "dbg.h"

#include <algorithm>
#include <array>
#include <cassert>
#include <cctype>
#include <charconv>
#include <cmath>
#include <deque>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <string_view>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;

namespace x {

template<typename T>
T
to(string_view from)
{
  T r{};
  if (auto [p, ec] = from_chars(from.data(), from.data() + from.size(), r);
      p != from.data() + from.size()) {
    ostringstream message;
    message << "cannot convert " << from;
    throw invalid_argument(message.str());
  }
  return r;
}

vector<string_view>
split(string_view sv, string_view delimiters = ", "sv)
{
  vector<string_view> parts;
  while (true) {
    auto p = sv.find_first_of(delimiters);
    if (p > 0)
      parts.push_back(sv.substr(0, p));
    if (p == string_view::npos)
      break;
    sv.remove_prefix(p + 1);
  }
  return parts;
}
}

class Tile
{
public:
  Tile(int id, const vector<string>& pixels)
    : id(id)
    , pixels(pixels)
  {
    for (size_t i = 0; i < pixels.size(); ++i) {
      Top().push_back(pixels.front()[i]);
      Right().push_back(pixels[i].back());
      Bottom().push_back(pixels.back()[i]);
      Left().push_back(pixels[i].front());
    }
  }

  void Rotate()
  {
    rotate(begin(borders), begin(borders) + 1, end(borders));
    reverse(begin(Left()), end(Left()));
    reverse(begin(Right()), end(Right()));

    vector<string> rotated_pixels = pixels;
    for (size_t i = 0; i < pixels.size(); ++i) {
      for (size_t j = 0; j < pixels.size(); ++j) {
        rotated_pixels[i][j] = pixels[j][pixels[j].size() - i - 1];
      }
    }
    pixels = move(rotated_pixels);

    assert(Top() == pixels.front());
    assert(Bottom() == pixels.back());
  }

  Tile FlipV() const
  {
    Tile result = *this;
    swap(result.Top(), result.Bottom());
    reverse(begin(result.Left()), end(result.Left()));
    reverse(begin(result.Right()), end(result.Right()));
    reverse(begin(result.pixels), end(result.pixels));
    return result;
  }

  Tile FlipH() const
  {
    Tile result = *this;
    swap(result.Left(), result.Right());
    reverse(begin(result.Top()), end(result.Top()));
    reverse(begin(result.Bottom()), end(result.Bottom()));
    for (auto& row : result.pixels) {
      reverse(begin(row), end(row));
    }
    return result;
  }

  uint64_t Id() const { return id; }

  const string& Top() const { return borders[static_cast<int>(Border::top)]; }
  string& Top() { return borders[static_cast<int>(Border::top)]; }
  const string& Right() const
  {
    return borders[static_cast<int>(Border::right)];
  }
  string& Right() { return borders[static_cast<int>(Border::right)]; }
  const string& Bottom() const
  {
    return borders[static_cast<int>(Border::bottom)];
  }
  string& Bottom() { return borders[static_cast<int>(Border::bottom)]; }
  const string& Left() const { return borders[static_cast<int>(Border::left)]; }
  string& Left() { return borders[static_cast<int>(Border::left)]; }

  const string& Row(int index) const { return pixels[index]; }

private:
  const int id;
  enum class Border
  {
    top = 0,
    right,
    bottom,
    left
  };
  array<string, 4> borders;
  vector<string> pixels;

  friend bool operator<(const Tile& lhs, const Tile& rhs);
};

bool
operator<(const Tile& lhs, const Tile& rhs)
{
  return tie(lhs.id, lhs.borders) < tie(rhs.id, rhs.borders);
}

int
main()
{
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  set<Tile> tiles;

  int total = 0;

  string line;
  while (getline(cin, line)) {
    if (line.rfind("Tile ", 0) == 0) {
      auto tile_id = x::to<int>(string_view(line).substr(5, 4));
      vector<string> pixels;
      while (getline(cin, line)) {
        if (line.empty())
          break;
        pixels.push_back(line);
      }
      Tile tile(tile_id, move(pixels));
      for (int i = 0; i < 4; ++i) {
        tiles.insert(tile);
        tiles.insert(tile.FlipV());
        tiles.insert(tile.FlipH());
        tile.Rotate();
      }
      ++total;
    }
  }

  unordered_map<string, vector<const Tile*>> lefts;
  unordered_map<string, vector<const Tile*>> tops;
  map<pair<string, string>, vector<const Tile*>> lefts_tops;
  for (const auto& tile : tiles) {
    lefts[tile.Left()].push_back(&tile);
    tops[tile.Top()].push_back(&tile);
    lefts_tops[{ tile.Left(), tile.Top() }].push_back(&tile);
  }

  const int side = sqrt(total);

  set<uint64_t> used_tiles;
  vector<vector<const Tile*>> square(side, vector<const Tile*>(side));

  auto Find = [&]() {
    auto _Find = [&](auto _Find, int n) {
      if (n >= total) {
        uint64_t result =
          square.front().front()->Id() * square.front().back()->Id() *
          square.back().front()->Id() * square.back().back()->Id();
        cout << result << endl;
        return true;
      }
      int row = n / side;
      int col = n % side;
      if (row == 0) {
        for (const auto tile : lefts[square[0][col - 1]->Right()]) {
          if (used_tiles.find(tile->Id()) == end(used_tiles)) {
            square[row][col] = tile;
            used_tiles.insert(tile->Id());
            if (_Find(_Find, n + 1))
              return true;
            used_tiles.erase(tile->Id());
          }
        }
      } else if (col == 0) {
        for (const auto tile : tops[square[row - 1][0]->Bottom()]) {
          if (used_tiles.find(tile->Id()) == end(used_tiles)) {
            square[row][col] = tile;
            used_tiles.insert(tile->Id());
            if (_Find(_Find, n + 1))
              return true;
            used_tiles.erase(tile->Id());
          }
        }
      } else {
        for (const auto tile : lefts_tops[{ square[row][col - 1]->Right(),
                                            square[row - 1][col]->Bottom() }]) {
          if (used_tiles.find(tile->Id()) == end(used_tiles)) {
            square[row][col] = tile;
            used_tiles.insert(tile->Id());
            if (_Find(_Find, n + 1))
              return true;
            used_tiles.erase(tile->Id());
          }
        }
      }
      return false;
    };
    for (const auto& tile : tiles) {
      square[0][0] = &tile;
      used_tiles.insert(tile.Id());
      if (_Find(_Find, 1))
        return true;
      used_tiles.erase(tile.Id());
    }
    return false;
  };

  Find();

  vector<string> image;

  for (const auto& row : square) {
    for (int i = 1; i < 10 - 1; ++i) {
      string line;
      for (const auto tile : row) {
        auto& tile_row = tile->Row(i);
        line += tile_row.substr(1, tile_row.size() - 2);
      }
      image.push_back(move(line));
    }
  }

  vector<string> monster = {
    "                  # ",
    "#    ##    ##    ###",
    " #  #  #  #  #  #   ",
  };
  const int monster_count = [&]() {
    int result = 0;
    for (const auto& row : monster) {
      result += count(begin(row), end(row), '#');
    }
    return result;
  }();

  auto Found = [&]() {
    int result = 0;
    for (size_t i = 0; i + monster.size() <= image.size(); ++i) {
      for (size_t j = 0; j + monster[0].size() <= image[i].size(); ++j) {
        bool found = true;
        for (size_t mi = 0; found && mi < monster.size(); ++mi) {
          for (size_t mj = 0; found && mj < monster[mi].size(); ++mj) {
            if (monster[mi][mj] == '#' && image[i + mi][j + mj] != '#') {
              found = false;
            }
          }
        }
        if (found)
          ++result;
      }
    }
    if (result > 0) {
      int total_count = 0;
      for (const auto& row : image) {
        total_count += count(begin(row), end(row), '#');
      }
      cout << total_count - monster_count * result << endl;
    }
  };

  for (int k = 0; k < 4; ++k) {
    Found();
    reverse(begin(image), end(image));
    Found();
    reverse(begin(image), end(image));
    for (auto& row : image) {
      reverse(begin(row), end(row));
    }
    Found();
    for (auto& row : image) {
      reverse(begin(row), end(row));
    }
    vector<string> rotated = image;
    for (size_t i = 0; i < image.size(); ++i) {
      for (size_t j = 0; j < image.size(); ++j) {
        rotated[i][j] = image[j][image[j].size() - i - 1];
      }
    }
    image = move(rotated);
  }
}
