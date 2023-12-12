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
import java.util.Map;
import java.util.StringJoiner;
import java.util.regex.Pattern;

import static java.lang.Integer.parseInt;

public class a12 {

  private final BufferedReader input;
  private final Pattern DELIMITERS = Pattern.compile(",");

  void run() throws IOException {
    long sum1 = 0, sum2 = 0;
    String line;
    while ((line = input.readLine()) != null) {
      var split = line.split(" ");
      var records = split[0];
      var groups = DELIMITERS.splitAsStream(split[1]).mapToInt(Integer::parseInt).toArray();

      sum1 += calculate(records, 0, groups, 0, 0, new HashMap<>());

      {
        var unfoldedRecords = new StringJoiner("?");
        var unfoldedGroups = new int[groups.length * 5];
        for (int i = 0; i < 5; i++) {
          unfoldedRecords.add(records);
          System.arraycopy(groups, 0, unfoldedGroups, i * groups.length, groups.length);
        }
        records = unfoldedRecords.toString();
        groups = unfoldedGroups;
      }

      sum2 += calculate(records, 0, groups, 0, 0, new HashMap<>());
    }
    System.out.println(sum1);
    System.out.println(sum2);
  }

  record State(int i, int j, int k) { }

  private long cached(String records, int i, int[] groups, int j, int k, Map<State, Long> cache) {
    var s = new State(i, j, k);
    Long r;
    if ((r = cache.get(s)) == null) cache.put(s, r = calculate(records, i, groups, j, k, cache));
    return r;
  }

  private long calculate(String records, int i, int[] groups, int j, int k, Map<State, Long> cache) {
    if (j < groups.length && groups[j] < k) return 0;
    if (i == records.length()) {
      if (j == groups.length - 1 && groups[j] == k || j == groups.length && k == 0) return 1;
      return 0;
    }
    return switch (records.charAt(i)) {
      case '#' -> cached(records, i + 1, groups, j, k + 1, cache);
      case '.' -> {
        if (k == 0) yield cached(records, i + 1, groups, j, 0, cache);
        if (j >= groups.length) yield 0;
        if (groups[j] != k) yield 0;
        yield cached(records, i + 1, groups, j + 1, 0, cache);
      }
      case '?' -> {
        long result = 0;
        if (k == 0) result += cached(records, i + 1, groups, j, k, cache);
        if (j >= groups.length) yield result;
        if (k < groups[j]) result += cached(records, i + 1, groups, j, k + 1, cache);
        else result += cached(records, i + 1, groups, j + 1, 0, cache);
        yield result;
      }
      default ->
          throw new IllegalStateException(STR."Unexpected value in \"\{records}\" at \{i}: \{records.charAt(i)}");
    };
  }

  public static void main(String[] args) throws IOException, InterruptedException {
    new a12().run();
  }

  //<editor-fold desc="Initialization">
  public a12() throws IOException, InterruptedException {
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