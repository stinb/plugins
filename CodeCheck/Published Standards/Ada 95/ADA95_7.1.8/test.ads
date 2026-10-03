package Test_7_1_8 is

   type Coordinates is record
      X : Integer := 0;
      Y : Integer := 0;
   end record;

   type Tagged_Coordinates is tagged record
      X : Integer := 0;
   end record;

   type Coordinates_Access is access Coordinates;

   procedure Touch (X : in out Coordinates);

   procedure Touch_Int (X : in out Integer);

   procedure Test_Bounded_Error
     (Parm_1 : in out Integer;   -- UndCC_Violation
      Parm_2 : in out Integer);  -- UndCC_Valid

   procedure Two_Records
     (A : in out Coordinates;   -- UndCC_Violation
      B : in out Coordinates);  -- UndCC_Violation

   procedure Direct_Access_Only
     (A : in out Coordinates;   -- UndCC_Valid
      B : in out Coordinates);  -- UndCC_Valid

   procedure Two_Tagged
     (A : in out Tagged_Coordinates;   -- UndCC_Valid
      B : in out Tagged_Coordinates);  -- UndCC_Valid

   procedure One_In_One_Out
     (A : in Coordinates;    -- UndCC_Valid
      B : out Coordinates);  -- UndCC_Valid

   procedure Single_Param (A : in out Coordinates);  -- UndCC_Valid

   procedure Both_In_Only (A : in Coordinates; B : in Coordinates);  -- UndCC_Valid

   procedure Different_Subtypes (A : in out Coordinates; B : in out Integer);  -- UndCC_Valid

   procedure Two_Access (A : access Coordinates; B : access Coordinates);  -- UndCC_Valid

   procedure Three_Records
     (A : in out Coordinates;   -- UndCC_Violation
      B : in Coordinates;      -- UndCC_Valid
      C : in out Coordinates);  -- UndCC_Valid

   task type Watcher is
      entry Update
        (A : in out Coordinates;   -- UndCC_Violation
         B : in out Coordinates);  -- UndCC_Violation
   end Watcher;

end Test_7_1_8;
