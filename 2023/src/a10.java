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
import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Set;

import static java.lang.Integer.parseInt;

public class a10 {

  private final BufferedReader input;

  record Cell(int row, int col) {
    char get(char[][] grid) { return grid[row][col]; }

    Cell up() { return new Cell(row - 1, col); }

    Cell down() { return new Cell(row + 1, col); }

    Cell left() { return new Cell(row, col - 1); }

    Cell right() { return new Cell(row, col + 1); }
  }

  void run() throws IOException {
    var grid = input.lines().map(String::toCharArray).toArray(char[][]::new);

    var start = findStartCell(grid);
    replaceStartCell(start, grid);
    var loop = findLoop(start, grid);

    System.out.println(loop.values().stream().mapToInt(x -> x).max().orElseThrow());
    System.out.println(getEnclosedCount(grid, loop.keySet()));
  }

  private static boolean leftOut(char tile) {
    return switch (tile) {
      case '-', 'J', '7' -> true;
      default -> false;
    };
  }

  private static boolean rightOut(char tile) {
    return switch (tile) {
      case '-', 'L', 'F' -> true;
      default -> false;
    };
  }

  private static boolean upOut(char tile) {
    return switch (tile) {
      case '|', 'J', 'L' -> true;
      default -> false;
    };
  }

  private static boolean downOut(char tile) {
    return switch (tile) {
      case '|', '7', 'F' -> true;
      default -> false;
    };
  }

  private static void replaceStartCell(Cell start, char[][] grid) {
    char left = start.col() > 0 ? grid[start.row()][start.col() - 1] : 0,
        right = start.col() < grid[start.row()].length - 1 ? grid[start.row()][start.col() + 1] : 0,
        up = start.row() > 0 ? grid[start.row() - 1][start.col()] : 0,
        down = start.row() < grid.length - 1 ? grid[start.row() + 1][start.col()] : 0;
    if (downOut(up) && upOut(down))
      grid[start.row()][start.col()] = '|';
    if (downOut(up) && rightOut(left))
      grid[start.row()][start.col()] = 'J';
    if (downOut(up) && leftOut(right))
      grid[start.row()][start.col()] = 'L';
    if (rightOut(left) && leftOut(right))
      grid[start.row()][start.col()] = '-';
    if (rightOut(left) && upOut(down))
      grid[start.row()][start.col()] = '7';
    if (leftOut(right) && upOut(down))
      grid[start.row()][start.col()] = 'F';
  }

  private static Cell findStartCell(char[][] grid) {
    for (int i = 0; i < grid.length; i++)
      for (int j = 0; j < grid[i].length; j++)
        if (grid[i][j] == 'S') return new Cell(i, j);
    throw new IllegalArgumentException("not found");
  }

  private static HashMap<Cell, Integer> findLoop(Cell start, char[][] grid) {
    var loop = new HashMap<Cell, Integer>();
    loop.put(start, 0);

    var queue = new ArrayDeque<Cell>();
    queue.push(start);
    for (Cell cell; (cell = queue.pollFirst()) != null; ) {
      for (Cell next : nextCells(grid, cell, loop.keySet())) {
        queue.addLast(next);
        loop.put(next, loop.get(cell) + 1);
      }
    }
    return loop;
  }

  private static List<Cell> nextCells(char[][] grid, Cell cell, Set<Cell> loop) {
    char tile = grid[cell.row()][cell.col()];
    List<Cell> nextCells = new ArrayList<>(2);
    Cell next;
    if (cell.row() > 0 && !loop.contains(next = cell.up())
        && upOut(tile) && downOut(next.get(grid))) {
      nextCells.add(next);
    }
    if (cell.row() < grid.length - 1 && !loop.contains(next = cell.down())
        && downOut(tile) && upOut(next.get(grid))) {
      nextCells.add(next);
    }
    if (cell.col() > 0 && !loop.contains(next = cell.left())
        && leftOut(tile) && rightOut(next.get(grid))) {
      nextCells.add(next);
    }
    if (cell.col() < grid[cell.row()].length - 1 && !loop.contains(next = cell.right())
        && rightOut(tile) && leftOut(next.get(grid))) {
      nextCells.add(next);
    }
    return nextCells;
  }

  private static int getEnclosedCount(char[][] grid, Set<Cell> loop) {
    int result = 0;
    for (int i = 0; i < grid.length; i++) {
      int crossCount = 0;
      char prevTile = 0;
      for (int j = 0; j < grid[i].length; j++) {
        var tile = grid[i][j];
        var inLoop = loop.contains(new Cell(i, j));
        if (!inLoop && (crossCount & 1) == 1) result++;
        else if ((tile == 'F' || tile == 'L') && inLoop) prevTile = tile;
        else if (tile == 'J' && prevTile == 'F' && inLoop) crossCount++;
        else if (tile == '7' && prevTile == 'L' && inLoop) crossCount++;
        else if (tile == '|' && inLoop) crossCount++;
      }
    }
    return result;
  }

  public static void main(String[] args) throws IOException, InterruptedException {
    new a10().run();
  }

  //<editor-fold desc="Initialization">
  public a10() throws IOException, InterruptedException {
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