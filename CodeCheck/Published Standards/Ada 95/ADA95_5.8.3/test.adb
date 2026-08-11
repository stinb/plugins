with Ada.Text_IO;

package body Test_5_8_3 is

   procedure Bare_Reraise is
   begin
      raise My_Error;
   exception
      when My_Error => -- UndCC_Violation
         raise;
   end Bare_Reraise;

   procedure Named_Bare_Reraise is
   begin
      raise My_Error;
   exception
      when E : My_Error => -- UndCC_Violation
         raise;
   end Named_Bare_Reraise;

   procedure Combined_Choice_Reraise is
   begin
      raise My_Error;
   exception
      when My_Error | Constraint_Error => -- UndCC_Violation
         raise;
   end Combined_Choice_Reraise;

   procedure Reraise_With_Cleanup is
   begin
      raise My_Error;
   exception
      when My_Error => -- UndCC_Valid
         Ada.Text_IO.Put_Line ("cleaning up");
         raise;
   end Reraise_With_Cleanup;

   procedure Translate_Exception is
   begin
      raise My_Error;
   exception
      when My_Error => -- UndCC_Valid
         raise Constraint_Error;
   end Translate_Exception;

   procedure Null_Then_Reraise is
   begin
      raise My_Error;
   exception
      when My_Error => -- UndCC_Violation
         null;
         raise;
   end Null_Then_Reraise;

   procedure Silent_Null_Only is
   begin
      raise My_Error;
   exception
      when My_Error => -- UndCC_Valid
         null;
   end Silent_Null_Only;

   procedure Real_Recovery is
   begin
      raise My_Error;
   exception
      when My_Error => -- UndCC_Valid
         Ada.Text_IO.Put_Line ("recovered, continuing");
   end Real_Recovery;

   package body Edge_Cases is

      procedure Mixed_Branch_With_Call is
         Flag : Boolean := True;
      begin
         raise My_Error;
      exception
         when My_Error => -- UndCC_Valid
            if Flag then
               Ada.Text_IO.Put_Line ("logging");
            else
               raise;
            end if;
      end Mixed_Branch_With_Call;

      procedure Conditional_Bare_Reraise_No_Else is
         Flag : Boolean := True;
      begin
         raise My_Error;
      exception
         when My_Error => -- UndCC_Violation
            if Flag then
               raise;
            end if;
      end Conditional_Bare_Reraise_No_Else;

      procedure Explicit_Same_Name_Reraise is
      begin
         raise My_Error;
      exception
         when My_Error => -- UndCC_Violation
            raise My_Error;
      end Explicit_Same_Name_Reraise;

      procedure Renamed_Exception_Reraise is
      begin
         raise My_Error;
      exception
         when Sub_Error => -- UndCC_Violation
            raise;
      end Renamed_Exception_Reraise;

      procedure Multiple_Handlers_Mixed is
      begin
         begin
            raise My_Error;
         exception
            when My_Error => -- UndCC_Violation
               raise;
         end;
         raise Constraint_Error;
      exception
         when Constraint_Error => -- UndCC_Valid
            Ada.Text_IO.Put_Line ("handled for real");
      end Multiple_Handlers_Mixed;

      function Func_Bare_Reraise return Integer is
      begin
         raise My_Error;
      exception
         when My_Error => -- UndCC_Violation
            raise;
      end Func_Bare_Reraise;

      procedure Uppercase_Raise is
      begin
         RAISE My_Error;
      exception
         WHEN My_Error => -- UndCC_Violation
            RAISE;
      end Uppercase_Raise;

      task body Dummy_Task_Type is
      begin
         null;
      end Dummy_Task_Type;

      Dummy_Task : Dummy_Task_Type;

      procedure Assignment_Cleanup_Then_Reraise is
         Flag : Boolean := True;
      begin
         raise My_Error;
      exception
         when My_Error => -- UndCC_Valid
            Flag := False;
            raise;
      end Assignment_Cleanup_Then_Reraise;

      procedure Abort_Task_Then_Reraise is
      begin
         raise My_Error;
      exception
         when My_Error => -- UndCC_Valid
            abort Dummy_Task;
            raise;
      end Abort_Task_Then_Reraise;

      procedure Message_Same_Name_Reraise is
      begin
         raise My_Error;
      exception
         when My_Error => -- UndCC_Valid
            raise My_Error with "additional context";
      end Message_Same_Name_Reraise;

   end Edge_Cases;

end Test_5_8_3;
