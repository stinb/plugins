public class Widget {
  private int total; /* UndCC_Valid Declared private */

  public int getTotal () {
    return total;
  }

  // Definitions for add() and remove() remain the same

  private static final HashMap<Integer, String> hm = new HashMap<Integer, String>(); /* UndCC_Valid Mutable field declared private */

  public static String getElement(int key) {
    return hm.get(key);
  }

  private static final String[] items = {/* ... */};  //* UndCC_Valid Final array declared private */

  public static final String getItem(int index) {
    return items[index];
  }

  public static final int getItemCount() {
    return items.length;
  }
}