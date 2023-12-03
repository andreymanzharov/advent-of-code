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
import java.util.HashMap;
import java.util.List;
import java.util.Optional;

import static java.lang.Character.isDigit;
import static java.lang.Math.max;
import static java.lang.Math.min;

public class a03 {

  private final BufferedReader input;

  record Pos(int row, int col) { }

  void run() {
    var schematic = input.lines().toList();
    var partNumbers = new ArrayList<Integer>();
    var gears = new HashMap<Pos, List<Integer>>();
    for (int row = 0; row < schematic.size(); row++) {
      var line = schematic.get(row);
      int startCol = -1, number = 0;
      for (int col = 0; col < line.length(); col++) {
        var ch = line.charAt(col);
        if (isDigit(ch)) {
          if (startCol < 0) startCol = col;
          number = number * 10 + ch - '0';
        } else if (startCol >= 0) {
          var partNumber = number;
          findPart(schematic, row, startCol, col).ifPresent(pos -> {
            partNumbers.add(partNumber);
            if (schematic.get(pos.row()).charAt(pos.col()) == '*')
              gears.computeIfAbsent(pos, _ -> new ArrayList<>(2)).add(partNumber);
          });
          startCol = -1;
          number = 0;
        }
      }
      if (startCol >= 0) {
        var partNumber = number;
        findPart(schematic, row, startCol, line.length()).ifPresent(pos -> {
          partNumbers.add(partNumber);
          if (schematic.get(pos.row()).charAt(pos.col()) == '*')
            gears.computeIfAbsent(pos, _ -> new ArrayList<>()).add(partNumber);
        });
      }
    }
    System.out.println(partNumbers.stream().mapToInt(n -> n).sum());
    System.out.println(gears.values().stream().filter(numbers -> numbers.size() == 2)
        .mapToInt(numbers -> numbers.getFirst() * numbers.getLast()).sum());
  }

  Optional<Pos> findPart(List<String> schematic, int row, int from, int to) {
    if (row > 0) {
      var line = schematic.get(row - 1);
      for (int i = max(0, from - 1); i < min(to + 1, line.length()); i++)
        if (line.charAt(i) != '.')
          return Optional.of(new Pos(row - 1, i));
    }
    if (row < schematic.size() - 1) {
      var line = schematic.get(row + 1);
      for (int i = max(0, from - 1); i < min(to + 1, line.length()); i++)
        if (line.charAt(i) != '.')
          return Optional.of(new Pos(row + 1, i));
    }
    var line = schematic.get(row);
    if (from > 0 && line.charAt(from - 1) != '.')
      return Optional.of(new Pos(row, from - 1));
    if (to < line.length() && line.charAt(to) != '.')
      return Optional.of(new Pos(row, to));
    return Optional.empty();
  }

  public static void main(String[] args) throws IOException, InterruptedException {
    new a03().run();
  }

  //<editor-fold desc="Initialization">
  public a03() throws IOException, InterruptedException {
    input = getInput(getClass().getSimpleName());
  }

  public a03(String example) {
    input = new BufferedReader(new StringReader(example));
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