final class Flag_n2 {
    private boolean flag = true;

    public void toggle() { // Unsafe
        flag ^= true; // Same as flag = !flag; UndCC_Violation
    }

    public boolean getFlag() { // Unsafe
        return flag; // UndCC_Violation
    }
}