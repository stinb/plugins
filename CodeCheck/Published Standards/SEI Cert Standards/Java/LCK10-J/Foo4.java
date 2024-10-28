final class Foo4 {
    private final ThreadLocal<Foo> perThreadInstance = new ThreadLocal<Foo>();
    private Helper helper = null;

    public Helper getHelper() {
        if (perThreadInstance.get() == null) {
            createHelper();
        }
        return helper;
    }

    private synchronized void createHelper() {
        if (helper == null) {
            helper = new Helper(1);
        }
        // Any non-null value can be used as an argument to set()
        perThreadInstance.set(this);
    }
}