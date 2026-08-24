with Ada.Direct_IO;
with Ada.Sequential_IO;

package Test is

   type Color is (Red, Yellow, Blue);
   for Color'Size use Integer'Size;

   type Rec is record
      Value : Integer;
   end record;

   package Color_Direct_IO is new Ada.Direct_IO (Color);
   package Color_Seq_IO is new Ada.Sequential_IO (Color);
   package Rec_Direct_IO is new Ada.Direct_IO (Rec);

   procedure Do_Something;

end Test;
