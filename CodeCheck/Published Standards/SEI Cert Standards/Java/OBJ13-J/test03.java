import java.util.Arrays;
import java.util.Collections;
import java.util.List;

public class Test03 {
    private final Integer[] unmodifiableArray;   // UndCC_Valid - nonpublic array
    private final Integer[] modifiableArray;     // UndCC_Valid - nonpublic array
    public final List<Integer> unmodifiableList; // UndCC_Valid - public unmodifiable collection
    public final List<Integer> modifiableList;   // UndCC_Violation - public modifiable collection

    public Test03() {
        unmodifiableArray = new Integer[64];
        modifiableArray = new Integer[64];
        unmodifiableList = Collections.unmodifiableList(Arrays.asList(unmodifiableArray));
        modifiableList = Arrays.asList(modifiableArray);
    }

    public static void main(String[] args) {
        Test03 test = new Test03();

        // System.out.println(test.unmodifiableList.get(0));
        // test.unmodifiableList.set(0, Integer.valueOf(42));
        // System.out.println(test.unmodifiableList.get(0));

        System.out.println(test.modifiableList.get(0));
        test.modifiableList.set(0, Integer.valueOf(42));
        System.out.println(test.modifiableList.get(0));
    }
}
