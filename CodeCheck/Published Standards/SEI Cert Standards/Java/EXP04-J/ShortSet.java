import java.util.HashSet;
import java.util.HashMap;
import java.util.Map;

public class ShortSet {
  public static void main(String[] args) {
    HashSet<Short> s = new HashSet<Short>();
    Map<Integer, String> map = new HashMap<Integer, String>();

    for (int i = 0; i < 10; i++) {
      s.add((short) i); // UndCC_Valid
      // Remove a Short
      if (s.remove((short) i) == false) {
        System.err.println("Error removing " + i);
      }
      System.err.println(map.get(i)); // UndCC_Valid
    }
    System.out.println(s.size());
  }
}