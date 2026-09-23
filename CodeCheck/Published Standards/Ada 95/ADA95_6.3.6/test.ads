with Ada.Command_Line;

package Test_6_3_6 is

   task type Racy_A is
   end Racy_A;

   task type Racy_B is
   end Racy_B;

   task type Racy_C is
   end Racy_C;

   RA : Racy_A;
   RB : Racy_B;
   RC : Racy_C;

   procedure Set_Exit_Status (Code : Integer);

   procedure Renamed_Exit_Status (Code : Ada.Command_Line.Exit_Status)
     renames Ada.Command_Line.Set_Exit_Status;

   procedure Call_Local;
   procedure Call_Renamed;

end Test_6_3_6;
