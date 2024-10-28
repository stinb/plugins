class A_4 {
    public static int a = B_4.b(); // UndCC_Valid
}

class B_4 {
    public static int b() {
        return B_4.c();
    }

    public static int c() {
        return 1;
    }
}