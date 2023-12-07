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
import java.util.Arrays;
import java.util.Comparator;
import java.util.List;
import java.util.stream.IntStream;

import static java.lang.Integer.parseInt;
import static java.util.Comparator.comparing;

public class a07 {

  private final BufferedReader input;

  enum Card {
    joker, card2, card3, card4, card5, card6, card7, card8, card9, cardT, cardJ, cardQ, cardK, cardA, count;

    static Card fromChar(char c) {
      return switch (c) {
        case '2' -> card2;
        case '3' -> card3;
        case '4' -> card4;
        case '5' -> card5;
        case '6' -> card6;
        case '7' -> card7;
        case '8' -> card8;
        case '9' -> card9;
        case 'T' -> cardT;
        case 'J' -> cardJ;
        case 'Q' -> cardQ;
        case 'K' -> cardK;
        case 'A' -> cardA;
        default -> throw new IllegalStateException(STR."Unexpected value: \{c}");
      };
    }
  }

  record Hand(int[] type, Card[] cards, int bid) {
    static final Comparator<Hand> COMPARATOR = comparing(Hand::type, Arrays::compare)
        .thenComparing(Hand::cards, Arrays::compare);
  }

  void run() throws IOException {
    var hands1 = new ArrayList<Hand>();
    var hands2 = new ArrayList<Hand>();

    String line;
    while ((line = input.readLine()) != null) {
      var split = line.split(" ");
      final var N = split[0].length();

      var bid = parseInt(split[1]);

      var cards = new Card[N];
      var numberByLabel = new int[Card.count.ordinal()];
      for (int i = 0; i < N; i++) {
        cards[i] = Card.fromChar(split[0].charAt(i));
        numberByLabel[cards[i].ordinal()]++;
      }
      var types = new int[N + 1];
      for (int n : numberByLabel) {
        types[N - n]++;
      }
      hands1.add(new Hand(types, cards, bid));

      var types2 = Arrays.copyOf(types, types.length);
      var cards2 = Arrays.copyOf(cards, cards.length);
      var jokers = 0;
      for (int j = 0; j < cards2.length; j++) {
        if (cards2[j] == Card.cardJ) {
          cards2[j] = Card.joker;
          jokers++;
        }
      }
      types2[N - jokers]--;
      for (int i = 0; i < types2.length; i++) {
        if (types2[i] != 0) {
          types2[i]--;
          types2[i - jokers]++;
          break;
        }
      }
      hands2.add(new Hand(types2, cards2, bid));
    }

    hands1.sort(Hand.COMPARATOR);
    hands2.sort(Hand.COMPARATOR);

    System.out.println(totalWinnings(hands1));
    System.out.println(totalWinnings(hands2));
  }

  int totalWinnings(List<Hand> hands) {
    return IntStream.rangeClosed(1, hands.size())
        .map(i -> hands.get(i - 1).bid() * i)
        .sum();
  }

  public static void main(String[] args) throws IOException, InterruptedException {
    new a07().run();
  }

  //<editor-fold desc="Initialization">
  public a07() throws IOException, InterruptedException {
    input = getInput(getClass().getSimpleName());
  }

  public a07(String example) {
    input = new BufferedReader(new StringReader(example));
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