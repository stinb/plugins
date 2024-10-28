class ForLoop {
    public static void main(String[] args){
         for(int i=1; i<11; i++){
              System.out.println("Count is: " + i);
              i--;  // control loop variable is decremented, leading to an infinite loop // UndCC_Violation
         }
    }
}