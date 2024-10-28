public class Compliant {
    boolean isCapitalized(String s) {
        if (s == null) {
            throw new NullPointerException();   //UNDCC_Valid
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
