
public class StringDemo2 {
  public static String trim2(String string) {
    char ch2;
    int i;
    for (i = 0; i < string.length(); i += 1) {
      ch2 = string.charAt(i);
      if (!Character.isLetter(ch2)) { // UndCC_Violation - trim2 method may fail because char is passed into isLetter method.
                                      // Methods that only accept a char value cannot support supplementary characters
        break;
      }
    }
    return string.substring(i);
  }

  public static void characterMethods(String string) {
    char[] a;
    CharSequence seq;
    Character anotherCharacter;
    Object obj;
    char ch;
    int i;
    for (i = 0; i < string.length(); i += 1) {
      ch = string.charAt(i);
      Character.charValue();                  // UndCC_Valid - no char or char[]
      Character.codePointAt(a, i);            // UndCC_Violation
      Character.codePointAt(a, i, i);         // UndCC_Violation
      Character.codePointAt(seq, i);          // UndCC_Valid - no char or char[]
      Character.codePointBefore(a, i);        // UndCC_Violation
      Character.codePointBefore(a, i, i);     // UndCC_Violation
      Character.codePointBefore(seq, i);      // UndCC_Valid - no char or char[]
      Character.codePointCount(a, i, i);      // UndCC_Violation
      Character.codePointCount(seq, i, i);    // UndCC_Valid - no char or char[]
      Character.compare(ch, ch);              // UndCC_Violation
      Character.compareTo(anotherCharacter);  // UndCC_Valid - no char or char[]
      Character.digit(ch, i);                 // UndCC_Violation
      Character.equals(obj);                  // UndCC_Valid - no char or char[]
      Character.forDigit(i, i);               // UndCC_Valid - no char or char[]
      Character.getDirectionality(ch);        // UndCC_Violation
      Character.getNumericValue(ch);          // UndCC_Violation
      Character.getType(ch);                  // UndCC_Violation
      Character.hashCode();                   // UndCC_Valid - no char or char[]
      Character.isDefined(ch);                // UndCC_Violation
      Character.isDigit(ch);                  // UndCC_Violation
      Character.isHighSurrogate(ch);          // UndCC_Violation
      Character.isIdentifierIgnorable(ch);    // UndCC_Violation
      Character.isISOControl(ch);             // UndCC_Violation
      Character.isJavaIdentifierPart(ch);     // UndCC_Violation
      Character.isJavaIdentifierStart(ch);    // UndCC_Violation
      Character.isJavaLetter(ch);             // UndCC_Violation
      Character.isJavaLetterOrDigit(ch);      // UndCC_Violation
      Character.isLetter(ch);                 // UndCC_Violation
      Character.isLetterOrDigit(ch);          // UndCC_Violation
      Character.isLowerCase(ch);              // UndCC_Violation
      Character.isLowSurrogate(ch);           // UndCC_Violation
      Character.isMirrored(ch);               // UndCC_Violation
      Character.isSpace(ch);                  // UndCC_Violation
      Character.isSpaceChar(ch);              // UndCC_Violation
      Character.isSurrogate(ch);              // UndCC_Violation
      Character.isSurrogatePair(ch, ch);      // UndCC_Violation
      Character.isTitleCase(ch);              // UndCC_Violation
      Character.isUnicodeIdentifierPart(ch);  // UndCC_Violation
      Character.isUnicodeIdentifierStart(ch); // UndCC_Violation
      Character.isUpperCase(ch);              // UndCC_Violation
      Character.isWhitespace(ch);             // UndCC_Violation
      Character.reverseBytes(ch);             // UndCC_Violation
      Character.toCodePoint(ch, ch);          // UndCC_Violation
      Character.toLowerCase(ch);              // UndCC_Violation
      Character.toString(ch);                 // UndCC_Violation
      Character.toTitleCase(ch);              // UndCC_Violation
      Character.toUpperCase(ch);              // UndCC_Violation
      Character.valueOf(ch);                  // UndCC_Violation
    }
  }
}