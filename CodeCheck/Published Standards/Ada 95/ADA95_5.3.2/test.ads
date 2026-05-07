package Test_5_3_2 is

   type Buffer_Index is range 1 .. 80;
   type Buffer is array (Buffer_Index) of Character;

   -- Valid: named array type used for variable
   Named_Line : Buffer;                                                     -- UndCC_Valid

   -- Violation: anonymous array type variable
   Input_Line : array (Buffer_Index) of Character;                         -- UndCC_Violation

   -- Violation: anonymous array with range literal
   Raw_Data : array (1 .. 256) of Integer;                                 -- UndCC_Violation

   -- Valid: named type for a two-dimensional array
   type Matrix_Index is range 1 .. 10;
   type Matrix is array (Matrix_Index, Matrix_Index) of Float;
   Grid : Matrix;                                                           -- UndCC_Valid

   -- Violation: anonymous two-dimensional array
   Table : array (1 .. 10, 1 .. 10) of Float;                              -- UndCC_Violation

   -- Violation: constant with anonymous array type
   Const_Data : constant array (1 .. 5) of Integer := (1, 2, 3, 4, 5);   -- UndCC_Violation

   -- Violation: aliased anonymous array
   Aliased_Data : aliased array (1 .. 5) of Integer;                      -- UndCC_Violation

   -- Violation: aliased constant anonymous array
   Aliased_Const : aliased constant array (1 .. 5) of Integer := (1, 2, 3, 4, 5); -- UndCC_Violation

   -- Valid: constant with named array type
   type Int_Array is array (1 .. 5) of Integer;
   Const_Named : constant Int_Array := (1, 2, 3, 4, 5);                   -- UndCC_Valid

   -- Valid: named type whose name starts with "array" — must not false-positive
   type Array_Buffer is array (1 .. 10) of Character;
   Named_Buffer : Array_Buffer;                                            -- UndCC_Valid

   -- Violation: aliased constant with multi-dimensional anonymous array
   Multi_Dim : aliased constant array (1 .. 3, 1 .. 3) of Float :=        -- UndCC_Violation
      (others => (others => 0.0));

   -- Violation: anonymous array indexed by enumeration
   type Color is (Red, Green, Blue);
   Color_Table : array (Color) of Integer;                                  -- UndCC_Violation

   -- Exception: unique table (periodic table pattern) — no named type needed
   type Element is (H, He, Li);
   Periodic_Table : array (1 .. 3) of Element;                            -- UndCC_FalsePos

   -- Exception: another unique lookup table (days abbreviations)
   type Day is (Mon, Tue, Wed, Thu, Fri, Sat, Sun);
   Day_Abbrev : array (Day) of Character;                                  -- UndCC_FalsePos

private

   -- Valid: named array type in private section
   type Private_Array is array (1 .. 8) of Boolean;
   Private_Named : Private_Array;                                           -- UndCC_Valid

   -- Violation: anonymous array in private section
   Private_Anon : array (1 .. 8) of Boolean;                               -- UndCC_Violation

end Test_5_3_2;
