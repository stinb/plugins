public class InterceptExit_n {
    public static void main(String[] args) {
      // ...
      System.exit(1);  // UndCC_Violation, Abrupt exit
      System.out.println("This never executes");
    }
  }  
