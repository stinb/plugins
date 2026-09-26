with Text_IO;                       -- UndCC_Violation
with Ada.Text_IO;                   -- UndCC_Valid
with Unchecked_Deallocation;        -- UndCC_Violation
with Ada.Unchecked_Deallocation;    -- UndCC_Valid

package Test_7_1_1 is

   type Priv is private;

   type Visible_Rec (D : Boolean := True) is record
      X : Integer;
   end record;

   procedure Use_It (P : Priv; V : Visible_Rec);

   type Int_Access is access Integer;

   procedure Free is new Unchecked_Deallocation (Integer, Int_Access);
   procedure Free2 is new Ada.Unchecked_Deallocation (Integer, Int_Access);

   generic
      type Item is private;
   package Gen is
      function Check_It (X : Item) return Boolean;
   end Gen;

private

   type Priv (D : Boolean := True) is record
      X : Integer;
   end record;

end Test_7_1_1;
