public class Test {
    public static void main(String args[]) {
        int num = 2;
        // UNDCC_Valid
        switch (num) {
            case 1:
                break;
            case 2:
                break;
            case 3:
                break;
            default:
                break;
        }

        
        switch (num) {
            case 1:
                break;
            default:  // UndCC_Violation
                break;
            case 2:
                break;
        }

        
        switch (num) {
            default:  // UndCC_Violation
                break;
            case 1:
                break;
            case 2:
                break;
        }
    }
}
