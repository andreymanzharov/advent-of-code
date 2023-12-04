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
import java.util.regex.Pattern;

import static java.util.stream.Collectors.toSet;

public class a04 {

  private final Pattern WHITESPACES = Pattern.compile(" +");
  private final BufferedReader input;

  void run() throws IOException {
    int totalPoints = 0;

    var scratchcards = new ArrayList<Integer>();

    String line;
    for (int i = 0; (line = input.readLine()) != null; i++) {
      if (i == scratchcards.size())
        scratchcards.add(1);

      var split = line.split("[:|] +");

      var winningNumbers = WHITESPACES.splitAsStream(split[1])
          .map(Integer::parseInt)
          .collect(toSet());
      var count = WHITESPACES.splitAsStream(split[2])
          .map(Integer::parseInt)
          .filter(winningNumbers::contains)
          .count();

      if (count > 0) {
        totalPoints += 1 << count - 1;

        for (int j = 1; j <= count; j++) {
          if (i + j < scratchcards.size())
            scratchcards.set(i + j, scratchcards.get(i + j) + scratchcards.get(i));
          else
            scratchcards.add(1 + scratchcards.get(i));
        }
      }
    }

    var totalScratchcards = scratchcards.stream().mapToInt(i -> i).sum();

    System.out.println(totalPoints);
    System.out.println(totalScratchcards);
  }

  public static void main(String[] args) throws IOException, InterruptedException {
    new a04().run();
  }

  //<editor-fold desc="Initialization">
  public a04() throws IOException, InterruptedException {
    input = getInput(getClass().getSimpleName());
  }

  private static BufferedReader getInput(String name) throws IOException, InterruptedException {
    var day = Integer.parseInt(name.substring(1));
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