with Ada.Command_Line;

package body Test_7_1_4_Pair is

   procedure P1 is
      N : Natural := Ada.Command_Line.Argument_Count;  -- UndCC_Violation
   begin
      null;
   end P1;

   procedure P2 is
      S : String := Ada.Command_Line.Argument (1);  -- UndCC_Violation
   begin
      null;
   end P2;

end Test_7_1_4_Pair;
