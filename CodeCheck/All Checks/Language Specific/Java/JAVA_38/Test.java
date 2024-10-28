public class Test {
  public static void main(String args[]) {
    int num = 2;
    switch (num) {
   } // UNDCC_Violation
    if (true) {
     } // UNDCC_Violation
    do {
      System.out.println("Test");} // UNDCC_Violation
    while (false);
    for (int i = 0; i < 10; i++) {
    } // UNDCC_Valid
  } // UNDCC_Valid
} // UNDCC_Valid



