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
import java.util.ArrayList;
import java.util.TreeSet;
import java.util.regex.Pattern;

import static java.lang.Integer.parseInt;
import static java.lang.Long.parseLong;
import static java.lang.Math.min;
import static java.util.Comparator.comparingLong;

public class a05 {

  private final Pattern WHITESPACES = Pattern.compile(" +");
  private final BufferedReader input;

  record Range(long sourceStart, long length, long destinationStart) {

    long apply(long value) {
      return destinationStart + (value - sourceStart);
    }
  }

  void run() throws IOException {
    var seeds = WHITESPACES.splitAsStream(
        input.readLine().substring("seeds: ".length())
    ).mapToLong(Long::parseLong).toArray();

    var rangeComparator = comparingLong(Range::sourceStart)
        .thenComparingLong(Range::destinationStart);

    var maps = new ArrayList<TreeSet<Range>>();
    var current = new TreeSet<>(rangeComparator);

    String line;
    while ((line = input.readLine()) != null) {
      if (line.isEmpty()) {
        input.readLine();
        current = new TreeSet<>(rangeComparator);
        maps.add(current);
      } else {
        var split = line.split(" ");
        current.add(new Range(parseLong(split[1]), parseLong(split[2]), parseLong(split[0])));
      }
    }
    {
      long minLocation = Long.MAX_VALUE;

      for (long seed : seeds) {
        long value = seed;
        for (TreeSet<Range> map : maps) {
          var range = map.lower(new Range(value, 1, 0));
          if (range == null || range.sourceStart() + range.length() <= value) continue;
          value = range.apply(value);
        }
        minLocation = min(minLocation, value);
      }
      System.out.println(minLocation);
    }

    {
      long minLocation = Long.MAX_VALUE;
      for (int i = 0; i < seeds.length; i += 2) {
        for (long j = 0; j < seeds[i + 1]; ) {
          long value = seeds[i] + j;
          long next = Long.MAX_VALUE;
          for (TreeSet<Range> map : maps) {
            var range = map.lower(new Range(value, 1, Long.MAX_VALUE));
            if (range != null && range.sourceStart() + range.length() > value) {
              next = min(next, range.length() - (value - range.sourceStart()));
              value += range.destinationStart() - range.sourceStart();
            } else if (range == null) {
              next = min(next, map.getFirst().sourceStart() - value);
            } else {
              range = map.higher(range);
              next = min(next, range == null ? Long.MAX_VALUE : range.sourceStart() - value);
            }
          }

          minLocation = min(minLocation, value);
          j += next;
        }
      }
      System.out.println(minLocation);
    }

  }

  public static void main(String[] args) throws IOException, InterruptedException {
    new a05().run();
  }

  //<editor-fold desc="Initialization">
  public a05() throws IOException, InterruptedException {
    input = getInput(getClass().getSimpleName());
  }

  public a05(String example) {
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