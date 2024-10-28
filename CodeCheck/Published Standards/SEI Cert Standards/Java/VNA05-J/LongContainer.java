class LongContainer {
    private volatile long i = 0;

    void assignValue(long j) {
        i = j;
    }

    void printLong() {
        System.out.println("i = " + i);
    }
}