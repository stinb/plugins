with Ada.Exceptions;
with Ada.Text_IO;
use Ada.Exceptions;

package body Test_5_8_2 is

   procedure Logged_Catch is
   begin
      raise Constraint_Error;
   exception
      when others => -- UndCC_Valid
         Ada.Text_IO.Put_Line
           (Ada.Exceptions.Exception_Information
              (Ada.Exceptions.Current_Exception));
   end Logged_Catch;

   procedure Silent_Catch is
   begin
      raise Constraint_Error;
   exception
      when others => -- UndCC_Violation
         null;
   end Silent_Catch;

   procedure Named_Handler_Catch is
   begin
      raise Constraint_Error;
   exception
      when E : others => -- UndCC_Valid
         Ada.Text_IO.Put_Line (Ada.Exceptions.Exception_Information (E));
   end Named_Handler_Catch;

   procedure Unrelated_Call_Then_Silent is
   begin
      Ada.Text_IO.Put_Line
        (Ada.Exceptions.Exception_Information
           (Ada.Exceptions.Current_Exception));
      raise Constraint_Error;
   exception
      when others => -- UndCC_Violation
         null;
   end Unrelated_Call_Then_Silent;

   procedure Logged_With_Message is
   begin
      raise Constraint_Error;
   exception
      when E : others => -- UndCC_Valid
         Ada.Text_IO.Put_Line (Ada.Exceptions.Exception_Message (E));
   end Logged_With_Message;

   procedure Logged_With_Name is
   begin
      raise Constraint_Error;
   exception
      when E : others => -- UndCC_Valid
         Ada.Text_IO.Put_Line (Ada.Exceptions.Exception_Name (E));
   end Logged_With_Name;

   procedure No_Others_Handler is
   begin
      raise Constraint_Error;
   exception
      when Constraint_Error => -- UndCC_Valid
         null;
   end No_Others_Handler;

   procedure Logs_Unrelated_Message is
   begin
      raise Constraint_Error;
   exception
      when others => -- UndCC_Violation
         Ada.Text_IO.Put_Line ("something went wrong");
   end Logs_Unrelated_Message;

   package body Edge_Cases is

      procedure Log (S : String) is
      begin
         Ada.Text_IO.Put_Line (S);
      end Log;

      procedure Upper_Case_Others is
      begin
         raise Constraint_Error;
      exception
         WHEN OTHERS => -- UndCC_Violation
            null;
      end Upper_Case_Others;

      procedure Nested_Then_Outer_Others is
      begin
         begin
            raise Constraint_Error;
         exception
            when others => -- UndCC_Valid
               Ada.Text_IO.Put_Line
                 (Ada.Exceptions.Exception_Information
                    (Ada.Exceptions.Current_Exception));
         end;
         raise Program_Error;
      exception
         when others => -- UndCC_Violation
            null;
      end Nested_Then_Outer_Others;

      procedure Many_Named_Then_Others is
      begin
         raise Constraint_Error;
      exception
         when Constraint_Error => null;
         when Program_Error => null;
         when others => -- UndCC_Violation
            null;
      end Many_Named_Then_Others;

      procedure Reraise_Only is
      begin
         raise Constraint_Error;
      exception
         when others => -- UndCC_Violation
            raise;
      end Reraise_Only;

      procedure Logged_Via_Use_Clause is
      begin
         raise Constraint_Error;
      exception
         when others => -- UndCC_Valid
            Ada.Text_IO.Put_Line (Exception_Information (Current_Exception));
      end Logged_Via_Use_Clause;

      procedure Logged_Indirectly is
      begin
         raise Constraint_Error;
      exception
         when others => -- UndCC_Valid
            Log (Ada.Exceptions.Exception_Information
                   (Ada.Exceptions.Current_Exception));
      end Logged_Indirectly;

      procedure Logged_Inside_Nested_If is
         Flag : Boolean := True;
      begin
         raise Constraint_Error;
      exception
         when others => -- UndCC_Valid
            if Flag then
               Ada.Text_IO.Put_Line
                 (Ada.Exceptions.Exception_Information
                    (Ada.Exceptions.Current_Exception));
            end if;
      end Logged_Inside_Nested_If;

   end Edge_Cases;

end Test_5_8_2;
