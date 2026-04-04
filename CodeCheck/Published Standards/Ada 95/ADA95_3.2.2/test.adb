procedure Test_3_2_2 is

   -- Valid: singular general nouns and meaningful suffixes
   type Day          is (Monday, Tuesday, Wednesday, Thursday, Friday, -- UndCC_Valid
                         Saturday, Sunday);
   type Day_Of_Month    is range      0 ..    31;                      -- UndCC_Valid
   type Month_Number    is range      1 ..    12;                      -- UndCC_Valid
   type Historical_Year is range -6_000 .. 2_500;                     -- UndCC_Valid
   type Sector_Range    is range  1 ..     4;                          -- UndCC_Valid
   type Track_Map       is array (Sector_Range) of Integer;            -- UndCC_Valid
   type Status          is (Active, Inactive);                         -- UndCC_Valid

   -- Violation: _Type / _Types suffix
   type Day_Type        is (Monday, Tuesday);                          -- UndCC_Violation
   type Color_Types     is (Red, Green, Blue);                         -- UndCC_Violation

   -- Violation: _Record / _Rec suffix
   type Employee_Record is range 0 .. 100;                             -- UndCC_Violation
   type Node_Rec        is range 0 .. 100;                             -- UndCC_Violation

   -- Violation: _Array / _Arr suffix
   type Buffer_Array    is array (1 .. 10) of Integer;                 -- UndCC_Violation
   type Data_Arr        is array (1 .. 10) of Integer;                 -- UndCC_Violation

   -- Violation: _Ptr / _Pointer suffix
   type Node_Ptr        is range 0 .. 100;                             -- UndCC_Violation
   type List_Pointer    is range 0 .. 100;                             -- UndCC_Violation

   -- Violation: _Enum / _Enumeration suffix
   type Color_Enum      is (Red, Green, Blue);                         -- UndCC_Violation
   type Day_Enumeration is (Mon, Tue, Wed);                            -- UndCC_Violation

   -- Violation: _Int / _Integer / _Float / _Bool / _Boolean suffix
   type Count_Int       is range 0 .. 100;                             -- UndCC_Violation
   type Size_Integer    is range 0 .. 100;                             -- UndCC_Violation
   type Rate_Float      is digits 6;                                   -- UndCC_Violation
   type Flag_Bool       is (True, False);                              -- UndCC_Violation
   type Switch_Boolean  is (On, Off);                                  -- UndCC_Violation

   -- Violation: shadows predefined Ada type name
   type Integer         is range 0 .. 100;                             -- UndCC_Violation
   type Boolean         is (Yes, No);                                  -- UndCC_Violation
   type Float           is digits 6;                                   -- UndCC_Violation
   type Address         is range 0 .. 16#FFFF#;                        -- UndCC_Violation

   -- Valid subtypes: singular general nouns
   subtype Weekday      is Day range Monday .. Friday;                 -- UndCC_Valid
   subtype Small_Count  is Standard.Integer range 0 .. 255;           -- UndCC_Valid
   subtype Sector_Index is Standard.Integer range 1 .. 4;             -- UndCC_Valid

   -- Violation: _Type / _Types suffix on subtype
   subtype Day_Type     is Day range Monday .. Friday;                 -- UndCC_Violation
   subtype Color_Types  is Standard.Integer range 0 .. 10;            -- UndCC_Violation

   -- Violation: _Array / _Arr suffix on subtype
   subtype Buffer_Array is Standard.Integer range 0 .. 100;           -- UndCC_Violation
   subtype Data_Arr     is Standard.Integer range 0 .. 100;           -- UndCC_Violation

   -- Violation: _Int / _Integer suffix on subtype
   subtype Count_Int    is Standard.Integer range 0 .. 100;           -- UndCC_Violation
   subtype Size_Integer is Standard.Integer range 0 .. 100;           -- UndCC_Violation

   -- Violation: shadows predefined Ada type name
   subtype Integer      is Standard.Integer range 0 .. 100;           -- UndCC_Violation
   subtype Boolean      is Standard.Boolean;                          -- UndCC_Violation

begin
   null;
end Test_3_2_2;
