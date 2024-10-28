class GrantAccess_2 {
    public static void displayAccountStatus() {
        System.out.println("Account details for admin: XX");
    }
}

class GrantUserAccess_2 extends GrantAccess_2 {
    public static void displayAccountStatus() { // UndCC_Violation, hides the static method
        System.out.println("Account details for user: XX");
    }
}

public class StatMethod_noncompliant {
    public static void choose(String username) {
        GrantAccess_2 admin = new GrantAccess_2();
        GrantAccess_2 user = new GrantUserAccess_2();
        if (username.equals("admin")) {
            admin.displayAccountStatus();
        } else {
            user.displayAccountStatus();
        }
    }

    public static void main(String[] args) {
        choose("user");
    }
}
