public class Compliant {
    boolean isName(String s) {
        if (s == null) {
            return false;
        }
        String names[] = s.split(" ");
        if (names.length != 2) {
            return false;
        }
        return (isCapitalized(names[0]) && isCapitalized(names[1]));
    }

    boolean isCapitalized(String s) {
        if (s == null) {
            throw new NullPointerException(); // UNDCC_Valid
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
