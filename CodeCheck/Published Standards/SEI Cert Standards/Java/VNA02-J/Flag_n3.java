final class Flag_n3 {
    private volatile boolean flag = true;

    public void toggle() { // Unsafe
        flag ^= true; // UndCC_Violation
    }

    public boolean getFlag() { // UndCC_Valid, Safe: read-only, volatile
        return flag;
    }
}