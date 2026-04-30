package body Test_5_2_4 is

   procedure No_Params is
   begin
      null;
   end No_Params;

   procedure All_In (A : in Integer; B : in Integer) is
   begin
      null;
   end All_In;

   procedure Mixed_Modes (A : in Integer; B : out Integer; C : in out Integer) is
   begin
      B := A;
      C := C + A;
   end Mixed_Modes;

   procedure Single_Out (A : out Integer) is
   begin
      A := 0;
   end Single_Out;

   function F_Explicit (A : in Integer; B : in Integer) return Integer is
   begin
      return A + B;
   end F_Explicit;

   procedure Access_Param (A : access Integer; B : in Integer) is
   begin
      A.all := B;
   end Access_Param;

   procedure Multi_Line_Valid (A : in Integer;
                               B : out Integer; C : in out Integer) is
   begin
      B := A;
      C := C + A;
   end Multi_Line_Valid;

   procedure Single_No_Mode (A : Integer) is
   begin
      null;
   end Single_No_Mode;

   procedure First_Missing (A : Integer; B : in Integer) is
   begin
      null;
   end First_Missing;

   procedure Last_Missing (A : in Integer; B : Integer) is
   begin
      null;
   end Last_Missing;

   function F_No_Mode (A : Integer) return Integer is
   begin
      return A;
   end F_No_Mode;

   procedure All_Missing (A : Integer;
                          B : Integer) is
   begin
      null;
   end All_Missing;

   procedure Middle_Missing (A : in Integer;
                             B : Integer;
                             C : out Integer) is
   begin
      C := A + B;
   end Middle_Missing;

   procedure Grouped_Explicit (A, B : in Integer; C : out Integer) is
   begin
      C := A + B;
   end Grouped_Explicit;

   procedure Default_With_Mode (A : in Integer := 0; B : out Integer) is
   begin
      B := A;
   end Default_With_Mode;

   procedure Grouped_No_Mode (A,
                              B : Integer) is
   begin
      null;
   end Grouped_No_Mode;

   procedure Default_No_Mode (A : Integer := 0; B : out Integer) is
   begin
      B := A;
   end Default_No_Mode;

   procedure Subtype_No_Mode (A : Positive; B : out Integer) is
   begin
      B := A;
   end Subtype_No_Mode;

   procedure Access_Then_No_Mode (A : access Integer; B : Integer) is
   begin
      null;
   end Access_Then_No_Mode;

   function F_Multi_No_Mode (A : Integer;
                             B : Integer) return Integer is
   begin
      return A + B;
   end F_Multi_No_Mode;

   procedure Last_Of_Many (A : in Integer; B : in Integer;
                           C : out Integer; D : Integer) is
   begin
      C := A + B;
      null;
   end Last_Of_Many;

   -- Body-only: valid
   procedure Body_Only_Valid (A : in Integer; B : out Integer) is   -- UndCC_Valid
   begin
      B := A;
   end Body_Only_Valid;

   -- Body-only: violation
   procedure Body_Only_Violation (A : Integer; B : out Integer) is   -- UndCC_Violation
   begin
      B := A;
   end Body_Only_Violation;

   -- Body-only: grouped params, each on its own line, both missing mode
   procedure Body_Only_Grouped (A,                                     -- UndCC_Violation
                                B : Integer) is                        -- UndCC_Violation
   begin
      null;
   end Body_Only_Grouped;

   -- Body-only: default value present but mode missing
   procedure Body_Only_Default_No_Mode (A : Integer := 0) is   -- UndCC_Violation
   begin
      null;
   end Body_Only_Default_No_Mode;

   procedure Run_Tests is
   begin
      null;
   end Run_Tests;

end Test_5_2_4;
