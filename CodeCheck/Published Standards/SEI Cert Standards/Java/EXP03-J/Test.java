import java.util.*;
public class Test{

static Comparator<Integer> cmp = new Comparator<Integer>() {
  public int compare(Integer i, Integer j) {
    return i < j ? -1 : (i == j ? 0 : 1); // UNDCC_Violation
  }
};

static Comparator<Integer> cmp1 = new Comparator<Integer>() {
  public int compare1(Integer i, Integer j) {
    return i < j ? -1 : (i > j ? 1 : 0) ; // UNDCC_Valid
  }
};

public static void main(String[] args) {
  
  Integer i1 = 100;
  Integer i2 = 100;
  Integer i3 = 1000;
  Integer i4 = 1000;

  System.out.println(i1 == i2); // UNDCC_Violation
  System.out.println(i1 != i2); // UNDCC_Violation
  System.out.println(i3 == i4); // UNDCC_Violation
  System.out.println(i3 != i4); // UNDCC_Violation
  System.out.println(i1.equals(i2)); // UNDCC_Valid
  System.out.println(!i1.equals(i2)); // UNDCC_Valid
  System.out.println(i3.equals(i4)); // UNDCC_Valid
  System.out.println(!i3.equals(i4)); // UNDCC_Valid
  
  
  // Create an array list of integers
  ArrayList<Integer> list1 = new ArrayList<Integer>();
  
  for (int i = 0; i < 10; i++) {
    list1.add(i + 1000);
  }
  
  // Create another array list of integers, where each element
  // has the same value as the first one
  ArrayList<Integer> list2 = new ArrayList<Integer>();
  for (int i = 0; i < 10; i++) {
    list2.add(i + 1000);
  }
  
  // Count matching values
  int counter = 0;
  for (int i = 0; i < 10; i++) {
    if (list1.get(i).equals(list2.get(i))) {  // UNDCC_Valid
      counter++;
    }
  }
  
  // Print the counter: 10 in this example
  System.out.println(counter);
  // Create an array list of integers, where each element
  // is greater than 127
  ArrayList<Integer> list3 = new ArrayList<Integer>();
  Integer[] a1 = {1, 2, 3};
  for (int i = 0; i < 10; i++) {
    list3.add(i + 1000);
  }
  
  // Create another array list of integers, where each element
  // has the same value as the first list
  ArrayList<Integer> list4 = new ArrayList<Integer>();
  for (int i = 0; i < 10; i++) {
    list4.add(i + 1000);
  }
  
  // Count matching values
  int counter1 = 0;
  for (int i = 0; i < 10; i++) {
    if (list3.get(i) == list4.get(i)) {  // UNDCC_Violation
      counter1++;
    }
  }
  
  // Print the counter: 0 in this example
  System.out.println(counter1);
}
}
