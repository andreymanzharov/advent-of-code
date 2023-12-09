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
import java.util.regex.Pattern;

import static java.lang.Integer.parseInt;

public class a09 {

  private final BufferedReader input;

  private final Pattern WHITESPACE = Pattern.compile(" +");

  void run() throws IOException {
    int sum1 = 0, sum2 = 0;

    for (String line; (line = input.readLine()) != null; ) {
      var values = WHITESPACE.splitAsStream(line).mapToInt(Integer::parseInt).toArray();
      var firsts = new int[values.length];

      int step = values.length;
      while (!Arrays.stream(values).limit(step).allMatch(x -> x == values[0])) {
        firsts[values.length - step] = values[0];
        for (int i = 1; i < step; i++)
          values[i - 1] = values[i] - values[i - 1];
        step--;
      }

      int next = values[step - 1];
      for (int i = step; i < values.length; i++)
        next += values[i];
      sum1 += next;

      int prev = values[0];
      for (int i = values.length - step; i > 0; i--)
        prev = firsts[i - 1] - prev;
      sum2 += prev;
    }

    System.out.println(sum1);
    System.out.println(sum2);
  }

  public static void main(String[] args) throws IOException, InterruptedException {
    new a09().run();
  }

  //<editor-fold desc="Initialization">
  public a09() throws IOException, InterruptedException {
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