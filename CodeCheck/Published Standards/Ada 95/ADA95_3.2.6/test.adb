with Ada.Numerics;

procedure Test_3_2_6 is

   Radius : Float := 5.0;
   Area   : Float;
   X      : Integer;

begin

   -- Valid: using Ada.Numerics.Pi
   Area := Ada.Numerics.Pi * Radius ** 2;       -- UndCC_Valid

   -- Valid: using Ada.Numerics.e
   Area := Ada.Numerics.e * Radius;             -- UndCC_Valid

   -- Violation: Pi approximation
   Area := 3.14159 * Radius ** 2;               -- UndCC_Violation

   -- Violation: e approximation
   X := Integer (2.71828 * Float (X));          -- UndCC_Violation

end Test_3_2_6;
