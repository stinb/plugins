final class Foo_n {
    private Helper helper;

    public Helper getHelper() {
        return helper;
    }

    public void setHelper(int num) {
        helper = new Helper(num); // UndCC_Violation
    }
}