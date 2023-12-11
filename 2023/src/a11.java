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

import static java.lang.Integer.parseInt;
import static java.lang.Math.abs;

public class a11 {

  private final BufferedReader input;

  record Location(int row, int col) { }

  void run() throws IOException {
    var image = input.lines().map(String::toCharArray).toArray(char[][]::new);

    boolean[] rowsContainsGalaxies = new boolean[image.length];
    boolean[] colsContainsGalaxies = new boolean[image[0].length];

    var galaxies = new ArrayList<Location>();
    for (int i = 0; i < image.length; i++) {
      for (int j = 0; j < image[i].length; j++) {
        if (image[i][j] == '#') {
          galaxies.add(new Location(i, j));
          rowsContainsGalaxies[i] = colsContainsGalaxies[j] = true;
        }
      }
    }

    var numbersOfExpandedRows = getNumbersOfExpanded(rowsContainsGalaxies);
    var numbersOfExpandedColumns = getNumbersOfExpanded(colsContainsGalaxies);

    var sum1 = 0L;
    var sum2 = 0L;
    for (int i = 0; i < galaxies.size(); i++) {
      var x = galaxies.get(i);
      for (int j = i + 1; j < galaxies.size(); j++) {
        var y = galaxies.get(j);
        sum1 += pathLength(x, y, 2, numbersOfExpandedRows, numbersOfExpandedColumns);
        sum2 += pathLength(x, y, 1_000_000, numbersOfExpandedRows, numbersOfExpandedColumns);
      }
    }

    System.out.println(sum1);
    System.out.println(sum2);
  }

  private static int[] getNumbersOfExpanded(boolean[] hasGalaxies) {
    int[] result = new int[hasGalaxies.length];
    result[0] = hasGalaxies[0] ? 0 : 1;
    for (int i = 1; i < result.length; i++)
      result[i] = result[i - 1] + (hasGalaxies[i] ? 0 : 1);
    return result;
  }

  private static int pathLength(
      Location x,
      Location y,
      int replaceWith,
      int[] numbersOfExpandedRows,
      int[] numbersOfExpandedColumns
  ) {
    return abs(x.row() - y.row()) + abs(x.col() - y.col())
        + (replaceWith - 1) * abs(numbersOfExpandedRows[x.row()] - numbersOfExpandedRows[y.row()])
        + (replaceWith - 1) * abs(numbersOfExpandedColumns[x.col()] - numbersOfExpandedColumns[y.col()]);
  }

  public static void main(String[] args) throws IOException, InterruptedException {
    new a11().run();
  }

  //<editor-fold desc="Initialization">
  public a11() throws IOException, InterruptedException {
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