public class Noncompliant {
    public static int getAbsAdd(int x, int y) {  /* UndCC_Violation - uses assertions to validate arguments of a public method */
        assert x != Integer.MIN_VALUE;
        assert y != Integer.MIN_VALUE;
        int absX = Math.abs(x);
        int absY = Math.abs(y);
        assert (absX <= Integer.MAX_VALUE - absY);
        return absX + absY;
      }

}
