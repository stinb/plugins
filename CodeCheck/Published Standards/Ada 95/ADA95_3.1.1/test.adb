procedure Test is
   MilesPerHour : Integer := 60;     -- UndCC_Violation
   entryvalue   : Integer := 100;    -- UndCC_FalseNeg
   jsonDataSize : Integer := 256;    -- UndCC_Violation
   SpeedValue   : Integer := 80;     -- UndCC_Violation
   Data_JSON    : String (1 .. 100) := ""; -- UndCC_Valid
begin
   null;
end Test;
