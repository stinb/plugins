import java.util.Date;
import java.util.Calendar;

public class Test
{ 
  public static void main(String[] args)
  {
    char ch1 = '3';
    boolean b1 = Character.isJavaLetter(ch1); // UNDCC_Violation
    boolean b2 = Character.isJavaIdentifierStart(ch1); // UNDCC_Valid

    Date date = new Date(); // UNDCC_Violation
    int month = date.getMonth(); // UNDCC_Violation

    Calendar calendar = Calendar.getInstance();
    int month1 = calendar.get(Calendar.MONTH); // UNDCC_Valid


    
  }


}
