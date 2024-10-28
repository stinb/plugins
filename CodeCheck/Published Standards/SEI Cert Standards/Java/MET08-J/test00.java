import java.net.MalformedURLException;
import java.net.URI;
import java.net.URISyntaxException;
import java.net.URL;
import java.security.Key;
import java.security.interfaces.RSAKey;
import java.security.interfaces.RSAPrivateKey;



public final class CaseInsensitiveString0 {
  private String s;

  public CaseInsensitiveString0(String s) {
    if (s == null) {
      throw new NullPointerException();
    }
    this.s = s;
  }

  // This method violates symmetry
  public boolean equals(Object o) { // UndCC_Violation
    if (o instanceof CaseInsensitiveString0) {
      return s.equalsIgnoreCase(((CaseInsensitiveString0)o).s);
    }

    if (o instanceof String) {
      return s.equalsIgnoreCase((String)o);
    }
    return false;
  }

  // Comply with MET09-J
  public int hashCode() {/* ... */}

  public static void main(String[] args) {
    CaseInsensitiveString0 cis = new CaseInsensitiveString0("Java");
    String s = "java";
    System.out.println(cis.equals(s)); // Returns true
    System.out.println(s.equals(cis)); // Returns false
  }
}



public final class CaseInsensitiveString1 {
  private String s;

  public CaseInsensitiveString1(String s) {
    if (s == null) {
      throw new NullPointerException();
    }
    this.s = s;
  }

  public boolean equals(Object o) { // UndCC_Valid
    return o instanceof CaseInsensitiveString1 &&
        ((CaseInsensitiveString1)o).s.equalsIgnoreCase(s);
  }

  public int hashCode() {/* ... */}

  public static void main(String[] args) {
    CaseInsensitiveString1 cis = new CaseInsensitiveString1("Java");
    String s = "java";
    System.out.println(cis.equals(s)); // Returns false now
    System.out.println(s.equals(cis)); // Returns false now
  }
}



public class Card0 {
  private final int number;

  public Card0(int number) {
    this.number = number;
  }

  public boolean equals(Object o) { // UndCC_Valid
    if (!(o instanceof Card0)) {
      return false;
    }

    Card0 c = (Card0)o;
    return c.number == number;
  }

  public int hashCode() {/* ... */}

}

class XCard0A extends Card0 {
  private String type;
  public XCard0A(int number, String type) {
    super(number);
    this.type = type;
  }

  public boolean equals(Object o) { // UndCC_Violation
    if (!(o instanceof Card0)) {
      return false;
    }

    // Normal Card0, do not compare type
    if (!(o instanceof XCard0A)) {
      return o.equals(this);
    }

    // It is an XCard0A, compare type as well
    XCard0A xc = (XCard0A)o;
    return super.equals(o) && xc.type == type;
  }

  public int hashCode() {/* ... */}

  public static void main(String[] args) {
    XCard0A p1 = new XCard0A(1, "type1");
    Card0 p2 = new Card0(1);
    XCard0A p3 = new XCard0A(1, "type2");
    System.out.println(p1.equals(p2)); // Returns true
    System.out.println(p2.equals(p3)); // Returns true
    System.out.println(p1.equals(p3)); // Returns false
                                       // violating transitivity
  }
}



class XCard0B {
  private String type;
  private Card0 card; // Composition

  public XCard0B(int number, String type) {
    card = new Card0(number);
    this.type = type;
  }

  public Card0 viewCard() {
    return card;
  }

  public boolean equals(Object o) { // UndCC_Valid
    if (!(o instanceof XCard0B)) {
      return false;
    }

    XCard0B cp = (XCard0B)o;
    return cp.card.equals(card) && cp.type.equals(type);
  }

  public int hashCode() {/* ... */}

  public static void main(String[] args) {
    XCard0B p1 = new XCard0B(1, "type1");
    Card0 p2 = new Card0(1);
    XCard0B p3 = new XCard0B(1, "type2");
    XCard0B p4 = new XCard0B(1, "type1");
    System.out.println(p1.equals(p2)); // Prints false
    System.out.println(p2.equals(p3)); // Prints false
    System.out.println(p1.equals(p3)); // Prints false
    System.out.println(p1.equals(p4)); // Prints true
  }
}



public class Card1 {
  private final int number;

  public Card1(int number) {
    this.number = number;
  }

  public boolean equals(Object o) { // UndCC_Valid
    if (!(o.getClass() == this.getClass())) {
      return false;
    }

    Card1 c = (Card1)o;
    return c.number == number;
  }

  public int hashCode() {/* ... */}

}



public class Filter0 {
  public static void main(String[] args) throws MalformedURLException {
    final URL allowed = new URL("http://mailwebsite.com");
    if (!allowed.equals(new URL(args[0]))) { // UndCC_Violation
      throw new SecurityException("Access Denied");
    }
    // Else proceed
  }
}



public class Filter1 {
  public static void main(String[] args) throws MalformedURLException {
    final URL allowed = new URL("http://mailwebsite.com");
    if (!allowed.toString().equals(new URL(args[0]).toString())) { // UndCC_Valid
      throw new SecurityException("Access Denied");
    }
    // Else proceed
  }
}



public class Filter2 {
  public static void main(String[] args)
                     throws MalformedURLException, URISyntaxException {
    final URI allowed = new URI("http://mailwebsite.com");
    if (!allowed.equals(new URI(args[0]))) { // UndCC_Valid
      throw new SecurityException("Access Denied");
    }
    // Else proceed
  }
}



public class KeyComparator0 {
  private static boolean keysEqual(Key key1, Key key2) { // UndCC_Violation
    if (key1.equals(key2)) {
      return true;
    }
    return false;
  }
}



public class KeyComparator1 {
  private static boolean keysEqual(Key key1, Key key2) { // UndCC_Valid
    if (key1.equals(key2)) {
      return true;
    }

    if (Arrays.equals(key1.getEncoded(), key2.getEncoded())) {
      return true;
    }

    // More code for different types of keys here.
    // For example, the following code can check if
    // an RSAPrivateKey and an RSAPrivateCrtKey are equal:
    if ((key1 instanceof RSAPrivateKey) &&
        (key2 instanceof RSAPrivateKey)) {

      if ((((RSAKey)key1).getModulus().equals(
          ((RSAKey)key2).getModulus())) &&
        (((RSAPrivateKey) key1).getPrivateExponent().equals(
          ((RSAPrivateKey) key2).getPrivateExponent()))) {
        return true;
      }
    }
    return false;
  }
}
