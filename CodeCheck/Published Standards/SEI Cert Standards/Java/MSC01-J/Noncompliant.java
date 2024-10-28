public class Noncompliant {
    public int nop() {
        while (true) { // UndCC_Violation
        }
    }

    public void infiniteLoopUsingFor() {
        for (;;) { // UndCC_Violation
        }
    }

    public void infiniteLoopUsingDoWhile() {
        do { // UndCC_Violation
        } while (true);
    }
}
