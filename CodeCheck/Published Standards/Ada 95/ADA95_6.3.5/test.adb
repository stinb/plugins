package body Test_6_3_5 is

   task body Task_A is
   begin
      accept E1 do
         OB.E2;  -- UndCC_Violation
      end E1;
   end Task_A;

   task body Task_B is
   begin
      accept E2 do
         OA.E1;  -- UndCC_Violation
      end E2;
   end Task_B;

   task body Self_Caller is
   begin
      accept E1 do
         OS.E2;  -- UndCC_Violation
      end E1;
      accept E2 do
         OS.E1;  -- UndCC_Violation
      end E2;
   end Self_Caller;

   task body Chain_A is
   begin
      accept Go do
         CB.Go;  -- UndCC_Violation
      end Go;
   end Chain_A;

   task body Chain_B is
   begin
      accept Go do
         CC.Go;  -- UndCC_Violation
      end Go;
   end Chain_B;

   task body Chain_C is
   begin
      accept Go do
         CA.Go;  -- UndCC_Violation
      end Go;
   end Chain_C;

   procedure Call_B_Go is
   begin
      VB.Go;  -- UndCC_Violation
   end Call_B_Go;

   task body Via_Proc_A is
   begin
      accept Go do
         Call_B_Go;
      end Go;
   end Via_Proc_A;

   task body Via_Proc_B is
   begin
      accept Go do
         VA.Go;  -- UndCC_Violation
      end Go;
   end Via_Proc_B;

   task body Valid_A is
   begin
      accept Go do
         NB.Go;  -- UndCC_Valid
      end Go;
   end Valid_A;

   task body Valid_B is
   begin
      accept Go do
         NC.Go;  -- UndCC_Valid
      end Go;
   end Valid_B;

   task body Valid_C is
   begin
      accept Go;
   end Valid_C;

   task body Diamond_A is
   begin
      accept Go do
         DB.Go;  -- UndCC_Valid
         DC.Go;  -- UndCC_Valid
      end Go;
   end Diamond_A;

   task body Diamond_B is
   begin
      accept Go do
         DD.Go;  -- UndCC_Valid
      end Go;
   end Diamond_B;

   task body Diamond_C is
   begin
      accept Go do
         DD.Go;  -- UndCC_Valid
      end Go;
   end Diamond_C;

   task body Diamond_D is
   begin
      accept Go;
   end Diamond_D;

   procedure Recur_1 (N : Integer);
   procedure Recur_2 (N : Integer);

   procedure Recur_1 (N : Integer) is
   begin
      if N > 0 then
         Recur_2 (N - 1);
      end if;
   end Recur_1;

   procedure Recur_2 (N : Integer) is
   begin
      if N > 0 then
         RC.Go;  -- UndCC_Violation
         Recur_1 (N - 1);
      end if;
   end Recur_2;

   task body Recur_Caller is
   begin
      accept Go do
         Recur_1 (3);
      end Go;
   end Recur_Caller;

end Test_6_3_5;
