with Ada.Finalization;
package Test_5_3_3 is

   -- Valid: private and limited private types
   type Private_Record is private;                                                -- UndCC_Valid
   type Limited_Record is limited private;                                        -- UndCC_Valid
   type Controlled_Type is new Ada.Finalization.Controlled with private;         -- UndCC_Valid

   -- Valid: abstract tagged private (Private Type, filtered by ~Private)
   type Abstract_Base is abstract tagged private;                                 -- UndCC_Valid

   -- Valid: scalar and enumeration types (commonly non-private by design)
   type Color is (Red, Green, Blue);                                              -- UndCC_Valid
   type Count is range 0 .. 100;                                                  -- UndCC_Valid
   type Ratio is digits 6 range 0.0 .. 1.0;                                      -- UndCC_Valid

   -- Violations: composite types with exposed structure in the visible part
   type Exposed_Record is record                                                   -- UndCC_Violation
      X : Integer;
      Y : Integer;
   end record;

   type Exposed_Array is array (1 .. 10) of Integer;                             -- UndCC_Violation

   type Exposed_Tagged is tagged record                                            -- UndCC_Violation
      Value : Integer;
   end record;

   -- Violation: tagged type with a visible record extension (not 'with private')
   type Visible_Extension is new Ada.Finalization.Controlled with record         -- UndCC_Violation
      Count : Integer;
   end record;

   -- Violation: record with discriminant — structure is still exposed
   type Discriminant_Rec (N : Positive) is record                                -- UndCC_Violation
      Data : Integer;
   end record;

   -- Violation: unconstrained array type
   type Matrix is array (Integer range <>, Integer range <>) of Float;           -- UndCC_Violation

   -- Violation: record derived from a Controlled extension — inherits private
   -- components from Controlled, giving it an implicitly private entity kind
   type Derived_Controlled_Record is new Visible_Extension with record           -- UndCC_Violation
      Extra : Float;
   end record;

   -- Violation: plain record derived from another exposed record — may also
   -- receive an implicitly private entity kind in Understand
   type Derived_Plain_Record is new Exposed_Record;                              -- UndCC_Violation

   -- Violation: array type derived from another array type
   type Derived_Plain_Array is new Exposed_Array;                                -- UndCC_Violation

   -- Violation: record with default field values — structure still exposed
   type Point is record                                                           -- UndCC_Violation
      X : Float := 0.0;
      Y : Float := 0.0;
   end record;

   -- Violation: array indexed by an enumeration type
   type Color_Map is array (Color) of Integer;                                   -- UndCC_Violation

   -- Violation: tagged null extension in the visible part
   type Null_Extension is new Exposed_Tagged with null record;                   -- UndCC_Violation

   -- Valid: access type — not a composite type, not checked
   type Ptr_To_Record is access Exposed_Record;                                  -- UndCC_Valid

   -- Valid: subtype — not a type declaration
   subtype Short_Count is Count;                                                 -- UndCC_Valid

   procedure Run_Tests;

private

   -- These are in the private section — must not be flagged
   type Private_Record is record
      Data : Integer;
   end record;

   type Limited_Record is record
      Data : Integer;
   end record;

   type Abstract_Base is abstract tagged record
      Data : Integer;
   end record;

   type Controlled_Type is new Ada.Finalization.Controlled with record
      Data : Integer;
   end record;

   type Helper_Array is array (1 .. 4) of Integer;

end Test_5_3_3;
