class Test {
    public static void main(String[] args) {
        double x = 0.0;
        double y = 0.0;
        for (int i = 0; i < 3; i++) {
            x = i - i + 3;   // UNDCC_FalseNeg - Expression value not determined with static analysis
            y = Math.PI + 1; // UNDCC_Violation
        }
    }
}
