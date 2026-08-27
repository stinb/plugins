package body Test is

   task body Worker is
   begin
      accept Signal;
   end Worker;

   procedure Blocking_Helper (W : access Worker) is
   begin
      W.Signal;
   end Blocking_Helper;

   protected body Guard is

      procedure Do_Delay is  -- UndCC_Violation
      begin
         delay 1.0;
      end Do_Delay;

      procedure Do_Abort (W : access Worker) is  -- UndCC_Violation
      begin
         abort W.all;
      end Do_Abort;

      procedure Do_Select (W : access Worker) is  -- UndCC_Violation
      begin
         select
            W.Signal;
         else
            null;
         end select;
      end Do_Select;

      procedure Call_Entry (W : access Worker) is  -- UndCC_Violation
      begin
         W.Signal;
      end Call_Entry;

      procedure Declare_Task is  -- UndCC_Violation
         Local_Worker : Worker;
      begin
         null;
      end Declare_Task;

      procedure Clean is  -- UndCC_Valid
      begin
         null;
      end Clean;

      procedure Calls_Blocking_Helper (W : access Worker) is  -- UndCC_Violation
      begin
         Blocking_Helper (W);
      end Calls_Blocking_Helper;

      procedure Uppercase_Delay is  -- UndCC_Violation
      begin
         DELAY 1.0;
      end Uppercase_Delay;

      function Clean_Func return Integer is  -- UndCC_Valid
      begin
         return 1;
      end Clean_Func;

      procedure Mutual_A is  -- UndCC_Valid
      begin
         Mutual_B;
      end Mutual_A;

      procedure Mutual_B is  -- UndCC_Valid
      begin
         Mutual_A;
      end Mutual_B;

   end Guard;

end Test;
