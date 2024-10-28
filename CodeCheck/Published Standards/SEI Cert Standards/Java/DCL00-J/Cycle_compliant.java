public class Cycle_compliant {
    private final int balance;
    private static final int deposit = (int) (Math.random() * 100); // Random deposit
    private static final Cycle_compliant c = new Cycle_compliant(); // UndCC_Valid, Inserted after initialization of
                                                                    // required fields

    public Cycle_compliant() {
        balance = deposit - 10; // Subtract processing fee
    }

    public static void main(String[] args) {
        System.out.println("The account balance is: " + c.balance);
    }
}