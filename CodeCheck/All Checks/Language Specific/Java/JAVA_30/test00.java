public class Program {
    public static void main(String[] args) {
        System.out.println('Hello', 'world');     // UNDCC_Valid
        System.out.println('Hello world');        // UNDCC_Valid
        System.out.println('Hello' , 'world');    // UNDCC_Violation
        System.out.println('Hello world') ;       // UNDCC_Violation
        // System.out.println('Hello' , 'world'); // UNDCC_Violation - violation unless ignoring comments
        /* System.out.println('Hello world') ; */ // UNDCC_Violation - violation unless ignoring comments
    }
}
