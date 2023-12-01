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

public class a01 {

  private final BufferedReader input;

  void run() throws IOException {
    int sum1 = 0, sum2 = 0;
    String line;
    while ((line = input.readLine()) != null) {
      int first1 = 0, last1 = 0;
      int first2 = 0, last2 = 0;
      State state = State.unknown;
      for (int i = 0; i < line.length(); i++) {
        var c = line.charAt(i);
        state = transform(state, c);
        if (state == State.digit) {
          last1 = last2 = c - '0';
          first1 = first1 != 0 ? first1 : last1;
          first2 = first2 != 0 ? first2 : last2;
        } else if (state.ordinal() < 10) {
          last2 = state.ordinal();
          first2 = first2 != 0 ? first2 : last2;
        }
      }
      sum1 += first1 * 10 + last1;
      sum2 += first2 * 10 + last2;
    }
    System.out.println(sum1);
    System.out.println(sum2);
  }

  enum State {
    digit,
    one, two, three, four, five, six, seven, eight, nine,
    o, on, t, tw, th, thr, thre, f, fo, fou, fi, fiv, s, si, se, sev, seve, e, ei, eig, eigh, n, ni, nin,
    unknown
  }

  State transform(State s, char c) {
    return switch (s) {
      case o, two -> c == 'n' ? State.on : transform(State.unknown, c);
      case on -> c == 'e' ? State.one : transform(State.n, c);
      case t, eight -> switch (c) {
        case 'w' -> State.tw;
        case 'h' -> State.th;
        default -> transform(State.unknown, c);
      };
      case tw -> c == 'o' ? State.two : transform(State.unknown, c);
      case th -> c == 'r' ? State.thr : transform(State.unknown, c);
      case thr -> c == 'e' ? State.thre : transform(State.unknown, c);
      case thre -> c == 'e' ? State.three : transform(State.e, c);
      case f -> switch (c) {
        case 'o' -> State.fo;
        case 'i' -> State.fi;
        default -> transform(State.unknown, c);
      };
      case fo -> c == 'u' ? State.fou : transform(State.o, c);
      case fou -> c == 'r' ? State.four : transform(State.unknown, c);
      case fi -> c == 'v' ? State.fiv : transform(State.unknown, c);
      case fiv -> c == 'e' ? State.five : transform(State.unknown, c);
      case s -> switch (c) {
        case 'i' -> State.si;
        case 'e' -> State.se;
        default -> transform(State.unknown, c);
      };
      case si -> c == 'x' ? State.six : transform(State.unknown, c);
      case se -> c == 'v' ? State.sev : transform(State.e, c);
      case sev -> c == 'e' ? State.seve : transform(State.unknown, c);
      case seve -> c == 'n' ? State.seven : transform(State.e, c);
      case e, one, three, five, nine -> c == 'i' ? State.ei : transform(State.unknown, c);
      case ei -> c == 'g' ? State.eig : transform(State.unknown, c);
      case eig -> c == 'h' ? State.eigh : transform(State.unknown, c);
      case eigh -> c == 't' ? State.eight : transform(State.unknown, c);
      case n, seven -> c == 'i' ? State.ni : transform(State.unknown, c);
      case ni -> c == 'n' ? State.nin : transform(State.unknown, c);
      case nin -> c == 'e' ? State.nine : transform(State.n, c);
      case digit, unknown, four, six -> switch (c) {
        case 'o' -> State.o;
        case 't' -> State.t;
        case 'f' -> State.f;
        case 's' -> State.s;
        case 'e' -> State.e;
        case 'n' -> State.n;
        case '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' -> State.digit;
        default -> State.unknown;
      };
    };
  }

  public static void main(String[] args) throws IOException, InterruptedException {
    new a01().run();
  }

  //<editor-fold desc="Initialization">
  public a01() throws IOException, InterruptedException {
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