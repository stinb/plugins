class LongContainer_n {
    private long i = 0;

    void assignValue(long j) {
        i = j; // UndCC_Violation
    }

    void printLong() {
        System.out.println("i = " + i); // UndCC_Violation
    }
}