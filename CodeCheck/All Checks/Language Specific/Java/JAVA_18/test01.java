public class booleanMethod {

    public boolean isEven(int mynumber)               /* UndCC_Valid */
    {
        return (mynumber % 2 == 0);
    }

    public boolean badFunctionDoesNothing(int mynumber)       /* Valid only if 'bad' specified in Options */ // UndCC_Violation
    {
        return mynumber;
    }

    public boolean requiresNumber(int mynumber)       /* UndCC_Valid */
    {
        return mynumber;
    }

    public static boolean oddNumCheck(int mynumber)   /* UndCC_Violation method not prepended with boolean expression */
    {
        return (mynumber % 2 != 0);
    }

    public static void main(String[] args)
    {
        int mynumber = 130;

        if(isEven(mynumber) == true)
            System.out.print("Even Number");
        else
            System.out.print("Odd Number");
    }
}
