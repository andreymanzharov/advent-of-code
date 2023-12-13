import java.io.BufferedReader;
import java.io.IOException;
import java.net.CookieManager;
import java.net.HttpCookie;
import java.net.ProxySelector;
import java.net.URI;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;
import java.nio.file.Files;
import java.nio.file.NoSuchFileException;
import java.nio.file.Paths;
import java.util.Arrays;

import static java.lang.Integer.parseInt;

public class a13 {

  private final BufferedReader input;

  record Reflection(int row, int col) {
    Reflection {
      assert row != col && (row == 0 || col == 0);
    }
  }

  static class Summary {
    int number = 0;

    void add(Reflection reflection) {
      number += reflection.row() * 100 + reflection.col();
    }

    int get() {
      return number;
    }
  }

  void run() throws IOException {
    var lines = input.lines().toList();
    var summary1 = new Summary();
    var summary2 = new Summary();

    int from = 0, to = 0;
    while (from < lines.size()) {
      while (to < lines.size() && !lines.get(to).isBlank()) {
        to++;
      }
      var grid = lines.subList(from, to).stream().map(String::toCharArray).toArray(char[][]::new);
      var reflection = findReflection(grid, null);
      summary1.add(reflection);

      smudges:
      for (char[] row : grid) {
        for (int j = 0; j < row.length; j++) {
          invert(row, j);
          var otherReflection = findReflection(grid, reflection);
          if (otherReflection != null) {
            summary2.add(otherReflection);
            break smudges;
          }
          invert(row, j);
        }
      }

      from = ++to;
    }

    System.out.println(summary1.get());
    System.out.println(summary2.get());
  }

  private static void invert(char[] row, int index) {
    row[index] = switch (row[index]) {
      case '.' -> '#';
      case '#' -> '.';
      default -> throw new IllegalStateException(STR."Unexpected value: \{row[index]}");
    };
  }

  Reflection findReflection(char[][] grid, Reflection other) {
    for (int row = 1; row < grid.length; row++) {
      if (other != null && other.row() == row) continue;
      if (Arrays.equals(grid[row], grid[row - 1])) {
        boolean mirror = true;
        for (int i = row - 2, j = row + 1; mirror && i >= 0 && j < grid.length; i--, j++)
          mirror = Arrays.equals(grid[i], grid[j]);
        if (mirror) return new Reflection(row, 0);
      }
    }
    for (int col = 1; col < grid[0].length; col++) {
      if (other != null && other.col() == col) continue;
      if (grid[0][col - 1] == grid[0][col]) {
        boolean mirror = true;
        for (int i = col - 1, j = col; mirror && i >= 0 && j < grid[0].length; i--, j++)
          for (int r = 0; mirror && r < grid.length; r++) {
            mirror = grid[r][i] == grid[r][j];
          }
        if (mirror) return new Reflection(0, col);
      }
    }

    return null;
  }

  public static void main(String[] args) throws IOException, InterruptedException {
    new a13().run();
  }

  //<editor-fold desc="Initialization">
  public a13() throws IOException, InterruptedException {
    input = getInput(getClass().getSimpleName());
  }

  private static BufferedReader getInput(String name) throws IOException, InterruptedException {
    var day = parseInt(name.substring(1));
    var inputPath = Paths.get(STR."\{name}.in");
    try {
      return Files.newBufferedReader(inputPath);
    } catch (NoSuchFileException e) {
      var session = System.getenv("AOC_SESSION");
      if (session == null) throw e;

      var cookieManager = new CookieManager();
      var sessionCookie = new HttpCookie("session", session);
      sessionCookie.setPath("/");
      sessionCookie.setVersion(0);
      cookieManager.getCookieStore()
          .add(URI.create("https://adventofcode.com/"), sessionCookie);
      var builder = HttpClient.newBuilder()
          .proxy(ProxySelector.getDefault())
          .cookieHandler(cookieManager);

      try (var client = builder.build()) {
        var request = HttpRequest.newBuilder()
            .uri(URI.create(STR."https://adventofcode.com/2023/day/\{day}/input"))
            .build();
        var response = client.send(request, HttpResponse.BodyHandlers.ofFile(inputPath));
        if (response.statusCode() == 200) return Files.newBufferedReader(response.body());
        var message = Files.readString(response.body()).strip();
        Files.deleteIfExists(response.body());
        throw new RuntimeException(message);
      } catch (Exception ex) {
        ex.addSuppressed(e);
        throw ex;
      }
    }
  }
  //</editor-fold>
}