pragma Suppress (All_Checks);  -- UndCC_Violation

package Test is

   pragma Suppress (Range_Check);  -- UndCC_Violation

   procedure Do_Something (Arr : in out String; Idx : Integer);

end Test;
