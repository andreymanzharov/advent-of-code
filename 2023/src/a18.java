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
import java.util.List;

import static java.lang.Integer.parseInt;
import static java.lang.Math.abs;

public class a18 {

  private final BufferedReader input;

  record Point(long x, long y) { }

  void run() throws IOException {
    long x1 = 0, y1 = 0, x2 = 0, y2 = 0;
    long b1 = 0, b2 = 0;

    var ps1 = new ArrayList<Point>();
    ps1.add(new Point(x1, y1));
    var ps2 = new ArrayList<Point>();
    ps2.add(new Point(x1, y1));
    for (String line; (line = input.readLine()) != null; ) {
      var split = line.split(" ");
      var n = Integer.parseInt(split[1]);
      b1 += n;
      switch (split[0]) {
        case "U" -> y1 += n;
        case "D" -> y1 -= n;
        case "L" -> x1 -= n;
        case "R" -> x1 += n;
      }
      ps1.add(new Point(x1, y1));
      n = Integer.parseInt(split[2].substring(2, 7), 16);
      b2 += n;
      switch (split[2].charAt(split[2].length() - 2)) {
        case '0' -> x2 += n;
        case '1' -> y2 -= n;
        case '2' -> x2 -= n;
        case '3' -> y2 += n;
      }
      ps2.add(new Point(x2, y2));
    }

    System.out.println(square(ps1, b1));
    System.out.println(square(ps2, b2));
  }

  private static long square(List<Point> ps, long b) {
    long s = 0;
    for (int i = 1; i < ps.size(); i++) {
      var p = ps.get(i - 1);
      var q = ps.get(i);
      s += p.x * q.y - p.y * q.x;
    }

    return abs(s / 2) - b / 2 + 1 + b;
  }

  public static void main(String[] args) throws IOException, InterruptedException {
    new a18().run();
  }

  //<editor-fold desc="Initialization">
  public a18() throws IOException, InterruptedException {
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