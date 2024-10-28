public class Test {
  public void test(){
    File someFile = new File("someFileName.txt");
    // Do something with someFile
    someFile.delete(); // UNDCC_Violation
    
  }
  public void test1(){
    
    File someFile = new File("someFileName.txt");
    // Do something with someFile
    if (!someFile.delete()) { // UNDCC_Valid
      // Handle failure to delete the file
    }
  }
  public void test2() {
    String original = "insecure";
    original.replace('i', '9'); // UNDCC_Violation
    System.out.println(original);
  }

  public void test3() {
    String original = "insecure";
    original = original.replace('i', '9'); // UNDCC_Valid
    System.out.println(original);
  }
}
