public class Noncompliant {
    boolean isName(String s) {
        try {
            String names[] = s.split(" ");

            if (names.length != 2) {
                return false;
            }
            return (isCapitalized(names[0]) && isCapitalized(names[1]));
        } catch (NullPointerException e) { // UNDCC_Violation, catch NullPointerException
            return false;
        }
    }

    boolean isCapitalized(String s) {
        if (s == null) {
            throw new NullPointerException();
        }
        if (s.equals("")) {
            return true;
        }
        String first = s.substring(0, 1);
        String rest = s.substring(1);
        return (first.equals(first.toUpperCase()) &&
                rest.equals(rest.toLowerCase()));
    }

}
