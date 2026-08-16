with Ada.Unchecked_Deallocation;

package Test is

   type Rec is record
      Value : Integer;
   end record;

   type Rec_Access is access Rec;

   procedure Visible_Free is new Ada.Unchecked_Deallocation (Object => Rec, Name => Rec_Access);  -- UndCC_Violation

   procedure Do_Something;

   -- Case-insensitive, unqualified (use-clause) access to the generic
   -- still resolves to Ada.Unchecked_Deallocation, and a rename of a
   -- spec-declared instantiation is itself flagged too since it
   -- re-exposes the deallocation in the visible part.
   package Alt_Naming_Demo is
      use Ada;

      procedure free_lc is new unchecked_deallocation (Object => Rec, Name => Rec_Access);  -- UndCC_Violation

      procedure Alias_Free (X : in out Rec_Access) renames free_lc;  -- UndCC_Violation
   end Alt_Naming_Demo;

private

   procedure Private_Free is new Ada.Unchecked_Deallocation (Object => Rec, Name => Rec_Access);  -- UndCC_Violation

end Test;
