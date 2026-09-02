package body Test is

   task body No_Handler is
      Condition_1, Condition_2 : Boolean := False;
   begin
      loop
         select  -- UndCC_Violation
            when Condition_1 =>
               accept Entry_1;
         or
            when Condition_2 =>
               accept Entry_2;
         end select;
      end loop;
   end No_Handler;

   task body Has_Program_Error_Handler is
      Condition_1, Condition_2 : Boolean := False;
   begin
      loop
         select  -- UndCC_Valid
            when Condition_1 =>
               accept Entry_1;
         or
            when Condition_2 =>
               accept Entry_2;
         end select;
      end loop;
   exception
      when Program_Error =>
         null;
   end Has_Program_Error_Handler;

   task body Has_Others_Handler is
      Condition_1, Condition_2 : Boolean := False;
   begin
      loop
         select  -- UndCC_Valid
            when Condition_1 =>
               accept Entry_1;
         or
            when Condition_2 =>
               accept Entry_2;
         end select;
      end loop;
   exception
      when others =>
         null;
   end Has_Others_Handler;

   task body Has_Else_No_Handler is
      Condition_1, Condition_2 : Boolean := False;
   begin
      loop
         select  -- UndCC_Valid
            when Condition_1 =>
               accept Entry_1;
         or
            when Condition_2 =>
               accept Entry_2;
         else
            null;
         end select;
      end loop;
   end Has_Else_No_Handler;

   task body Has_Unguarded_Alternative is
      Condition_1 : Boolean := False;
   begin
      loop
         select  -- UndCC_Valid
            when Condition_1 =>
               accept Entry_1;
         or
            accept Entry_2;
         end select;
      end loop;
   end Has_Unguarded_Alternative;

   task body Timed_Entry_Call is
   begin
      select  -- UndCC_Valid
         Target.Entry_1;
      or
         delay 1.0;
      end select;
   end Timed_Entry_Call;

   task body Tricky_Nested is
      Condition_1, Condition_2 : Boolean := False;
   begin
      loop
         select  -- UndCC_Violation
            when Condition_1 or Condition_2 =>
               accept Entry_1 do
                  if Condition_1 then
                     null;
                  else
                     null;
                  end if;
               end Entry_1;
         or
            when Condition_2 =>
               accept Entry_2 do
                  Guarded:
                  begin
                     select  -- UndCC_Valid
                        accept Entry_1;
                     or
                        delay 1.0;
                     end select;
                  exception
                     when Constraint_Error =>
                        null;
                  end Guarded;
               end Entry_2;
         end select;
      end loop;
   end Tricky_Nested;

   task body Async_Select is
      Condition_1 : Boolean := False;
   begin
      select  -- UndCC_Valid
         delay 1.0;
      then abort
         loop
            case Condition_1 is
               when True =>
                  null;
               when False =>
                  null;
            end case;
         end loop;
      end select;
   end Async_Select;

end Test;
