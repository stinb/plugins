final class Flag_n {
    private boolean flag = true;

    public void toggle() { // Unsafe
        flag = !flag; // UndCC_Violation
    }

    public boolean getFlag() { // Unsafe
        return flag; // UndCC_Violation
    }
}