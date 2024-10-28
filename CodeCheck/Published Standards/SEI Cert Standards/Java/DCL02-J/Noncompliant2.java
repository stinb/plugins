import java.util.List;
import java.util.Arrays;

public class Noncompliant {
    public static void main(String args[]) {
        List<Integer> list = Arrays.asList(new Integer[] { 13, 14, 15 });
        boolean first = true;

        System.out.println("Processing list...");
        for (Integer i : list) {
            if (first) {
                first = false;
            }
            System.out.println(" New item: " + i);
            // Process i
        }

        System.out.println("Modified list?");
        for (Integer i : list) {
            i = new Integer(100); // UndCC_Violation
            System.out.println("List item: " + i);
        }

    }

}
