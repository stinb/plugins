package body Test is

   task body Dispatch is
      C : Natural;
   begin
      loop
         select
            when Transmit'Count > 0 and Receive'Count = 0 =>  -- UndCC_Violation
               accept Transmit;
         or
            accept Receive;
         end select;
      end loop;
   end Dispatch;

   protected body Bounded_Buffer is
      entry Put when True is
         N : Natural;
      begin
         N := Put'Count;  -- UndCC_Valid
      end Put;
   end Bounded_Buffer;

   procedure Check_Callable is
   begin
      if D'Callable then  -- UndCC_Violation
         D.Receive;
      end if;
   end Check_Callable;

   procedure Check_Terminated is
   begin
      if D'Terminated then  -- UndCC_Violation
         null;
      end if;
   end Check_Terminated;

   procedure Check_Negated_Callable is
   begin
      if not D'Callable then  -- UndCC_Violation
         null;
      end if;
   end Check_Negated_Callable;

   procedure Check_Dotted_Name is
   begin
      if Test.D'Callable then  -- UndCC_Violation
         Test.D.Receive;
      end if;
   end Check_Dotted_Name;

end Test;
