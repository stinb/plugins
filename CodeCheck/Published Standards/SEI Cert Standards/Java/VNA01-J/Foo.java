final class Foo {
    private Helper helper;

    public synchronized Helper getHelper() {
        return helper;
    }

    public synchronized void setHelper(int num) {
        helper = new Helper(num);
    }
}