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
import java.util.HashMap;
import java.util.HashSet;

import static java.lang.Integer.parseInt;

public class a08 {

  private final BufferedReader input;

  record Node(String left, String right) { }

  void run() throws IOException {
    var instructions = input.readLine();
    input.readLine();

    var nodes = new HashMap<String, Node>();

    for (String line; (line = input.readLine()) != null; ) {
      nodes.put(line.substring(0, 3), new Node(line.substring(7, 10), line.substring(12, 15)));
    }

    {
      var steps = 0;
      var node = "AAA";
      for (int i = 0; !"ZZZ".equals(node); i = (i + 1) % instructions.length()) {
        if (instructions.charAt(i) == 'L')
          node = nodes.get(node).left();
        else
          node = nodes.get(node).right();
        steps++;
      }

      System.out.println(steps);
    }

    {
      var steps = nodes.keySet().stream().filter(s -> s.endsWith("A"))
          .mapToLong(current -> findCycle(current, instructions, nodes))
          .reduce(a08::lcm)
          .orElseThrow();

      System.out.println(steps);
    }
  }

  static int findCycle(String current, String instructions, HashMap<String, Node> nodes) {
    var visited = new HashSet<String>();
    int step = 0, pre = 0;
    for (int i = 0; true; i = (i + 1) % instructions.length()) {
      if (current.endsWith("Z")) {
        if (!visited.add(current)) {
          break;
        }
        pre = step;
      }
      if (instructions.charAt(i) == 'L')
        current = nodes.get(current).left();
      else
        current = nodes.get(current).right();
      step++;
    }
    return step - pre;
  }

  static long gcd(long a, long b) {
    while (b != 0) {
      long t = b;
      b = a % b;
      a = t;
    }
    return a;
  }

  static long lcm(long a, long b) {
    return a / gcd(a, b) * b;
  }

  public static void main(String[] args) throws IOException, InterruptedException {
    new a08().run();
  }

  //<editor-fold desc="Initialization">
  public a08() throws IOException, InterruptedException {
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
        if (response.statusCode() == 200) {
          return Files.newBufferedReader(response.body());
        }
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