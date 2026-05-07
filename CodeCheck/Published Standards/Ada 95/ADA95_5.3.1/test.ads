with Ada.Finalization;
package Test_5_3_1 is

   type Number  is range 0 .. 1_000_000;
   type Columns is range 0 .. 200;
   type Score   is digits 6 range 0.0 .. 100.0;
   type Color   is (Red, Green, Blue);

   -- Valid: subtypes with range constraints (guideline 2)
   subtype Listing_Paper is Columns range 1 .. 132;                           -- UndCC_Valid
   subtype Dumb_Terminal is Columns range 1 .. 80;                            -- UndCC_Valid
   subtype Small_Number  is Number  range 0 .. 100;                           -- UndCC_Valid
   subtype High_Score    is Score   range 75.0 .. 100.0;                      -- UndCC_Valid
   subtype Warm_Color    is Color   range Green .. Blue;                      -- UndCC_Valid

   -- Violation: scalar subtypes with no range constraint (guideline 2)
   subtype Unconstrained_Columns is Columns;                                  -- UndCC_Violation
   subtype Unconstrained_Number  is Number;                                   -- UndCC_Violation
   subtype Bare_Integer          is Integer;                                  -- UndCC_Violation
   subtype Bare_Float            is Float;                                    -- UndCC_Violation
   subtype Bare_Color            is Color;                                    -- UndCC_Violation

   -- Violation: subtype of a subtype with no further constraint (guideline 2)
   subtype Doubly_Unconstrained is Small_Number;                              -- UndCC_Violation

   -- Valid: non-scalar subtype — array index constraint is not a range constraint
   type Line is array (Columns range <>) of Character;

   subtype Listing_Line is Line (Listing_Paper);                              -- UndCC_Valid

   -- Valid: simple type derivation with no extension (guideline 5)
   type Dollars is new Number;                                                -- UndCC_Valid
   type Cents   is new Number;                                                -- UndCC_Valid

   -- Valid: type extension that adds meaningful components (guideline 5)
   type Base_Record is tagged record
      Id : Integer;
   end record;

   type Extended_Record is new Base_Record with record                        -- UndCC_Valid
      Name : Integer;
   end record;

   -- Violation: non-abstract extension with null record — use plain derivation (guideline 5)
   type Null_Extension is new Base_Record with null record;                   -- UndCC_Violation

   -- Valid: abstract null extension is a legitimate OOP pattern
   type Abstract_Extension is abstract new Base_Record with null record;      -- UndCC_Valid

   -- Valid: deriving from abstract type (Controlled) with null record is intentional
   type My_Controlled is                                                      -- UndCC_Valid
      new Ada.Finalization.Controlled with null record;

   -- Valid: same pattern for Limited_Controlled
   type My_Limited_Controlled is                                              -- UndCC_Valid
      new Ada.Finalization.Limited_Controlled with null record;

   -- Violation: null record extension of a second-level derived type
   type Derived_Record is new Extended_Record with null record;               -- UndCC_Violation

   -- Violation: subtypes of other predefined scalars with no constraint (guideline 2)
   subtype Bare_Boolean   is Boolean;                                         -- UndCC_Violation
   subtype Bare_Duration  is Duration;                                        -- UndCC_Violation
   subtype Bare_Natural   is Natural;                                         -- UndCC_Violation
   subtype Bare_Character is Character;                                       -- UndCC_Violation

   type Byte is mod 256;
   subtype Bare_Byte    is Byte;                                              -- UndCC_Violation
   subtype Half_Byte    is Byte range 0 .. 127;                              -- UndCC_Valid

   type Mode is range 0 .. 10;
   subtype Bare_Mode is Mode;                                                 -- UndCC_Violation

end Test_5_3_1;
