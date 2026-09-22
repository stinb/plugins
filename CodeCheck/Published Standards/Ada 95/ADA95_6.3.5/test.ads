package Test_6_3_5 is

   task type Task_A is
      entry E1;
   end Task_A;

   task type Task_B is
      entry E2;
   end Task_B;

   OA : Task_A;
   OB : Task_B;

   task type Self_Caller is
      entry E1;
      entry E2;
   end Self_Caller;

   OS : Self_Caller;

   task type Chain_A is
      entry Go;
   end Chain_A;

   task type Chain_B is
      entry Go;
   end Chain_B;

   task type Chain_C is
      entry Go;
   end Chain_C;

   CA : Chain_A;
   CB : Chain_B;
   CC : Chain_C;

   task type Via_Proc_A is
      entry Go;
   end Via_Proc_A;

   task type Via_Proc_B is
      entry Go;
   end Via_Proc_B;

   VA : Via_Proc_A;
   VB : Via_Proc_B;

   task type Valid_A is
      entry Go;
   end Valid_A;

   task type Valid_B is
      entry Go;
   end Valid_B;

   task type Valid_C is
      entry Go;
   end Valid_C;

   NA : Valid_A;
   NB : Valid_B;
   NC : Valid_C;

   task type Diamond_A is
      entry Go;
   end Diamond_A;

   task type Diamond_B is
      entry Go;
   end Diamond_B;

   task type Diamond_C is
      entry Go;
   end Diamond_C;

   task type Diamond_D is
      entry Go;
   end Diamond_D;

   DA : Diamond_A;
   DB : Diamond_B;
   DC : Diamond_C;
   DD : Diamond_D;

   task type Recur_Caller is
      entry Go;
   end Recur_Caller;

   RC : Recur_Caller;

end Test_6_3_5;
