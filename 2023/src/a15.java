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
import java.util.LinkedHashMap;

import static java.lang.Integer.parseInt;
import static java.util.Arrays.stream;
import static java.util.stream.IntStream.range;

public class a15 {

  private final BufferedReader input;

  void run() throws IOException {
    var steps = input.readLine().split(",");

    System.out.println(stream(steps)
        .mapToInt(step -> step.chars().reduce(0, a15::hash))
        .sum()
    );

    var boxes = range(0, 256).mapToObj(LinkedHashMap<String, Integer>::new).toList();

    for (String step : steps) {
      var index = 0;
      var i = 0;
      while (step.charAt(i) != '=' && step.charAt(i) != '-') {
        index = hash(index, step.charAt(i));
        i++;
      }
      var label = step.substring(0, i);
      switch (step.charAt(i)) {
        case '-' -> boxes.get(index).remove(label);
        case '=' -> {
          var focalLength = step.charAt(i + 1) - '0';
          if (boxes.get(index).computeIfPresent(label, (_, _) -> focalLength) == null)
            boxes.get(index).put(label, focalLength);
        }
      }
    }
    var focusingPower = 0;
    for (int boxIndex = 1; boxIndex <= boxes.size(); boxIndex++) {
      var box = boxes.get(boxIndex - 1);
      var lensSlot = 1;
      for (var lens : box.entrySet()) {
        focusingPower += boxIndex * lensSlot * lens.getValue();
        lensSlot++;
      }
    }
    System.out.println(focusingPower);
  }

  private static int hash(int hash, int c) {
    return (hash + c) * 17 & 0xff;
  }

  public static void main(String[] args) throws IOException, InterruptedException {
    new a15().run();
  }

  //<editor-fold desc="Initialization">
  public a15() throws IOException, InterruptedException {
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