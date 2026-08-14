package body Test is

   function Checked_Inst is new Ada.Unchecked_Conversion (Source => Integer, Target => Color);    -- UndCC_Valid
   function Unchecked_Inst is new Ada.Unchecked_Conversion (Source => Integer, Target => Color);  -- UndCC_Valid
   function Rec_Inst is new Ada.Unchecked_Conversion (Source => Integer, Target => Rec);           -- UndCC_Valid

   procedure Do_Something (N : Integer) is
      C1 : Color;
      C2 : Color;
      R1 : Rec;
   begin
      C1 := Checked_Inst (N);

      if C1'Valid then
         null;
      end if;

      C2 := Unchecked_Inst (N);  -- UndCC_Violation

      R1 := Rec_Inst (N);  -- UndCC_Valid

      null;
   end Do_Something;

   -- Calls reached through a rename of the instantiation, and through
   -- parameter passing, must still be tracked for a missing 'Valid check.
   package More_Cases is
      procedure Run (N : Integer);
   end More_Cases;

   package body More_Cases is

      function Local_Inst is new Ada.Unchecked_Conversion (Source => Integer, Target => Color);  -- UndCC_Valid
      function Local_Alias (S : Integer) return Color renames Local_Inst;

      procedure Consume (C : Color) is
      begin
         null;
      end Consume;

      procedure Run (N : Integer) is
         Via_Alias : Color;
      begin
         if Local_Inst (N)'Valid then
            null;
         end if;

         Via_Alias := Local_Alias (N);  -- UndCC_Violation

         Consume (Local_Inst (N));  -- UndCC_Violation
      end Run;

   end More_Cases;

   -- A private type whose full view is a record is not scalar, so no
   -- 'Valid check is required even though the target looks opaque here.
   package Priv_Target_Demo is
      type Priv_Rec is private;
      procedure Run (N : Integer);
   private
      type Priv_Rec is record
         Value : Integer;
      end record;
   end Priv_Target_Demo;

   package body Priv_Target_Demo is

      function Uc_Priv_Rec is new Ada.Unchecked_Conversion (Source => Integer, Target => Priv_Rec);  -- UndCC_Valid

      procedure Run (N : Integer) is
         Result : Priv_Rec;
      begin
         Result := Uc_Priv_Rec (N);  -- UndCC_Valid
         null;
      end Run;

   end Priv_Target_Demo;

   -- Case-insensitive, unqualified (use-clause) access to the generic
   -- still resolves to Ada.Unchecked_Conversion, and a bare return is
   -- tracked as a sink needing a 'Valid check just like an assignment.
   package Alt_Naming_Demo is
      function Get_Color (N : Integer) return Color;
   end Alt_Naming_Demo;

   package body Alt_Naming_Demo is
      use Ada;

      function unqualified_inst is new unchecked_conversion (Source => Integer, Target => Color);

      function Get_Color (N : Integer) return Color is
      begin
         return unqualified_inst (N);  -- UndCC_Violation
      end Get_Color;

   end Alt_Naming_Demo;

   -- A private type whose full view is scalar still requires a 'Valid
   -- check; one whose full view is access does not.
   package Priv_Variants_Demo is
      type Priv_Scalar is private;
      type Priv_Access is private;
      procedure Run (N : Integer);
   private
      type Priv_Scalar is range 1 .. 100;
      type Priv_Access is access Integer;
   end Priv_Variants_Demo;

   package body Priv_Variants_Demo is

      function Uc_Scalar is new Ada.Unchecked_Conversion (Source => Integer, Target => Priv_Scalar);  -- UndCC_Valid
      function Uc_Access is new Ada.Unchecked_Conversion (Source => Integer, Target => Priv_Access);  -- UndCC_Valid

      procedure Run (N : Integer) is
         S : Priv_Scalar;
         A : Priv_Access;
      begin
         S := Uc_Scalar (N);  -- UndCC_Violation
         A := Uc_Access (N);  -- UndCC_Valid
         null;
      end Run;

   end Priv_Variants_Demo;

end Test;
