public class Storage {
    private static BankOperations bop;  // UNDCC_Violation, uses a nonfinal static variable
   
    public static void store(BankOperations bo) {
    // Store only if it is initialized
      if (bop == null) {
        if (bo == null) {
          System.out.println("Invalid object!");
          System.exit(1);
        }
        bop = bo;
      }
    }
  }
