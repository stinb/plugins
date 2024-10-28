class A {
    public static final int a = 2;
    // ...
}

class B {
    public static final int b = A.a + 1; // UndCC_Valid
    // ...
}