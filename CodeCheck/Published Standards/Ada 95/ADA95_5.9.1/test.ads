with Ada.Unchecked_Conversion;

package Test is

   type Color is (Red, Yellow, Blue);
   for Color'Size use Integer'Size;

   type Rec is record
      Value : Integer;
   end record;

   function Visible_Inst is new Ada.Unchecked_Conversion (Source => Integer, Target => Color);  -- UndCC_Violation

   procedure Do_Something (N : Integer);

private

   function Private_Inst is new Ada.Unchecked_Conversion (Source => Integer, Target => Color);  -- UndCC_Violation

end Test;
