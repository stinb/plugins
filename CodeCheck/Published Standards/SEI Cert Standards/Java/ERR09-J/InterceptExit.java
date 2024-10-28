public class InterceptExit {
    public static void main(String[] args) {
        PasswordSecurityManager secManager = new PasswordSecurityManager();
        System.setSecurityManager(secManager);
        try {
            // ...
            System.exit(1); // Abrupt exit call
        } catch (Throwable x) {
            if (x instanceof SecurityException) {
                System.out.println("Intercepted System.exit()");
                // Log exception
            } else {
                // Forward to exception handler
            }
        }

        // ...
        secManager.setExitAllowed(true); // Permit exit
        // System.exit() will work subsequently
        // ...
    }
}