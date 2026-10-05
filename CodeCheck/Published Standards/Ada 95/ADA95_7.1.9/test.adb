with Ada.Text_IO;

package body Test_7_1_9 is

   ID : Integer := 0;

   ID2 : Integer := 0;

   function Unique_ID return Integer is
   begin
      ID := ID + 1;
      return ID;
   end Unique_ID;

   function Another_ID return Integer is
   begin
      ID2 := ID2 + 1;
      return ID2;
   end Another_ID;

   function Pure_Add (X, Y : Integer) return Integer is
   begin
      return X + Y;
   end Pure_Add;

   procedure Use_Twice (A, B : Integer) is
   begin
      null;
   end Use_Twice;

   procedure Probe is
   begin
      Use_Twice (Unique_ID, Unique_ID);  -- UndCC_Violation
      Ada.Text_IO.Put_Line (Integer'Image (Unique_ID) & Integer'Image (Unique_ID));  -- UndCC_Violation
      Use_Twice (Pure_Add (1, 2), Pure_Add (3, 4));  -- UndCC_Valid
      Use_Twice (Unique_ID, Another_ID);  -- UndCC_Valid
      Use_Twice (Unique_ID, 0);  -- UndCC_Valid
      Use_Twice (0, Unique_ID);  -- UndCC_Valid
   end Probe;

end Test_7_1_9;
