public class Test01 {
    private final Integer integer;    // UndCC_Valid - immutable object
    private final Integer[] integers; // UndCC_Valid - nonpublic array

    public Test01() {
        integer = Integer.valueOf(0);
        integers = new Integer[64];
    }

    public final int getInteger() {
        return integer;  // UndCC_Valid - public immutable object
    }

    public final Integer[] getIntegers() {
        return integers; // UndCC_Violation - public array
    }

    public static void main(String[] args) {
        Test01 test = new Test01();

        // System.out.println(test.getInteger());
        // test.getInteger() = 42;
        // System.out.println(test.getInteger());

        System.out.println(test.getIntegers()[0]);
        test.getIntegers()[0] = 42;
        System.out.println(test.getIntegers()[0]);
    }
}
