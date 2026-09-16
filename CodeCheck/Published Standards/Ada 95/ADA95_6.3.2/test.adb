package body Test_6_3_2 is

   task body No_Terminate is
   begin
      loop
         select  -- UndCC_Violation
            accept Retrieve;
         or
            accept Store;
         end select;
      end loop;
   end No_Terminate;

   task body Has_Terminate is
   begin
      loop
         select  -- UndCC_Valid
            accept Retrieve;
         or
            accept Store;
         or
            terminate;
         end select;
      end loop;
   end Has_Terminate;

   task body Has_Delay is
   begin
      loop
         select  -- UndCC_Valid
            accept Retrieve;
         or
            delay 1.0;
         end select;
      end loop;
   end Has_Delay;

   task body Has_Else is
   begin
      loop
         select  -- UndCC_Valid
            accept Retrieve;
         else
            null;
         end select;
      end loop;
   end Has_Else;

   task body Timed_Call_Only is
      Other : Has_Delay;
   begin
      select  -- UndCC_Valid
         Other.Retrieve;
      or
         delay 1.0;
      end select;
   end Timed_Call_Only;

   task body Nested_Select is
      Cond1, Cond2 : Boolean := False;
   begin
      loop
         select  -- UndCC_Valid
            when Cond1 or Cond2 =>
               accept Go do
                  if Cond1 then
                     select
                        accept Stop;
                     or
                        terminate;
                     end select;
                  end if;
               end Go;
         or
            terminate;
         end select;
      end loop;
   end Nested_Select;

   procedure Free_Without_Check (W : in out Worker_Access) is
   begin
      Free (W);  -- UndCC_Violation
   end Free_Without_Check;

   procedure Free_With_Check (W : in out Worker_Access) is
   begin
      if W.all'Terminated then
         Free (W);  -- UndCC_Valid
      end if;
   end Free_With_Check;

   procedure Free_Non_Task (N : in out Non_Task_Access) is
   begin
      Free_Int (N);  -- UndCC_Valid
   end Free_Non_Task;

end Test_6_3_2;
