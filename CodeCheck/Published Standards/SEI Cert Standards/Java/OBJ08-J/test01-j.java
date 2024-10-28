class Test {
private String x;
private double y;
private char z;

public class Point {
  public void getPoint() {
    System.out.println("(" + x + "," + y + "," + z + ")"); // UNDCC_Violation
  }
  public String getX() {
    return x; // UNDCC_Violation
  }
  public double getY() {
    return y; // UNDCC_Violation
  }
  public char getZ() {
    return z; // UNDCC_Violation
  }
}
}



class Test1 {
  private int x;
  private int y;
  
  private class Point {
    private void getPoint() {
      System.out.println("(" + x + "," + y  + ")"); // UNDCC_Valid
    }
    private int getX() {
      return x; // UNDCC_Valid
    }
    private int getY() {
      return y; // UNDCC_Valid
    }
  }
}

class AnotherClass1 {
  public static void main(String[] args) {
    Test1 c = new Test1();
    Test1.Point p = c.new Point();    // Fails to compile
    p.getPoint();
  }
}
