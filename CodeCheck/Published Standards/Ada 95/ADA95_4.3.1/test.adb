package body Test_4_3_1 is

   My_Error : exception;

   procedure Raise_User_Defined is
   begin
      raise My_Error;           -- UndCC_Valid
   end Raise_User_Defined;

   procedure Raise_Constraint_Error is
   begin
      raise Constraint_Error;   -- UndCC_Violation
   end Raise_Constraint_Error;

   procedure Raise_Program_Error is
   begin
      raise Program_Error;      -- UndCC_Violation
   end Raise_Program_Error;

   procedure Raise_Storage_Error is
   begin
      raise Storage_Error;      -- UndCC_Violation
   end Raise_Storage_Error;

   procedure Raise_Tasking_Error is
   begin
      raise Tasking_Error;      -- UndCC_Violation
   end Raise_Tasking_Error;

end Test_4_3_1;
