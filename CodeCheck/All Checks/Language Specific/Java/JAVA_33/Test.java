public class Test {
    public static void main(String args[]) {
        int num = 2;
        switch (num) {
            // UNDCC_Valid
            case 1:
                break;
        
        case 2: // UndCC_Violation
            break;
          
          case 3: // UndCC_Violation
                break;
            default:
                break;
        }
    }
}


