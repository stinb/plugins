package Test_5_4_1 is

   type Number_List is array (Integer range <>) of Integer;

   -- Violation: discriminant typed with unconstrained Natural
   type Number_Holder_1 (Current_Length : Natural := 0) is      -- UndCC_Violation
      record
         Numbers : Number_List (1 .. Current_Length);
      end record;

   -- Valid: discriminant typed with constrained subtype
   subtype Max_Numbers is Natural range 0 .. 42;
   type Number_Holder_2 (Current_Length : Max_Numbers := 0) is  -- UndCC_Valid
      record
         Numbers : Number_List (1 .. Current_Length);
      end record;

   -- Violation: discriminant typed with unconstrained Integer
   type Holder_3 (Size : Integer := 0) is                       -- UndCC_Violation
      record
         Data : Number_List (1 .. Size);
      end record;

   -- Valid: discriminant typed with constrained subtype of Integer
   subtype Small_Size is Integer range 1 .. 100;
   type Holder_4 (Size : Small_Size := 1) is                    -- UndCC_Valid
      record
         Data : Number_List (1 .. Size);
      end record;

   -- Violation: discriminant typed with Positive (predefined, unconstrained)
   type Holder_5 (Count : Positive := 1) is                     -- UndCC_Violation
      record
         Data : Number_List (1 .. Count);
      end record;

   -- Valid: discriminant typed with enumeration (already constrained by values)
   type Color is (Red, Green, Blue);
   type Color_Record (C : Color := Red) is                      -- UndCC_Valid
      record
         Value : Integer;
      end record;

   -- Valid: discriminant typed with a Float subtype with digits constraint
   subtype Precise_Float is Float digits 6;
   type Float_Record (F : Precise_Float := 0.0) is              -- UndCC_Valid
      record
         Value : Integer;
      end record;

   -- Violation: discriminant typed with unconstrained Float
   type Float_Holder (F : Float := 0.0) is                      -- UndCC_Violation
      record
         Value : Integer;
      end record;

   -- Violation: discriminant typed with Long_Integer (predefined, unconstrained)
   type Long_Holder (N : Long_Integer := 0) is                  -- UndCC_Violation
      record
         Value : Integer;
      end record;

   -- Violation: discriminant typed with Duration (predefined, unconstrained)
   type Timed_Record (T : Duration := 0.0) is                   -- UndCC_Violation
      record
         Value : Integer;
      end record;

   -- Valid: discriminant typed with Boolean (predefined enumeration)
   type Bool_Record (Flag : Boolean := False) is                -- UndCC_Valid
      record
         Value : Integer;
      end record;

   -- Valid: discriminant typed with Character (predefined enumeration)
   type Char_Record (Ch : Character := ' ') is                  -- UndCC_Valid
      record
         Value : Integer;
      end record;

   -- Valid: named constrained subtype of Integer used as discriminant
   subtype Tiny_Int is Integer range 1 .. 10;
   type Int_Constrained (D : Tiny_Int := 1) is                 -- UndCC_Valid
      record
         Value : Integer;
      end record;

   -- Valid: fixed-point subtype with delta+range constraint
   subtype Small_Delta is Duration delta 0.001 range 0.0 .. 1.0;
   type Delta_Record (T : Small_Delta := 0.0) is                -- UndCC_Valid
      record
         Value : Integer;
      end record;

   -- Valid: variant record with enumeration discriminant
   type Shape_Kind is (Circle, Rectangle);
   type Shape (Kind : Shape_Kind := Circle) is                  -- UndCC_Valid
      record
         case Kind is
            when Circle =>
               Radius : Float;
            when Rectangle =>
               Width : Float;
               Height : Float;
         end case;
      end record;

   -- Multiple discriminants: one unconstrained, one constrained
   subtype Max_Cols is Natural range 1 .. 80;
   type Grid (Rows : Natural := 0; Cols : Max_Cols := 1) is     -- UndCC_Violation
      record
         Value : Integer;
      end record;

   -- Violation: tagged type with unconstrained discriminant
   type Tagged_T (N : Natural := 0) is tagged                   -- UndCC_Violation
      record
         Value : Integer;
      end record;

   -- Violation: unconstrained subtype of Integer resolves to "Integer"
   subtype Bare_Int is Integer;
   type Bare_Holder (D : Bare_Int := 0) is                      -- UndCC_Violation
      record
         Value : Integer;
      end record;

   -- Violation: discriminant with no default value, unconstrained Natural
   type No_Default_Holder (N : Natural) is                      -- UndCC_Violation
      record
         Data : Number_List (1 .. N);
      end record;

   -- Valid: discriminant with no default value, constrained subtype
   type No_Default_Constrained (N : Max_Numbers) is             -- UndCC_Valid
      record
         Data : Number_List (1 .. N);
      end record;

   -- Valid: modular type discriminant (constrained by its modulus)
   type Byte is mod 256;
   type Byte_Record (B : Byte := 0) is                          -- UndCC_Valid
      record
         Value : Integer;
      end record;

   -- Valid: discriminant typed with a user modular subtype
   subtype Half_Byte is Byte range 0 .. 15;
   type Half_Record (H : Half_Byte := 0) is                     -- UndCC_Valid
      record
         Value : Integer;
      end record;

   -- Violation: multiple unconstrained discriminants — each is flagged
   type Multi_Bad (Rows : Natural := 0; Cols : Natural := 0) is -- UndCC_Violation
      record
         Value : Integer;
      end record;

   -- Valid: multiple discriminants all constrained
   subtype Row_Count is Natural range 1 .. 100;
   subtype Col_Count is Natural range 1 .. 200;
   type Multi_Good (Rows : Row_Count := 1; Cols : Col_Count := 1) is -- UndCC_Valid
      record
         Value : Integer;
      end record;

private

   -- Violation: private type with unconstrained discriminant
   type Private_Holder (N : Natural := 0) is                    -- UndCC_Violation
      record
         Value : Integer;
      end record;

   -- Valid: private type with constrained discriminant
   subtype Private_Size is Natural range 0 .. 10;
   type Private_Constrained (N : Private_Size := 0) is          -- UndCC_Valid
      record
         Value : Integer;
      end record;

end Test_5_4_1;
