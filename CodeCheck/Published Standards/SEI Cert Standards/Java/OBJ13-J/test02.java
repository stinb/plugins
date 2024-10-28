public class Test02 {
    private final Integer[] integers; // UndCC_Valid - nonpublic array

    public final Integer[] getIntegers() {
        return integers.clone();      // UndCC_Valid - public clone of array
    }

    public Test02() {
        integers = new Integer[64];
    }

    public static void main(String[] args) {
        Test02 test = new Test02();

        System.out.println(test.getIntegers()[0]);
        test.getIntegers()[0] = 42;
        System.out.println(test.getIntegers()[0]);
    }
}
