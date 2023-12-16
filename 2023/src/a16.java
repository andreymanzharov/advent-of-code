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
import java.util.ArrayList;
import java.util.HashSet;

import static java.lang.Integer.parseInt;
import static java.lang.Math.max;

public class a16 {

  private final BufferedReader input;

  record Position(int r, int c, int dr, int dc) {
    Position next() { return new Position(r + dr, c + dc, dr, dc); }
  }

  void run() throws IOException {
    var grid = input.lines().map(String::toCharArray).toArray(char[][]::new);

    System.out.println(numberOfEnergized(grid, new Position(0, 0, 0, 1)));

    int largest = 0;
    for (int i = 0; i < grid.length; i++) {
      largest = max(largest, numberOfEnergized(grid, new Position(i, 0, 0, 1)));
      largest = max(largest, numberOfEnergized(grid, new Position(i, grid[i].length - 1, 0, -1)));
    }
    for (int j = 0; j < grid[0].length; j++) {
      largest = max(largest, numberOfEnergized(grid, new Position(0, j, 1, 0)));
      largest = max(largest, numberOfEnergized(grid, new Position(grid.length - 1, j, -1, 0)));
    }

    System.out.println(largest);
  }

  private static int numberOfEnergized(char[][] grid, Position start) {
    var stack = new ArrayList<Position>();
    stack.add(start);

    var visited = new HashSet<Position>();

    record Tile(int r, int c) {
      Tile(Position p) {
        this(p.r(), p.c());
      }
    }
    var energized = new HashSet<Tile>();

    while (!stack.isEmpty()) {
      var p = stack.removeLast();
      if (p.r() < 0 || p.r() >= grid.length || p.c() < 0 || p.c() >= grid[p.r()].length) continue;
      if (!visited.add(p)) continue;
      energized.add(new Tile(p));
      switch (grid[p.r()][p.c()]) {
        case '.' -> stack.add(p.next());
        case '-' -> {
          if (p.dr() == 0)
            stack.add(p.next());
          else {
            stack.add(new Position(p.r(), p.c() - 1, 0, -1));
            stack.add(new Position(p.r(), p.c() + 1, 0, 1));
          }
        }
        case '|' -> {
          if (p.dc() == 0)
            stack.add(p.next());
          else {
            stack.add(new Position(p.r() - 1, p.c(), -1, 0));
            stack.add(new Position(p.r() + 1, p.c(), 1, 0));
          }
        }
        case '/' -> {
          if (p.dr > 0) stack.add(new Position(p.r(), p.c() - 1, 0, -1));
          if (p.dr < 0) stack.add(new Position(p.r(), p.c() + 1, 0, 1));
          if (p.dc > 0) stack.add(new Position(p.r() - 1, p.c(), -1, 0));
          if (p.dc < 0) stack.add(new Position(p.r() + 1, p.c(), 1, 0));
        }
        case '\\' -> {
          if (p.dr < 0) stack.add(new Position(p.r(), p.c() - 1, 0, -1));
          if (p.dr > 0) stack.add(new Position(p.r(), p.c() + 1, 0, 1));
          if (p.dc < 0) stack.add(new Position(p.r() - 1, p.c(), -1, 0));
          if (p.dc > 0) stack.add(new Position(p.r() + 1, p.c(), 1, 0));
        }
      }
    }
    return energized.size();
  }

  public static void main(String[] args) throws IOException, InterruptedException {
    new a16().run();
  }

  //<editor-fold desc="Initialization">
  public a16() throws IOException, InterruptedException {
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