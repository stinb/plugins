class ForLoop2 {
    public static void main(String[] args){
         for(int i=1; i<11; i++){
              System.out.println("Count is: " + i);
              i += 2;  // control loop variable is modified // UndCC_Violation
              i = 1;  // control loop variable is modified // UndCC_Violation
         }
    }
}
