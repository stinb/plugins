class TryFinally_2 {
    private static boolean doLogic() {
        try {
            throw new IllegalStateException();
        } finally {
            int c = 0;
            try {
                while (c != -1) {
                    if (c > 128) {
                        break;
                    }
                }
            } catch (Exception e) {
                // Forward to handler
            }
            System.out.println("logic done");
        }
        // Any return statements must go here; applicable only when exception is thrown
        // conditionally
    }
}