import java.lang.Character;
import java.lang.Class;
import java.lang.Thread;
import java.lang.ThreadGroup;
import java.util.Date;
import java.util.Dictionary;
import java.util.Properties;

@Deprecated
public class Base {}

class Inherited extends Base {}

public class Program {
    public static void main(String[] args) {
        Inherited i = new Inherited(); // UNDCC_CF1_Violation

        Character.isJavaLetter('a');         // UNDCC_Violation - Character.isJavaLetter method
        Character.isJavaLetterOrDigit('b');  // UNDCC_Violation - Character.isJavaLetterOrDigit method
        Character.isSpace(' ');              // UNDCC_Violation - Character.isSpace method

        Class.newInstance(); // UNDCC_Violation - Class.newInstance method

        Thread.run();  // UNDCC_Violation - Thread.run method
        Thread.stop(); // UNDCC_Violation - Thread.stop method

        ThreadGroup.setDaemon(); // UNDCC_Violation - ThreadGroup methods

        new Date();   // UNDCC_Violation - Date class
        Date.clone(); // UNDCC_Violation - Date methods

        Dictionary<String, int> d; // UNDCC_Violation - Dictionary class

        Properties.save(); // UNDCC_Violation - Properties.save method
    }
}
