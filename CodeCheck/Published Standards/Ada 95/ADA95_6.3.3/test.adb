package body Test_6_3_3 is

   task body Worker is
   begin
      accept Go;
      Global_Flag := True;  -- UndCC_Violation
      Atomic_Flag := True;  -- UndCC_Valid
      Set_Via_Out_Param (Out_Param_Target);  -- UndCC_Valid
      accept Stop;
   end Worker;

   procedure Set_Via_Out_Param (X : out Boolean) is
   begin
      X := True;
   end Set_Via_Out_Param;

   procedure Do_Abort is
   begin
      abort Trigger;  -- UndCC_Violation
   end Do_Abort;

   procedure Async_Update is
   begin
      select
         Trigger.Stop;
      then abort  -- UndCC_Valid
         Global_Flag := True;  -- UndCC_Violation
      end select;
   end Async_Update;

   procedure If_Then_Abort is
      Some_Cond : Boolean := True;
   begin
      if Some_Cond then
         abort Trigger;  -- UndCC_Violation
      end if;
   end If_Then_Abort;

   procedure Plain_Assignment is
   begin
      Global_Flag := False;  -- UndCC_Valid
   end Plain_Assignment;

   procedure Multi_Target_Abort is
   begin
      abort Trigger, Second_Trigger;  -- UndCC_Violation
   end Multi_Target_Abort;

   procedure Nested_Async is
      Cond : Boolean := False;
   begin
      select
         Trigger.Go;
      then abort  -- UndCC_Valid
         if Cond then
            select
               Second_Trigger.Go;
            then abort  -- UndCC_Valid
               G1 := True;  -- UndCC_Violation
            end select;
         else
            G2 := True;  -- UndCC_Violation
         end if;
      end select;
   end Nested_Async;

end Test_6_3_3;
