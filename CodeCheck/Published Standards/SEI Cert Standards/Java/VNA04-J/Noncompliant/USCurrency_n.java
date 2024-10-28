public final class USCurrency_n {
    // Change requested, denomination (optional fields)
    private int quarters = 0;
    private int dimes = 0;
    private int nickels = 0;
    private int pennies = 0;

    public USCurrency_n() {
    }

    // Setter methods
    public USCurrency_n setQuarters(int quantity) {
        quarters = quantity;
        return this;
    }

    public USCurrency_n setDimes(int quantity) {
        dimes = quantity;
        return this;
    }

    public USCurrency_n setNickels(int quantity) {
        nickels = quantity;
        return this;
    }

    public USCurrency_n setPennies(int quantity) {
        pennies = quantity;
        return this;
    }
}