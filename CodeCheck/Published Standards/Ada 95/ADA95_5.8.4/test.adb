with Ada.Text_IO;
with Ada.Exceptions;

package body Test_5_8_4 is

   procedure Translate_No_Info is
   begin
      raise Constraint_Error;
   exception
      when Constraint_Error => -- UndCC_Violation
         raise Custom_Error;
   end Translate_No_Info;

   procedure Translate_Program_Error_No_Info is
   begin
      raise Program_Error;
   exception
      when Program_Error => -- UndCC_Violation
         raise Custom_Error;
   end Translate_Program_Error_No_Info;

   procedure Translate_With_Message_Info is
   begin
      raise Constraint_Error;
   exception
      when E : Constraint_Error => -- UndCC_Valid
         raise Custom_Error with Ada.Exceptions.Exception_Message (E);
   end Translate_With_Message_Info;

   procedure Translate_With_Logged_Info is
   begin
      raise Constraint_Error;
   exception
      when E : Constraint_Error => -- UndCC_Valid
         Ada.Text_IO.Put_Line (Ada.Exceptions.Exception_Information (E));
         raise Custom_Error;
   end Translate_With_Logged_Info;

   procedure Translate_User_Defined_Source is
   begin
      raise My_Error;
   exception
      when My_Error => -- UndCC_Valid
         raise Custom_Error;
   end Translate_User_Defined_Source;

   procedure No_Translate_Just_Log is
   begin
      raise Constraint_Error;
   exception
      when E : Constraint_Error => -- UndCC_Valid
         Ada.Text_IO.Put_Line (Ada.Exceptions.Exception_Information (E));
   end No_Translate_Just_Log;

   procedure Reraise_Same_Predefined_No_Info is
   begin
      raise Constraint_Error;
   exception
      when Constraint_Error => -- UndCC_Valid
         raise;
   end Reraise_Same_Predefined_No_Info;

   package body Edge_Cases is

      procedure Uppercase_Choice is
      begin
         raise CONSTRAINT_ERROR;
      exception
         WHEN CONSTRAINT_ERROR => -- UndCC_Violation
            RAISE Custom_Error;
      end Uppercase_Choice;

      procedure Mixed_Predefined_First is
      begin
         raise Constraint_Error;
      exception
         when Constraint_Error | My_Error => -- UndCC_Violation
            raise Custom_Error;
      end Mixed_Predefined_First;

      procedure Mixed_User_Defined_First is
      begin
         raise My_Error;
      exception
         when My_Error | Constraint_Error => -- UndCC_Valid
            raise Custom_Error;
      end Mixed_User_Defined_First;

      procedure Save_Occurrence_Used is
         Saved : Ada.Exceptions.Exception_Occurrence;
      begin
         raise Constraint_Error;
      exception
         when E : Constraint_Error => -- UndCC_Valid
            Ada.Exceptions.Save_Occurrence (Saved, E);
            raise Custom_Error;
      end Save_Occurrence_Used;

      procedure Predefined_To_Predefined_No_Info is
      begin
         raise Constraint_Error;
      exception
         when Constraint_Error => -- UndCC_Violation
            raise Program_Error;
      end Predefined_To_Predefined_No_Info;

      procedure Renamed_Predefined_Source is
      begin
         raise Constraint_Error;
      exception
         when Sub_CE => -- UndCC_Violation
            raise Custom_Error;
      end Renamed_Predefined_Source;

      procedure Multiple_Handlers_Mixed is
      begin
         begin
            raise Constraint_Error;
         exception
            when Constraint_Error => -- UndCC_Violation
               raise Custom_Error;
         end;
         raise Program_Error;
      exception
         when E : Program_Error => -- UndCC_Valid
            Ada.Text_IO.Put_Line (Ada.Exceptions.Exception_Information (E));
            raise Custom_Error;
      end Multiple_Handlers_Mixed;

      function Func_Translate_No_Info return Integer is
      begin
         raise Constraint_Error;
      exception
         when Constraint_Error => -- UndCC_Violation
            raise Custom_Error;
      end Func_Translate_No_Info;

   end Edge_Cases;

end Test_5_8_4;
