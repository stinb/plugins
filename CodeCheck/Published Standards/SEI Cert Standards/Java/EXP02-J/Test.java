public class Test {
  public static void main(String[] args) {
    int[] arr1 = new int[20];
    int[] arr2 = new int[20];
    System.out.println(arr1.equals(arr2)); // UNDCC_Violation
    
    System.out.println(Arrays.equals(arr1, arr2)); // UNDCC_Valid
    
    System.out.println(arr1 == arr2); // UNDCC_Valid
  }
}
