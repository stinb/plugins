
final class Foo3 {
    // Lazy initialization
    private static class Holder {
        static Helper helper = new Helper(1);
    }

    public static Helper getInstance() {
        return Holder.helper;
    }
}