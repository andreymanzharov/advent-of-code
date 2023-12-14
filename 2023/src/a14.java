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
import java.util.HashMap;
import java.util.Iterator;
import java.util.stream.Collectors;

import static java.lang.Integer.parseInt;

public class a14 {

  private final BufferedReader input;

  void run() throws IOException {

    class Grid {
      final int n;
      final char[][] grid;
      final Iterator<Direction> directions;
      Direction direction;

      Grid(char[][] grid) {
        this.n = grid.length;
        this.grid = grid;
        this.directions = new Iterator<>() {

          final Direction[] values = Direction.values();
          int index = -1;

          @Override
          public boolean hasNext() {
            return true;
          }

          @Override
          public Direction next() {
            return values[index = (index + 1) & 3];
          }
        };
        this.direction = directions.next();
      }

      enum Direction {north, west, south, east}

      void rotate() { direction = directions.next(); }

      record Location(int row, int col) { }

      Location transform(int row, int col) {
        return switch (direction) {
          case north -> new Location(row, col);
          case west -> new Location(n - col - 1, row);
          case south -> new Location(n - row - 1, n - col - 1);
          case east -> new Location(col, n - row - 1);
        };
      }

      char get(int row, int col) {
        var l = transform(row, col);
        return grid[l.row()][l.col()];
      }

      void set(int row, int col, char c) {
        var l = transform(row, col);
        grid[l.row()][l.col()] = c;
      }

      int totalLoad() {
        int result = 0;
        for (int col = 0; col < n; col++) {
          for (int row = 0; row < n; row++) {
            if (grid[row][col] == 'O') {
              result += n - row;
            }
          }
        }
        return result;
      }

      @Override
      public String toString() {
        return Arrays.stream(grid).map(String::new).collect(Collectors.joining());
      }
    }

    var grid = new Grid(input.lines().map(String::toCharArray).toArray(char[][]::new));

    {
      int totalLoad = 0;
      for (int col = 0; col < grid.n; col++) {
        int rock = -1;
        for (int row = 0; row < grid.n; row++) {
          switch (grid.get(row, col)) {
            case '#' -> rock = row;
            case 'O' -> {
              rock++;
              totalLoad += grid.n - rock;
            }
          }
        }
      }
      System.out.println(totalLoad);
    }

    final int CYCLES = 1_000_000_000;

    var state = new HashMap<String, Integer>();
    boolean loopFound = false;
    for (int cycle = 0; cycle < CYCLES; cycle++) {
      for (int i = 0; i < 4; i++) {
        for (int col = 0; col < grid.n; col++) {
          int rock = -1;
          for (int row = 0; row < grid.n; row++) {
            switch (grid.get(row, col)) {
              case '#' -> rock = row;
              case 'O' -> {
                grid.set(row, col, '.');
                grid.set(++rock, col, 'O');
              }
            }
          }
        }
        grid.rotate();
      }
      if (!loopFound) {
        var stateKey = grid.toString();

        var previousCycle = state.get(stateKey);
        if (previousCycle == null) {
          state.put(stateKey, cycle);
        } else {
          loopFound = true;
          var loop = cycle - previousCycle;
          cycle += (CYCLES - cycle) / loop * loop;
        }
      }

    }

    System.out.println(grid.totalLoad());
  }

  public static void main(String[] args) throws IOException, InterruptedException {
    new a14().run();
  }

  //<editor-fold desc="Initialization">
  public a14() throws IOException, InterruptedException {
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