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
import java.util.HashMap;
import java.util.concurrent.ThreadLocalRandom;
import java.util.function.ToIntFunction;
import java.util.regex.Pattern;

import static java.lang.Integer.parseInt;
import static java.util.Arrays.stream;
import static java.util.stream.IntStream.range;

public class a25 {

  private final BufferedReader input;

  void run() throws IOException {
    var ids = new HashMap<String, Integer>();
    ToIntFunction<String> toIndex = id -> ids.computeIfAbsent(id, _ -> ids.size());

    var SEP = Pattern.compile(":? ");

    record Edge(int v, int u) { }

    var edges = new ArrayList<Edge>();

    String line;
    while ((line = input.readLine()) != null) {
      var colon = line.indexOf(':');
      int v = toIndex.applyAsInt(line.substring(0, colon));
      SEP.splitAsStream(line).skip(1)
          .mapToInt(toIndex)
          .mapToObj(u -> new Edge(v, u))
          .forEach(edges::add);
    }

    while (true) {
      var random = ThreadLocalRandom.current();
      var parent = range(0, ids.size()).toArray();
      var vs = ids.size();
      while (vs > 2) {
        var e = edges.get(random.nextInt(edges.size()));

        int v = find(parent, e.v);
        int u = find(parent, e.u);

        if (v == u) continue;

        union(parent, v, u);
        vs--;
      }

      var cut = 0;
      for (Edge edge : edges) {
        if (find(parent, edge.v) != find(parent, edge.u)) {
          cut++;
        }
      }
      if (cut == 3) {
        var first = stream(parent).filter(v -> v == parent[0]).count();
        var second = parent.length - first;
        System.out.println(first * second);
        break;
      }
    }
  }

  private int find(int[] parent, int v) {
    if (parent[v] == v) return v;
    return parent[v] = find(parent, parent[v]);
  }

  private void union(int[] parent, int v, int u) {
    v = find(parent, v);
    u = find(parent, u);
    parent[v] = u;
  }

  public static void main(String[] args) throws IOException, InterruptedException {
    new a25().run();
  }

  //<editor-fold desc="Initialization">
  public a25() throws IOException, InterruptedException {
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