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
import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.regex.Pattern;
import java.util.stream.IntStream;

import static java.lang.Integer.parseInt;
import static java.lang.Math.max;
import static java.util.Comparator.comparingInt;

public class a22 {

  private final BufferedReader input;
  private final Pattern SEP = Pattern.compile("[,~]");

  void run() throws IOException {
    String line;

    record Brick(int x1, int y1, int z1, int x2, int y2, int z2) {
      Brick {
        assert x1 <= x2 && y1 <= y2 && z1 <= z2;
      }
    }

    var bricks = new ArrayList<Brick>();
    while ((line = input.readLine()) != null) {
      var coordinates = SEP.splitAsStream(line).mapToInt(Integer::parseInt).toArray();
      bricks.add(
          new Brick(coordinates[0], coordinates[1], coordinates[2], coordinates[3], coordinates[4], coordinates[5])
      );
    }

    int[][] g = new int[10][10];
    int[][] ids = new int[10][10];
    bricks.sort(comparingInt(Brick::z1));

    var stayOnSet = IntStream.range(0, bricks.size() + 1).mapToObj(HashSet<Integer>::new).toList();
    var supportSet = IntStream.range(0, bricks.size() + 1).mapToObj(HashSet<Integer>::new).toList();

    var i = 1;
    for (Brick brick : bricks) {
      int z = 0;
      for (int x = brick.x1; x <= brick.x2; x++) {
        for (int y = brick.y1; y <= brick.y2; y++) {
          z = max(z, g[x][y]);
        }
      }
      for (int x = brick.x1; x <= brick.x2; x++) {
        for (int y = brick.y1; y <= brick.y2; y++) {
          if (g[x][y] == z) {
            stayOnSet.get(i).add(ids[x][y]);
            supportSet.get(ids[x][y]).add(i);
          }
          g[x][y] = z + brick.z2 - brick.z1 + 1;
          ids[x][y] = i;
        }
      }
      i++;
    }

    var stayOn = stayOnSet.stream().map(set -> set.stream().mapToInt(x -> x).toArray()).toArray(int[][]::new);
    var support = supportSet.stream().map(set -> set.stream().mapToInt(x -> x).toArray()).toArray(int[][]::new);

    int count = 0;
    for (int j = 1; j <= bricks.size(); j++) {
      if (IntStream.of(support[j]).allMatch(k -> stayOn[k].length > 1))
        count++;
    }
    System.out.println(count);

    var sum = 0;
    for (int j = 1; j <= bricks.size(); j++) {
      sum += disintegrate(j, support, stayOn);
    }
    System.out.println(sum);
  }

  private int disintegrate(int start, int[][] support, int[][] stayOn) {
    var queue = new ArrayDeque<Integer>();
    queue.addLast(start);

    var fall = new boolean[support.length];
    fall[start] = true;

    var count = 0;
    Integer v;
    while ((v = queue.pollLast()) != null) {
      for (int u : support[v]) {
        if (!fall[u] && IntStream.of(stayOn[u]).allMatch(x -> fall[x])) {
          queue.addLast(u);
          fall[u] = true;
          count++;
        }
      }
    }
    return count;
  }

  public static void main(String[] args) throws IOException, InterruptedException {
    new a22().run();
  }

  //<editor-fold desc="Initialization">
  public a22() throws IOException, InterruptedException {
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