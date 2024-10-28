import java.util.*;
public class Test01 {
  
  static boolean isB() {
    return true;
  }
  
  static String getS() {
    return "Hello World";
  }
  
  public static void main(String args[]) {
    isB(); // UNDCC_Violation
    if(isB()) { // UNDCC_Valid
    } 
    else if (isB()) { // UNDCC_Valid
    }
    
    getS(); // UNDCC_Violation
    String s = getS(); // UNDCC_Valid
    
    Stack<Integer> stack = new Stack<Integer>(); // UNDCC_Valid
    stack.push(1); // UNDCC_Violation
    stack.push(2); // UNDCC_Violation
    stack.push(3); // UNDCC_Violation
    System.out.println(stack.pop());// UNDCC_Valid ???????????????
    stack.pop(); // UNDCC_Violation
  }
}

