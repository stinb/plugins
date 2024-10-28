import java.util.HashSet;
import java.util.concurrent.ConcurrentLinkedDeque;

public class ShortSet_n {
  public static void main(String[] args) {
    HashSet<Short> s = new HashSet<Short>();
    ConcurrentLinkedDeque<Short> d = new ConcurrentLinkedDeque<Short>();
    for (int i = 0; i < 10; i++) {
      s.add((short) i); // Cast required so that the code compiles
      s.remove(i); // UndCC_Violation, Tries to remove an Integer

      d.add((short) i); // Cast required so that the code compiles
      d.remove(i); // UndCC_Violation, Tries to remove an Integer
    }
    System.out.println(s.size());
    System.out.println(d.size());
  }
}
