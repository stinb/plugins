public class Test {
    public static void main(String args[]) {
        int num = 2;
        switch (num) {
            case 1:
                break;
            case 2:
                break;
            case 3:
                break;
            default: // UndCC_Violation
                
        }
        switch (num) {
            case 1:
                break;
            case 2:
                break;
            case 3:
                break;
            default:
                // UNDCC_Valid
                break;
        }
    }
}


