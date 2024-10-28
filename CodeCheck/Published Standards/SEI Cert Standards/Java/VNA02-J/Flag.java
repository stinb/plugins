final class Flag {
    private boolean flag = true;

    public synchronized void toggle() { // UndCC_Valid, synchronized
        flag ^= true; // Same as flag = !flag;
    }

    public synchronized boolean getFlag() { // UndCC_Valid, synchronized
        return flag;
    }
}