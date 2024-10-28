/* This class is also not thread-safe */
public final class HalfLife {
    private static int lifespan;

    public void decay() {
        lifespan /= 2;      //UndCC_Violation
    }
}
