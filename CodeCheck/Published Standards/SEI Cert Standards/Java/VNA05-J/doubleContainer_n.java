class doubleContainer_n {
    private double num = 19.99d;

    void assignValue_2(float k) {
        num = k; // UndCC_Violation
    }

    void printDouble() {
        System.out.println("num = " + num); // UndCC_Violation
    }
}
