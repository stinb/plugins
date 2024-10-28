final class Adder_n {
    private int a;
    private int b;

    public int getSum() {
        return a + b; // UndCC_Violation
    }

    public void setValues(int a, int b) {
        this.a = a; // UndCC_Violation
        this.b = b;
    }
}