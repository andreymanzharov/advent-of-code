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
import java.util.regex.Pattern;

import static java.lang.Integer.parseInt;
import static java.util.stream.IntStream.range;

public class a24 {

  private final BufferedReader input;

  record V(double x, double y, double z) { }

  record Line(V p, V v) { }

  void run() throws IOException {
    var sep = Pattern.compile(" ?[,@] +");

    var lines = input.lines()
        .map(line -> {
          var ns = sep.splitAsStream(line).mapToLong(Long::parseLong).toArray();
          return new Line(new V(ns[0], ns[1], ns[2]), new V(ns[3], ns[4], ns[5]));
        })
        .toArray(Line[]::new);

    int count = 0;
    for (int i = 0; i < lines.length; i++) {
      for (int j = i + 1; j < lines.length; j++) {
        if (cross(lines[i], lines[j], 200000000000000d, 400000000000000d))
          count++;
      }
    }
    System.out.println(count);

    // (v1.x - v.x) * t1 + p1.x - p.x = 0
    // (v1.y - v.y) * t1 + p1.y - p.y = 0
    // (v1.z - v.z) * t1 + p1.z - p.z = 0

    // (v2.x - v.x) * t2 + p2.x - p.x = 0
    // (v2.y - v.y) * t2 + p2.y - p.y = 0
    // (v2.z - v.z) * t2 + p2.z - p.z = 0

    // (v3.x - v.x) * t3 + p3.x - p.x = 0
    // (v3.y - v.y) * t3 + p3.y - p.y = 0
    // (v3.z - v.z) * t3 + p3.z - p.z = 0

    "xyz".chars().forEach(c -> System.out.printf("(declare-const p%c Int)\n", c));
    "xyz".chars().forEach(c -> System.out.printf("(declare-const v%c Int)\n", c));
    range(0, 3).forEach(t -> System.out.printf("(declare-const t%d Int)\n", t + 1));
    for (int i = 0; i < 3; i++) {
      System.out.printf("(assert (= 0 (+ (* t%d (- %d vx)) %d (* -1 px))))\n", i + 1, (long) lines[i].v.x,
          (long) lines[i].p.x);
      System.out.printf("(assert (= 0 (+ (* t%d (- %d vy)) %d (* -1 py))))\n", i + 1, (long) lines[i].v.y,
          (long) lines[i].p.y);
      System.out.printf("(assert (= 0 (+ (* t%d (- %d vz)) %d (* -1 pz))))\n", i + 1, (long) lines[i].v.z,
          (long) lines[i].p.z);
    }
    System.out.println("(check-sat)");
    System.out.println("(get-model)");
  }

  private static boolean cross(Line line1, Line line2, double atLeast, double atMost) {
    var p1 = line1.p;
    var v1 = line1.v;
    var p2 = line2.p;
    var v2 = line2.v;

    var t1 = ((p1.x - p2.x) / v2.x - (p1.y - p2.y) / v2.y) / (v1.y / v2.y - v1.x / v2.x);
    var t2 = (p1.x - p2.x + t1 * v1.x) / v2.x;
    if (t1 <= 0 || t2 <= 0) return false;
    var x = p1.x + t1 * v1.x;
    var y = p1.y + t1 * v1.y;
    return atLeast <= x && x <= atMost && atLeast <= y && y <= atMost;
  }

  public static void main(String[] args) throws IOException, InterruptedException {
    new a24().run();
  }

  //<editor-fold desc="Initialization">
  public a24() throws IOException, InterruptedException {
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