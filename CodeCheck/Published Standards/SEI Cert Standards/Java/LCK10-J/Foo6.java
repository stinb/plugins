final class Foo6 {
    private Helper2 helper = null;

    public Helper2 getHelper() {
        Helper2 h = helper; // Only unsynchronized read of helper
        if (h == null) {
            synchronized (this) {
                h = helper; // In synchronized block, so this is safe
                if (h == null) {
                    h = new Helper2(42);
                    helper = h;
                }
            }
        }
        return h;
    }
}