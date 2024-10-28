
class Test01 { // package-private class
  public int x = 0; /* UndCC_FalsePos - OBJ01-J-EX1 */
  protected int y = 0;
}

public class Test02 {

  public static final String y = "Hello World"; /* UndCC_FalsePos - OBJ01-J-EX2 */
  public static String z = "Hello World"; // UNDCC_Violation
  public Object x;  // UNDCC_Violation
  protected boolean t;
  Object w; // UNDCC_Valid
  int v; // UNDCC_Valid

  private class test03 {
    public int x = 0; //* UndCC_FalsePos - OBJ01-J-EX1 */
  }

  public static void main(String args[]) {
    int a = 0;
    int b = 0;
  }
}
