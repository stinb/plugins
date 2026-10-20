package Test_7_2_7 is

   type Temperature is digits 6 range -50.0 .. 200.0;
   type Ratio is digits 6;
   type Price is delta 0.01 range 0.0 .. 1_000.0;
   type Derived_Temp is new Temperature;
   type Count is range 0 .. 100;
   type Name_String is new String (1 .. 20);

   function Is_Freezing (T : Temperature) return Boolean;

   function Is_Zero (C : Count) return Boolean;

end Test_7_2_7;
