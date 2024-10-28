public class Player {
    private int currentHealth;
    private int maxHealth;

    public int setCurrentHealth(int newHealth) {
        currentHealth = newHealth;
    }

    public int currentHealth(int newHealth) {        // UNDCC_Violation
        currentHealth = newHealth;
    }

    public int heal(int healing) {                   // UNDCC_Valid - Not a simple "set" method
        currentHealth = currentHealth + healing;
    }

    public int maxHeal(int healing) {                // UNDCC_Valid - Not a simple "set" method
        currentHealth = maxHealth;
    }

    public int conditionalSetHealth(int newHealth) { // UNDCC_Valid - Not a simple "set" method
                                                     // However, it is invalid if searching
                                                     // for complex "set" methods.
        if (newHealth >= 0 && newHealth <= maxHealth) {
            currentHealth = newHealth;
        }
    }

    public int setX(int y) {
        private int x = y;
    }
}

public class Program {
    public static void main() {
        Player player = new Player();
        player.currentHealth(12);
    }
}
