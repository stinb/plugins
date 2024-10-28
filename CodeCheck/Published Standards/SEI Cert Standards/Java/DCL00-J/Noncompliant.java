class A_2 {
    public static final int a = B_2.b + 1; // UndCC_Violation
    // ...
}

class B_2 {
    public static final int b = A_2.a + 1; // UndCC_Violation
    // ...
}