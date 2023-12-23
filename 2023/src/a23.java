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

import static java.lang.Integer.parseInt;
import static java.lang.Math.max;

public class a23 {

  private final BufferedReader input;

  void run() throws IOException {
    var map = input.lines().map(String::toCharArray).toArray(char[][]::new);
    var visited = new boolean[map.length][map[0].length];
    int longest1 = 0, longest2 = 0;
    for (int j = 0; j < map[0].length; j++) {
      if (map[0][j] == '.') {
        longest1 = max(longest1, dfs1(0, j, 0, map, visited));
        longest2 = max(longest2, dfs2(0, j, 0, map, visited));
      }
    }
    System.out.println(longest1);
    System.out.println(longest2);
  }

  static int[] dr = new int[] {-1, 0, 1, 0}, dc = new int[] {0, -1, 0, 1};
  static char[] slopes = new char[] {'^', '<', 'v', '>'};

  private static int dfs1(int row, int col, int steps, char[][] map, boolean[][] visited) {
    if (row == map.length - 1) return steps;
    visited[row][col] = true;
    var longest = 0;
    for (int i = 0; i < 4; i++) {
      int nr = row + dr[i], nc = col + dc[i];
      if (nr >= 0 && (map[nr][nc] == '.' || map[nr][nc] == slopes[i]) && !visited[nr][nc]) {
        longest = max(longest, dfs1(nr, nc, steps + 1, map, visited));
      }
    }
    visited[row][col] = false;
    return longest;
  }

  private static int dfs2(int r, int c, int k, char[][] g, boolean[][] v) {
    if (r == g.length - 1) return k;
    v[r][c] = true;
    var steps = 0;
    for (int i = 0; i < 4; i++) {
      int nr = r + dr[i], nc = c + dc[i];
      if (nr >= 0 && g[nr][nc] != '#' && !v[nr][nc]) {
        steps = max(steps, dfs2(nr, nc, k + 1, g, v));
      }
    }
    v[r][c] = false;
    return steps;
  }

  public static void main(String[] args) throws IOException, InterruptedException {
    new a23().run();
  }

  //<editor-fold desc="Initialization">
  public a23() throws IOException, InterruptedException {
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