package Test is

   Max_Limit : constant Natural := 100;

   -- Referenced by only one task's guard: no sharing risk.
   Solo_Flag : Boolean := False;

   -- Referenced from a guard by Bad_Manager (nonlocal) and nowhere else:
   -- shared with no other task or outside code, so not itself an
   -- ERR2 case, but still an ERR1 violation for the guard reference.
   Num_Requested : Natural;
   Item_Count    : Natural := 1000;
   pragma Volatile (Item_Count);

   task type Bad_Manager (Personal_Limit : Natural := 1) is
      entry Make_Request (Num : Natural);
      entry Get_Container;
   end Bad_Manager;

   task type Good_Manager (Personal_Limit : Natural := 1) is
      entry Make_Request (Num : Natural);
      entry Get_Container;
   end Good_Manager;

   Shared_Counter : Natural := 0;  -- UndCC_Violation

   task type Reader is
      entry Go;
   end Reader;

   task type Writer is
      entry Go;
   end Writer;

   -- Shared between a task and ordinary sequential code: ERR2.
   Outside_Shared : Natural := 0;  -- UndCC_Violation

   -- Shared between Reader and Writer, but Atomic: no ERR2.
   Flag_Atomic : Boolean := False;
   pragma Atomic (Flag_Atomic);

   protected type Test_And_Set is
      entry Seize;
      procedure Release;
   end Test_And_Set;

   -- An object of a protected type is already synchronized: no ERR2,
   -- even though it is shared between Reader and Writer.
   Single_Task : Test_And_Set;

end Test;
