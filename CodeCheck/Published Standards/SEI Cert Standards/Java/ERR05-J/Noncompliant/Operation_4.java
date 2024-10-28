import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

class Operation_4 {
  public static void main(String[] args)
  {
    try {
      
      // open files
      System.out.println("Open files");
      BufferedReader reader = new BufferedReader(new FileReader(some_file));
      
      // do some processing
      int a = 45;
      int b = 5;
      
      int div = a / b;
      System.out.println("After dividing a and b ans is " + div);
    }
    
    catch (ArithmeticException ae) {
      
      System.out.println("exception caught");
      
      // display exception details
      System.out.println(ae);
    }
    
    finally {  // UndCC_Violation
      
      System.out.println("Inside finally block");
      
      // close the files irrespective of any exception
      System.out.println("Close files");
      reader.close();
    }
  }
}
