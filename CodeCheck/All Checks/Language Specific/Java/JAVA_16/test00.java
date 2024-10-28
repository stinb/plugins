public class Player {
    private String id;
    private int currentHealth;
    private int maxHealth;

    public int getCurrentHealth() {
        return currentHealth;
    }

    public int maxHealth() {                       // UndCC_Violation
        return maxHealth;
    }

    public int increaseAndGetHealth(int healing) { // UndCC_Valid - Not a simple "get" method
        currentHealth += healing;
        return currentHealth;
    }

    public double healthDouble(int healing) {      // UndCC_Valid - Not a simple "get" method
        return currentHealth;
    }

    public int getX() {                            // UndCC_Valid - Gets method member
        private int x;
        return x;
    }

    @Override
    public String toString () {                    // UndCC_Valid - Method not named by this class
        return id;
    }
}
