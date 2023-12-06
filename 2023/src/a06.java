import java.io.BufferedReader;
import java.io.IOException;
import java.io.StringReader;
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
import java.util.stream.IntStream;

import static java.lang.Integer.parseInt;

public class a06 {

  private final Pattern WHITESPACES = Pattern.compile(" +");
  private final BufferedReader input;

  void run() throws IOException {
    long[] times = WHITESPACES.splitAsStream(input.readLine()).skip(1).mapToLong(Long::parseLong).toArray();
    long[] distances = WHITESPACES.splitAsStream(input.readLine()).skip(1).mapToLong(Long::parseLong).toArray();

    long answer1 = IntStream.range(0, times.length)
        .mapToLong(i -> ways(times[i], distances[i]))
        .reduce(1, (a, b) -> a * b);
    System.out.println(answer1);

    long time = concat(times);
    long distance = concat(distances);

    long answer2 = ways(time, distance);
    System.out.println(answer2);
  }

  long ways(long time, long distance) {
    double d = Math.sqrt(time * time - 4 * distance);
    long x1 = (long) Math.floor((time - d) / 2) + 1;
    long x2 = (long) Math.ceil((time + d) / 2) - 1;
    return x2 - x1 + 1;
  }

  long concat(long[] numbers) {
    long result = 0;
    for (long number : numbers) {
      long pow = 1;
      while (pow < number)
        pow *= 10;
      result = result * pow + number;
    }
    return result;
  }

  public static void main(String[] args) throws IOException, InterruptedException {
    new a06().run();
  }

  //<editor-fold desc="Initialization">
  public a06() throws IOException, InterruptedException {
    input = getInput(getClass().getSimpleName());
  }

  public a06(String example) {
    input = new BufferedReader(new StringReader(example));
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