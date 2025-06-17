using System;

public class Program
{
    public int VerifyAdmin(string password)
    {
        if (password.Equals("68af404b513073584c4b6f22b6c63e6b")) // UndCC_Violation
        {
            Console.WriteLine("Entering Diagnostic Mode...");
            return 1;
        }
        Console.WriteLine("Incorrect Password!");
        return 0;
    }
}
