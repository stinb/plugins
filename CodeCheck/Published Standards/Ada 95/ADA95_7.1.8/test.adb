package body Test_7_1_8 is

   procedure Touch (X : in out Coordinates) is
   begin
      null;
   end Touch;

   procedure Touch_Int (X : in out Integer) is
   begin
      null;
   end Touch_Int;

   procedure Test_Bounded_Error (Parm_1 : in out Integer; Parm_2 : in out Integer) is
      procedure Inner (Parm : in out Integer) is
      begin
         Parm := Parm * 10;
      end Inner;
   begin
      Parm_2 := 5;
      Inner (Parm_1);
   end Test_Bounded_Error;

   procedure Two_Records (A : in out Coordinates; B : in out Coordinates) is
   begin
      Touch (A);
      Touch (B);
   end Two_Records;

   procedure Direct_Access_Only (A : in out Coordinates; B : in out Coordinates) is
   begin
      A.X := B.X;
   end Direct_Access_Only;

   procedure Two_Tagged (A : in out Tagged_Coordinates; B : in out Tagged_Coordinates) is
   begin
      A.X := B.X;
   end Two_Tagged;

   procedure One_In_One_Out (A : in Coordinates; B : out Coordinates) is
   begin
      B := A;
   end One_In_One_Out;

   procedure Single_Param (A : in out Coordinates) is
   begin
      Touch (A);
   end Single_Param;

   procedure Both_In_Only (A : in Coordinates; B : in Coordinates) is
   begin
      null;
   end Both_In_Only;

   procedure Different_Subtypes (A : in out Coordinates; B : in out Integer) is
   begin
      Touch (A);
      Touch_Int (B);
   end Different_Subtypes;

   procedure Two_Access (A : access Coordinates; B : access Coordinates) is
   begin
      null;
   end Two_Access;

   procedure Three_Records (A : in out Coordinates; B : in Coordinates; C : in out Coordinates) is
   begin
      Touch (A);
   end Three_Records;

   task body Watcher is
   begin
      accept Update (A : in out Coordinates; B : in out Coordinates) do
         Touch (A);
         Touch (B);
      end Update;
   end Watcher;

   procedure Body_Only
     (A : in out Coordinates;   -- UndCC_Violation
      B : in out Coordinates)  -- UndCC_Violation
   is
   begin
      Touch (A);
      Touch (B);
   end Body_Only;

   procedure Outer_With_Forward is
      procedure Find_Colon (Index : in out Integer);  -- UndCC_Valid

      procedure Find_Colon (Index : in out Integer) is  -- UndCC_Valid
      begin
         Index := Index + 1;
      end Find_Colon;

      N : Integer := 0;
   begin
      Find_Colon (N);
   end Outer_With_Forward;

end Test_7_1_8;
