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
import java.util.HashSet;
import java.util.PriorityQueue;

import static java.lang.Integer.parseInt;
import static java.util.Comparator.comparingInt;

public class a17 {

  private final BufferedReader input;

  record Position(int row, int col, int deltaRow, int deltaCol) { }

  record State(int heatLost, Position position) { }

  void run() throws IOException {
    var map = input.lines().map(line -> line.chars().map(c -> c - '0').toArray()).toArray(int[][]::new);
    System.out.println(leastHeatLost(map, 1, 3));
    System.out.println(leastHeatLost(map, 4, 10));
  }

  private static int leastHeatLost(int[][] map, int minSteps, int maxSteps) {
    var queue = new PriorityQueue<>(comparingInt(State::heatLost));
    queue.add(new State(0, new Position(0, 0, 0, 1)));
    queue.add(new State(0, new Position(0, 0, 1, 0)));

    var visited = new HashSet<Position>();

    for (State state; (state = queue.poll()) != null; ) {
      var pos = state.position;
      if (pos.row == map.length - 1 && pos.col == map[pos.row].length - 1) return state.heatLost;
      if (!visited.add(pos)) continue;
      int heatLost = state.heatLost();
      for (int step = 1; step <= maxSteps; step++) {
        int row = pos.row + step * pos.deltaRow;
        int col = pos.col + step * pos.deltaCol;
        if (row < 0 || row >= map.length || col < 0 || col >= map[row].length) continue;
        heatLost += map[row][col];
        if (step < minSteps) continue;
        queue.add(new State(heatLost, new Position(row, col, -pos.deltaCol, pos.deltaRow)));
        queue.add(new State(heatLost, new Position(row, col, pos.deltaCol, -pos.deltaRow)));
      }
    }
    throw new IllegalStateException();
  }

  public static void main(String[] args) throws IOException, InterruptedException {
    new a17().run();
  }

  //<editor-fold desc="Initialization">
  public a17() throws IOException, InterruptedException {
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