import java.nio.CharBuffer;

final class Wrap_nc {
    private char[] dataArray;

    public Wrap_nc() {
        dataArray = new char[10];
        // Initialize
    }

    public CharBuffer getBufferCopy() {
        return CharBuffer.wrap(dataArray); // UndCC_Violation
    }
}
