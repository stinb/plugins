import java.util.Arrays;
import java.util.Collections;
import java.util.List;

public class Test04 {
    private final Integer[] unmodifiableArray;    // UndCC_Valid - nonpublic array
    private final Integer[] modifiableArray;      // UndCC_Valid - nonpublic array
    private final List<Integer> unmodifiableList; // UndCC_Valid - nonpublic unmodifiable collection
    private final List<Integer> modifiableList;   // UndCC_Valid - nonpublic modifiable collection

    public Test04() {
        unmodifiableArray = new Integer[64];
        modifiableArray = new Integer[64];
        unmodifiableList = Collections.unmodifiableList(Arrays.asList(unmodifiableArray));
        modifiableList = Arrays.asList(modifiableArray);
    }

    public final List<Integer> getUnmodifiableList() {
        return unmodifiableList; // UndCC_Valid - public unmodifiable collection
    }

    public final List<Integer> getModifiableList() {
        return modifiableList; // UndCC_Violation - public modifiable collection
    }

    public static void main(String[] args) {
        Test04 test = new Test04();

        // System.out.println(test.getUnmodifiableList().get(0));
        // test.getUnmodifiableList().set(0, Integer.valueOf(42));
        // System.out.println(test.getUnmodifiableList().get(0));

        System.out.println(test.getModifiableList().get(0));
        test.getModifiableList().set(0, Integer.valueOf(42));
        System.out.println(test.getModifiableList().get(0));
    }
}
