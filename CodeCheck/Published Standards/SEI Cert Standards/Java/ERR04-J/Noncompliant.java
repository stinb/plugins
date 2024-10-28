class TryFinally_3 {
    private static boolean doLogic() {
        try {
            throw new IllegalStateException();
        } finally {
            System.out.println("logic done");
            return true; // UndCC_Violation
        }
    }
}
