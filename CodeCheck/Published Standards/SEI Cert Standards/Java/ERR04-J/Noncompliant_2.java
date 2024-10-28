class TryFinally_4 {
    private static boolean doLogic() {
        try {
            throw new IllegalStateException();
        } finally {
            System.out.println("logic done");
            break; // UndCC_Violation
        }
    }
}
