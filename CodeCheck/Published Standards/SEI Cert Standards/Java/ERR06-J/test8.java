public class Test {
    public static void main(String[] args) {
        Utils.throwException0(new Exception("This is an exception!"));
        Utils.throwException1(new Exception("This is an exception!")); // UndCC_Violation - casting T extends Throwable
    }
}

class Utils {
    @SuppressWarnings("unchecked")
    private static <T extends Throwable> void throwExceptionHelper(Throwable exception, Object dummy) throws T {
        throw (T) exception;
    }
    public static void throwException0(Throwable exception) {
        Utils.<RuntimeException>throwExceptionHelper(exception, null); // UndCC_Violation - casting T extends Throwable
    }

    @SuppressWarnings("unchecked")
    public static <T extends Throwable> void throwException1(Throwable exception) throws T {
        throw (T) exception;
    }
}
