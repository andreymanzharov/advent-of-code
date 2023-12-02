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
import java.util.List;

import static java.lang.Integer.parseInt;
import static java.lang.Math.max;

public class a02 {

  private final BufferedReader input;

  void run() throws IOException {
    record Cubes(int total, String color) { }
    var bag = List.of(
        new Cubes(12, "red"),
        new Cubes(13, "green"),
        new Cubes(14, "blue")
    );

    int sum1 = 0, sum2 = 0;

    String line;
    while ((line = input.readLine()) != null) {
      var sets = line.split("[:;] ");
      var id = parseInt(sets[0].substring(5));
      boolean possible = true;

      int[] fewestNumbers = new int[bag.size()];

      for (var set : sets) {
        for (var token : set.split(", ")) {
          for (int i = 0; i < bag.size(); i++) {
            var cube = bag.get(i);
            if (token.endsWith(cube.color)) {
              var count = parseInt(token.substring(0, token.length() - cube.color().length() - 1));
              if (count > cube.total())
                possible = false;
              fewestNumbers[i] = max(fewestNumbers[i], count);
            }
          }
        }
      }
      if (possible)
        sum1 += id;

      int power = 1;
      for (var number : fewestNumbers)
        power *= number;
      sum2 += power;
    }

    System.out.println(sum1);
    System.out.println(sum2);
  }

  public static void main(String[] args) throws IOException, InterruptedException {
    new a02().run();
  }

  //<editor-fold desc="Initialization">
  public a02() throws IOException, InterruptedException {
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