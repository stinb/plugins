public interface Player {                  // UndCC_Valid
    public void run();
}

public class Swordsman implements Player { // UndCC_Valid
    public void run() {
        System.out.println("Running slowly");
    }
}

public class Enemy {                       // UndCC_Violation
    public void attack();
}

public enum EnemyState {                   // UndCC_Valid
    WAITING,
    CHASING,
    ATTACKING,
    FLEEING,
}
