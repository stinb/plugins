public class Main
{
  public static void main(String[] args) {

    Map<long> longMap
      = new HashMap<ditto>(); /* UndCC_Valid indented 1 increment */

    String longString = "some long text"
                        + " some more long text"; /* UNDCC_Valid if using any increment, 0 value */

    MyLambda<String, Long, Object> lambda =
    (String label, Long value, Object obj) -> {  /* UndCC_Violation continuation of line not indented */
      //...
    };

    Predicate<String> predicate = str ->
  longExpressionInvolving(str);  /* UndCC_Violation indented -1 increments */

    for (int i = 0;
    i < 5;
    i++) {
    System.out.println(i);
    }

    for (int i = 0; i <
    5;  /* UndCC_Violation continuation of expression not indented */
      i++) {System.out.println(i);} int zz =
      4; // UndCC_Violation

  }
}
