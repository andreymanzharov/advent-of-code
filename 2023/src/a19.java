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
import java.util.HashMap;
import java.util.List;
import java.util.function.ToIntFunction;

import static java.lang.Integer.parseInt;
import static java.lang.Math.max;
import static java.lang.Math.min;

public class a19 {

  private final BufferedReader input;

  record Part(int x, int m, int a, int s) { }

  enum Category {
    x(Part::x), m(Part::m), a(Part::a), s(Part::s);
    final ToIntFunction<Part> ex;

    Category(ToIntFunction<Part> ex) { this.ex = ex; }
  }

  enum Condition {less, greater}

  record Rule(Category cat, Condition cond, int value, String next) { }

  record Workflow(List<Rule> rules, String otherwise) {
    String apply(Part part) {
      for (Rule rule : rules) {
        var x = rule.cat.ex.applyAsInt(part);
        switch (rule.cond) {
          case less -> {
            if (x < rule.value) return rule.next;
          }
          case greater -> {
            if (x > rule.value) return rule.next;
          }
        }
      }
      return otherwise;
    }
  }

  void run() throws IOException {

    var workflows = new HashMap<String, Workflow>();

    String line;
    while (!(line = input.readLine()).isEmpty()) {
      var tokens = line.split("[{:,}]");
      var rules = new ArrayList<Rule>(tokens.length);
      for (int i = 1; i < tokens.length - 1; i += 2) {
        var cat = switch (tokens[i].charAt(0)) {
          case 'x' -> Category.x;
          case 'm' -> Category.m;
          case 'a' -> Category.a;
          case 's' -> Category.s;
          default -> throw new IllegalStateException(STR."Unexpected value: \{tokens[i].charAt(0)}");
        };
        var cond = switch (tokens[i].charAt(1)) {
          case '<' -> Condition.less;
          case '>' -> Condition.greater;
          default -> throw new IllegalStateException(STR."Unexpected value: \{tokens[i].charAt(1)}");
        };
        rules.add(new Rule(cat, cond, parseInt(tokens[i].substring(2)), tokens[i + 1]));
      }
      workflows.put(tokens[0], new Workflow(rules, tokens[tokens.length - 1]));
    }

    var sum = sumAcceptedRatings(workflows);
    System.out.println(sum);

    var ranges = new Range[Category.values().length];
    for (int i = 0; i < ranges.length; i++) {
      ranges[i] = new Range(1, 4000);
    }
    System.out.println(distinctCombinations("in", ranges, workflows));
  }

  private int sumAcceptedRatings(HashMap<String, Workflow> workflows) throws IOException {
    String line;
    var sum = 0;
    while ((line = input.readLine()) != null) {
      var tokens = line.split("[=,}]");
      var part = new Part(parseInt(tokens[1]), parseInt(tokens[3]), parseInt(tokens[5]), parseInt(tokens[7]));

      var next = "in";
      while (!"A".equals(next) && !"R".equals(next)) {
        next = workflows.get(next).apply(part);
      }
      if ("A".equals(next))
        sum += part.x + part.m + part.a + part.s;
    }
    return sum;
  }

  record Range(int left, int right) {
    boolean isValid() {
      return left <= right;
    }
  }

  private long distinctCombinations(String name, Range[] ranges, HashMap<String, Workflow> workflows) {
    if ("A".equals(name)) {
      var count = 1L;
      for (Range range : ranges) {
        count *= range.right - range.left + 1;
      }
      return count;
    }
    var workflow = workflows.get(name);
    if (workflow == null) return 0;
    var count = 0L;
    var nextRanges = ranges.clone();
    for (Rule rule : workflow.rules) {
      var index = rule.cat.ordinal();
      var trueRange = switch (rule.cond) {
        case less -> new Range(nextRanges[index].left, min(nextRanges[index].right, rule.value - 1));
        case greater -> new Range(max(nextRanges[index].left, rule.value + 1), nextRanges[index].right);
      };
      if (trueRange.isValid()) {
        var saved = nextRanges[index];
        nextRanges[index] = trueRange;
        count += distinctCombinations(rule.next, nextRanges, workflows);
        nextRanges[index] = saved;
      }
      var falseRange = switch (rule.cond) {
        case less -> new Range(max(nextRanges[index].left, rule.value), nextRanges[index].right);
        case greater -> new Range(nextRanges[index].left, min(nextRanges[index].right, rule.value));
      };
      if (!falseRange.isValid()) return count;
      nextRanges[index] = falseRange;
    }
    count += distinctCombinations(workflow.otherwise, nextRanges, workflows);
    return count;
  }

  public static void main(String[] args) throws IOException, InterruptedException {
    new a19().run();
  }

  //<editor-fold desc="Initialization">
  public a19() throws IOException, InterruptedException {
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