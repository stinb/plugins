package body Test is

   procedure Initialize (Self : in out Resource) is  -- UndCC_Violation
   begin
      select
         delay 1.0;
      then abort
         null;
      end select;
   end Initialize;

   procedure Finalize (Self : in out Resource) is  -- UndCC_Valid
   begin
      null;
   end Finalize;

   procedure Adjust (Self : in out Resource) is  -- UndCC_Violation
      Local_Task : Some_Task;
   begin
      null;
   end Adjust;

   task body Some_Task is
   begin
      null;
   end Some_Task;

   task body Worker is
   begin
      accept Signal do
         requeue Wait_Item with abort;
      end Signal;
      accept Wait_Item;
   end Worker;

   protected body Guard is

      procedure Do_Async_Select (W : access Worker) is  -- UndCC_Violation
      begin
         select
            W.Signal;
         then abort
            null;
         end select;
      end Do_Async_Select;

      procedure Do_Task_Create is  -- UndCC_Violation
         Local_Task : Some_Task;
      begin
         null;
      end Do_Task_Create;

      procedure Do_Timed_Call (W : access Worker) is  -- UndCC_Valid
      begin
         select
            W.Signal;
         else
            null;
         end select;
      end Do_Timed_Call;

      procedure Clean is  -- UndCC_Valid
      begin
         null;
      end Clean;

      procedure Uppercase_Async_Select is  -- UndCC_Violation
      begin
         SELECT
            delay 1.0;
         THEN ABORT
            null;
         END SELECT;
      end Uppercase_Async_Select;

      procedure Do_Requeue (W : access Worker) is  -- UndCC_Valid
      begin
         requeue W.Wait_Item with abort;
      end Do_Requeue;

   end Guard;

   procedure Ordinary_Subprogram is  -- UndCC_Valid
      Local_Task : Some_Task;
   begin
      select
         delay 1.0;
      then abort
         null;
      end select;
   end Ordinary_Subprogram;

end Test;
