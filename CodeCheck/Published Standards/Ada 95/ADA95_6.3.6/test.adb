package body Test_6_3_6 is

   package CL renames Ada.Command_Line;

   task body Racy_C is
   begin
      CL.Set_Exit_Status (CL.Failure);  -- UndCC_Violation
   end Racy_C;

   task body Racy_A is
   begin
      Ada.Command_Line.Set_Exit_Status (Ada.Command_Line.Failure);  -- UndCC_Violation
   end Racy_A;

   task body Racy_B is
   begin
      Ada.Command_Line.Set_Exit_Status (Ada.Command_Line.Success);  -- UndCC_Violation
   end Racy_B;

   procedure Set_Exit_Status (Code : Integer) is
   begin
      null;
   end Set_Exit_Status;

   procedure Call_Local is
   begin
      Set_Exit_Status (0);  -- UndCC_Valid
   end Call_Local;

   procedure Call_Renamed is
   begin
      Renamed_Exit_Status (Ada.Command_Line.Success);  -- UndCC_Violation
   end Call_Renamed;

end Test_6_3_6;
