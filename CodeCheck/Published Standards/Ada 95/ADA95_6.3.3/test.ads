package Test_6_3_3 is

   Global_Flag : Boolean := False;

   Atomic_Flag : Boolean := False;
   pragma Atomic (Atomic_Flag);

   task type Worker is
      entry Go;
      entry Stop;
   end Worker;

   Trigger : Worker;

   procedure Do_Abort;

   procedure Async_Update;

   procedure If_Then_Abort;

   procedure Plain_Assignment;

   G1, G2 : Boolean := False;

   Second_Trigger : Worker;

   procedure Multi_Target_Abort;

   procedure Nested_Async;

   Out_Param_Target : Boolean := False;

   procedure Set_Via_Out_Param (X : out Boolean);

end Test_6_3_3;
