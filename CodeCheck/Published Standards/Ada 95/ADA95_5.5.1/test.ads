package Test_5_5_1 is

   type Temperature is range -50 .. 150;
   type Color is (Red, Green, Blue);
   type Index is range 1 .. 100;

   -- Violation: T'First .. T'Last in subtype constraint
   subtype Temp_Full is Temperature range Temperature'First .. Temperature'Last; -- UndCC_Violation

   -- Valid: using 'Range
   subtype Temp_Range is Temperature range Temperature'Range; -- UndCC_Valid

   -- Valid: using type name directly
   subtype Temp_All is Temperature; -- UndCC_Valid

   -- Valid: only 'First (not the full pattern)
   subtype Temp_Pos is Temperature range 0 .. Temperature'Last; -- UndCC_Valid

   -- Valid: only 'Last (not the full pattern)
   subtype Temp_Neg is Temperature range Temperature'First .. 0; -- UndCC_Valid

   -- Violation: array index using T'First .. T'Last
   type Temp_Array is array (Temperature'First .. Temperature'Last) of Integer; -- UndCC_Violation

   -- Valid: array index using 'Range
   type Temp_Array_Good is array (Temperature'Range) of Integer; -- UndCC_Valid

   -- Valid: array index using type name
   type Color_Array is array (Color) of Integer; -- UndCC_Valid

   -- Violation: enumeration type
   subtype Color_Full is Color range Color'First .. Color'Last; -- UndCC_Violation

   -- Valid: using 'Range for enum
   subtype Color_Good is Color range Color'Range; -- UndCC_Valid

   -- Violation: attribute names in different case (Ada is case-insensitive)
   subtype Index_Full is Index range Index'FIRST .. Index'LAST; -- UndCC_Violation

   -- Valid: different type names on each side
   type Mixed_Array is array (Color'First .. Index'Last) of Boolean; -- UndCC_Valid

   -- Violation: built-in Integer type (the "huge range" caution from the guideline)
   subtype Big_Int is Integer range Integer'First .. Integer'Last; -- UndCC_Violation

   -- Valid: qualified expression uses apostrophe differently (not an attribute access)
   Sixty : constant Temperature := Temperature'(60); -- UndCC_Valid

end Test_5_5_1;
