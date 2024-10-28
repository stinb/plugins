public class Test00 {
    public final Integer integer;    // UndCC_Valid - immutable object
    public final Integer[] integers; // UndCC_Violation - public array

    public Test00() {
        integer = Integer.valueOf(0);
        integers = new Integer[64];
    }

    public static void main(String[] args) {
        Test00 test = new Test00();

        // System.out.println(test.integer);
        // test.integer = Integer.valueOf(42);
        // System.out.println(test.integer);

        System.out.println(test.integers[0]);
        test.integers[0] = 42;
        System.out.println(test.integers[0]);
    }
}
