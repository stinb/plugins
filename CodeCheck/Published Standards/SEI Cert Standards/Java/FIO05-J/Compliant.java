import java.nio.CharBuffer;

final class Wrap {
    private char[] dataArray;

    public Wrap() {
        dataArray = new char[10];
        // Initialize
    }

    public CharBuffer getBufferCopy() {
        return CharBuffer.wrap(dataArray).asReadOnlyBuffer(); // UndCC_Valid
    }
}

final class Warp_2 {
    private char[] dataArray;

    public Warp_2() {
        dataArray = new char[10];
        // Initialize
    }

    public CharBuffer getBufferCopy() {
        CharBuffer cb = CharBuffer.allocate(dataArray.length);
        cb.put(dataArray);
        return cb; // UndCC_Valid
    }
}

final class Dup {
    CharBuffer cb;

    public Dup() {
        cb = CharBuffer.allocate(10);
        // Initialize
    }

    public CharBuffer getBufferCopy() {
        return cb.asReadOnlyBuffer(); // UndCC_Valid
    }
}