final class Foo5 {
    private Helper_n helper = null;

    public Helper_n getHelper() {   //UndCC_Violation
        if (helper == null) { // First read of helper
            synchronized (this) {
                if (helper == null) { // Second read of helper
                    helper = new Helper_n(42);
                }
            }
        }
        return helper; // Third read of helper
    }
}