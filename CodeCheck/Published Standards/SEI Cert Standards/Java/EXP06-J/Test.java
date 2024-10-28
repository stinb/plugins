import java.util.*;
class Test01 {
  private ArrayList<String> names;

  void process(int index) {
    assert names.remove(null); // UNDCC_Violation
    // ...
  }
  
  void process1(int index) {
    boolean nullsRemoved = names.remove(null);
    assert nullsRemoved; // UNDCC_Valid
    // ...
  }
  
  boolean test(int a) {
    a += 5;
    return a >= 20;
  }
  
  public static void main(String args[])
  {
    int value = 15;
    assert value >= 20 : " Underweight"; // UNDCC_Valid
    System.out.println("value is " + value);
    
    int value = 15;
    assert test(value) : " Underweight"; // UNDCC_Violation
    System.out.println("value is " + value);
  }
}

