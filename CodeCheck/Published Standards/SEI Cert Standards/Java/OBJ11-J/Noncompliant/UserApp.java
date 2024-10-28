public class UserApp {
    public static void main(String[] args) {
      BankOperations bo;
      try {
        bo = new BankOperations();
      } catch (SecurityException ex) { bo = null; }
   
      Storage.store(bo);
      System.out.println("Proceed with normal logic");
    }
  }