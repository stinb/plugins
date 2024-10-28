public final class USCurrency {
    private final int quarters;
    private final int dimes;
    private final int nickels;
    private final int pennies;

    public USCurrency(Builder builder) {
        this.quarters = builder.quarters;
        this.dimes = builder.dimes;
        this.nickels = builder.nickels;
        this.pennies = builder.pennies;
    }

    // Static class member
    public static class Builder {
        private int quarters = 0;
        private int dimes = 0;
        private int nickels = 0;
        private int pennies = 0;

        public static Builder newInstance() {
            return new Builder();
        }

        private Builder() {
        }

        // Setter methods
        public Builder setQuarters(int quantity) {
            this.quarters = quantity;
            return this;
        }

        public Builder setDimes(int quantity) {
            this.dimes = quantity;
            return this;
        }

        public Builder setNickels(int quantity) {
            this.nickels = quantity;
            return this;
        }

        public Builder setPennies(int quantity) {
            this.pennies = quantity;
            return this;
        }

        public USCurrency build() {
            return new USCurrency(this);
        }
    }
}