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
import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;
import java.util.Optional;

import static java.lang.Integer.parseInt;

public class a20 {

  private final BufferedReader input;

  enum Type {FlipFlop, Conjunction, Broadcast}

  record Module(Type type, String[] destination) { }

  enum Pulse {
    low, high;

    static boolean isHigh(Pulse pulse) {
      return pulse == high;
    }
  }

  enum FlipFlopState {off, on}

  record Count(int low, int high) {
    Count add(Count other) {
      return new Count(low + other.low, high + other.high);
    }
  }

  void run() throws IOException {
    var modules = new HashMap<String, Module>();
    for (String line; (line = input.readLine()) != null; ) {
      var tokens = line.split(" -> |, ");
      var destination = Arrays.copyOfRange(tokens, 1, tokens.length);
      switch (tokens[0].charAt(0)) {
        case '%' -> modules.put(tokens[0].substring(1), new Module(Type.FlipFlop, destination));
        case '&' -> modules.put(tokens[0].substring(1), new Module(Type.Conjunction, destination));
        default -> {
          assert "broadcaster".equals(tokens[0]);
          modules.put(tokens[0], new Module(Type.Broadcast, destination));
        }
      }
    }

    var flipFlopStates = new HashMap<String, FlipFlopState>();

    var conjunctionStates = new HashMap<String, Map<String, Pulse>>();
    for (Map.Entry<String, Module> entry : modules.entrySet()) {
      var from = entry.getKey();
      for (String to : entry.getValue().destination) {
        var module = modules.get(to);
        if (module != null && module.type == Type.Conjunction) conjunctionStates
            .computeIfAbsent(to, _ -> new HashMap<>())
            .put(from, Pulse.low);
      }
    }

    var count = new Count(0, 0);
    for (int i = 0; i < 1000; i++) {
      count = count.add(pressButton(modules, flipFlopStates, conjunctionStates));
    }
    System.out.println(count.low * count.high);
  }

  private static Count pressButton(
      HashMap<String, Module> modules,
      HashMap<String, FlipFlopState> flipFlopStates,
      HashMap<String, Map<String, Pulse>> conjunctionStates
  ) {
    int[] counts = new int[2];

    record Event(String from, String to, Pulse pulse) { }

    var queue = new ArrayDeque<Event>();
    queue.addLast(new Event("button", "broadcaster", Pulse.low));
    for (Event event; (event = queue.pollFirst()) != null; ) {
      counts[event.pulse.ordinal()]++;
      var to = modules.get(event.to);
      if (to == null) continue;
      var nextPulse = switch (to.type) {
        case FlipFlop -> switch (event.pulse) {
          case low -> {
            var state = flipFlopStates.compute(event.to, (_, previousState) -> switch (previousState) {
              case off -> FlipFlopState.on;
              case on -> FlipFlopState.off;
              case null -> FlipFlopState.on;
            });
            yield Optional.of(switch (state) {
              case off -> Pulse.low;
              case on -> Pulse.high;
            });
          }
          case high -> Optional.<Pulse>empty();
        };
        case Conjunction -> {
          var state = conjunctionStates.get(event.to);
          state.put(event.from, event.pulse);
          yield Optional.of(state.values().stream().allMatch(Pulse::isHigh) ? Pulse.low : Pulse.high);
        }
        case Broadcast -> Optional.of(event.pulse);
      };
      var source = event.to;
      nextPulse.ifPresent(pulse -> {
        for (String destination : to.destination) {
          queue.addLast(new Event(source, destination, pulse));
        }
      });
    }
    return new Count(counts[0], counts[1]);
  }

  public static void main(String[] args) throws IOException, InterruptedException {
    new a20().run();
  }

  //<editor-fold desc="Initialization">
  public a20() throws IOException, InterruptedException {
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