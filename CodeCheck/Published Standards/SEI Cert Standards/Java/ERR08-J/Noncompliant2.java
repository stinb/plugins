import java.io.*;

class Noncompliant2
{
    public static void main (String[] args)
    {
        // Initializing String variable with null value
        String ptr = null;

        try
        {
            // because ptr is null
            if (ptr.equals("gfg"))
                System.out.print("Same");
            else
                System.out.print("Not Same");
        }
        catch(NullPointerException e)/* UndCC_Violation - NullPointerException caught */
        {
            System.out.print("NullPointerException Caught"); 
        }
    }
}
