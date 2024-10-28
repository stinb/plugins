final class Flag2 {
    private volatile boolean flag = true;

    public synchronized void toggle() { // UndCC_Valid, syncrhonized
        flag ^= true; // Same as flag = !flag;
    }

    public boolean getFlag() { // UndCC_Valid, read-only, volatile
        return flag;
    }
}