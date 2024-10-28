public class Noncompliant {
    public static void main(String argsp[]) {
        int i = 1;
        String s = Double.valueOf(i / 1000.0).toString();
        if (s.equals("0.001")) { // UndCC_Violation
            // ...
        }
    }

}
