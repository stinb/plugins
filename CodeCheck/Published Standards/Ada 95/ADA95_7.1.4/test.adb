package body Test_7_1_4 is

   package CL renames Ada.Command_Line;

   procedure Proc_A is
      N : Natural := Ada.Command_Line.Argument_Count;  -- UndCC_Violation
      S : String := Ada.Command_Line.Argument (2);     -- UndCC_Violation

      procedure Nested is
         M : Natural := CL.Argument_Count;  -- UndCC_Violation
      begin
         null;
      end Nested;
   begin
      Nested;
   end Proc_A;

   procedure Proc_C is
      Nm : String := CL.Command_Name;  -- UndCC_Violation
   begin
      null;
   end Proc_C;

   procedure Proc_B is
      S : String := Ada.Command_Line.Argument (1);  -- UndCC_Violation
   begin
      null;
   end Proc_B;

   function Argument_Count return Natural is
   begin
      return 0;
   end Argument_Count;

   procedure Uses_Local is
      N : Natural := Argument_Count;  -- UndCC_Valid
   begin
      null;
   end Uses_Local;

end Test_7_1_4;
