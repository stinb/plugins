public class MyClass {
  public static void main(String args[]) {
    
    for (float x = 1f; x < 2; x += .1f) { // UNDCC_Violation
      System.out.println("hi");
    }
    
    double x = 0;
    
    for (;;x++) { // UNDCC_Violation
      break;
    }
    
    for (;true;) {
      break;
    }
    
    for (int z=1;;) { 
      break;
    }
    
    for(;;) {
      break;
    }
    
    for (Integer i = 1;;) { 
      break;
    }
    
    for (Integer i = 1;;) {
      break;
    }
    
    for (double i = 1;;) { // UNDCC_Violation
      break;
    }
    for (Double i = new Double(1);;) { // UNDCC_Violation
      break;
    }
    for (Float i = new Float(1);;) { // UNDCC_Violation
      break;
    }
    for (Float i = 1.0f;;) { // UNDCC_Violation
      break;
    }
    
    int[] numbers = {};
    
    for (int num : numbers) {
    }
    
    while ( x < 10.5) { // UNDCC_Violation
      x++;
    }

    do {
      break;

    } while(true);
    
    float m = 0f;
    int n = 1;
    
    do {
      m++;
    } while(n > m); // UNDCC_Violation



    while (m > n) { // UNDCC_Violation
      m++;
    }
  }
}
