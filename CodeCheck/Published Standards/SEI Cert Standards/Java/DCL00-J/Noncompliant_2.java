class A_3 {
    public static int a = B_3.b(); // UndCC_Violation

    public static int c() {
        return 1;
    }
}

class B_3 {
    public static int b() {
        return A_3.c();
    }
}