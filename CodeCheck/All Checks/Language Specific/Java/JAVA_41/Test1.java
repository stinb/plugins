public class Test1 {
    private int a;
    private int b;

    public static int makeInt(double x) { // UNDCC_Violation
        return (int) x;
    }

    public static int toInt(double x) {   // UNDCC_Valid
        return (int) x;
    }

    public static int makeInt(Animal x) { // UNDCC_Violation
        return x.toInt();
    }

    public static int toInt(Animal x) {   // UNDCC_Valid
        return x.toInt();
    }

    public static Animal makeAnimal(String n) { // UNDCC_Violation
        return new Animal(n, 4);
    }

    public static Animal toAnimal(String n) {   // UNDCC_Valid
        return new Animal(n, 4);
    }

    public static void main(String[] args) {

    }
}
