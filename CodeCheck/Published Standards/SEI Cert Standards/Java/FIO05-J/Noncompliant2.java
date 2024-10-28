import java.nio.CharBuffer;

final class Dup_nc {
    CharBuffer cb;

    public Dup_nc() {
        cb = CharBuffer.allocate(10);
        // Initialize
    }

    public CharBuffer getBufferCopy() {
        return cb.duplicate(); // UndCC_Violation
    }
}

final class Dup_nc2 {
    CharBuffer cb;

    public Dup_nc2() {
        cb = CharBuffer.allocate(10);
        // Initialize
    }

    public CharBuffer getBufferCopy() {
        return cb; // UndCC_Violation
    }
}
