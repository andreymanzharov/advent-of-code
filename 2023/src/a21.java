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
import java.util.function.LongUnaryOperator;

import static java.lang.Integer.parseInt;

public class a21 {

  private final BufferedReader input;

  record P(int row, int col) { }

  void run() throws IOException {
    var map = input.lines().map(String::toCharArray).toArray(char[][]::new);
    var n = map.length;
    assert map[n / 2][n / 2] == 'S';
    map[n / 2][n / 2] = '.';

    System.out.println(count(map, 64));

    //var results = IntStream.range(0, 3).map(i -> count(map, n / 2 + i * n)).toArray();
    LongUnaryOperator count = x -> 15094 * x * x + 15196 * x + 3835;
    //assert IntStream.range(0, 3).allMatch(i -> results[i] == count.applyAsLong(i));
    System.out.println(count.applyAsLong((26501365 - n / 2) / n));
  }

  private static final P[] D = new P[] {new P(-1, 0), new P(0, -1), new P(1, 0), new P(0, 1)};

  private static int count(char[][] m, int k) {
    var n = m.length;
    var set = new HashSet<P>();
    set.add(new P(n / 2, n / 2));
    for (int i = 0; i < k; i++) {
      var next = new HashSet<P>();
      for (P a : set) {
        for (P d : D) {
          var np = new P(a.row + d.row, a.col + d.col);
          if (m[(np.row + n * 100) % n][(np.col + n * 100) % n] == '.')
            next.add(np);
        }
      }
      set = next;
    }
    return set.size();
  }

  public static void main(String[] args) throws IOException, InterruptedException {
    new a21().run();
  }

  //<editor-fold desc="Initialization">
  public a21() throws IOException, InterruptedException {
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