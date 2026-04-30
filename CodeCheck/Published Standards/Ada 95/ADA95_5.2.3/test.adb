package body Test_5_2_3 is

   procedure No_Params is
   begin
      null;
   end No_Params;

   procedure Single_No_Default (A : in Integer) is
   begin
      null;
   end Single_No_Default;

   procedure Single_With_Default (A : in Integer := 0) is
   begin
      null;
   end Single_With_Default;

   procedure No_Defaults (A : in Integer; B : in Integer) is
   begin
      null;
   end No_Defaults;

   procedure All_Defaults (A : in Integer := 0; B : in Integer := 0) is
   begin
      null;
   end All_Defaults;

   procedure All_Defaults_Three (A : in Integer := 0; B : in Integer := 0;
                                 C : in Integer := 0) is
   begin
      null;
   end All_Defaults_Three;

   procedure Default_At_End (A : in Integer; B : in Integer := 0) is
   begin
      null;
   end Default_At_End;

   procedure Two_Defaults_At_End (A : in Integer;
                                  B : in Integer := 0; C : in Integer := 0) is
   begin
      null;
   end Two_Defaults_At_End;

   procedure Three_Then_Two_Defaults (A : in Integer; B : in Integer; C : in Integer;
                                      D : in Integer := 0; E : in Integer := 0) is
   begin
      null;
   end Three_Then_Two_Defaults;

   function F_Valid (A : in Integer; B : in Integer := 0) return Integer is
   begin
      return A + B;
   end F_Valid;

   procedure Default_Before_NonDefault (A : in Integer := 0; B : in Integer) is
   begin
      null;
   end Default_Before_NonDefault;

   procedure Default_In_Middle (A : in Integer;
                                B : in Integer := 0; C : in Integer) is
   begin
      null;
   end Default_In_Middle;

   procedure Default_First_Of_Three (A : in Integer := 0;
                                     B : in Integer := 0; C : in Integer) is
   begin
      null;
   end Default_First_Of_Three;

   function F_Violation (A : in Integer := 0; B : in Integer) return Integer is
   begin
      return A + B;
   end F_Violation;

   procedure Grouped_No_Default (A, B : in Integer; C, D : in Integer := 0) is
   begin
      null;
   end Grouped_No_Default;

   procedure Mode_Before_Default (A : in out Integer; B : in Integer := 0) is
   begin
      null;
   end Mode_Before_Default;

   procedure Grouped_Default_Before (A, B : in Integer := 0; C : in Integer) is
   begin
      null;
   end Grouped_Default_Before;

   procedure Interleaved (A : in Integer; B : in Integer := 0;
                          C : in Integer; D : in Integer := 0) is
   begin
      null;
   end Interleaved;

   procedure Default_Before_Out (A : in Integer := 0; B : in out Integer) is
   begin
      null;
   end Default_Before_Out;

   procedure Deep_Violation (A : in Integer; B : in Integer; C : in Integer;
                             D : in Integer := 0; E : in Integer := 0; F : in Integer) is
   begin
      null;
   end Deep_Violation;

   -- Body-only: valid default order (no spec in test.ads)
   procedure Body_Only_Valid (A : in Integer; B : in Integer := 0) is   -- UndCC_Valid
   begin
      null;
   end Body_Only_Valid;

   -- Body-only: violation (no spec in test.ads)
   procedure Body_Only_Violation (A : in Integer := 0; B : in Integer) is   -- UndCC_Violation
   begin
      null;
   end Body_Only_Violation;

   procedure Run_Tests is
   begin
      null;
   end Run_Tests;

end Test_5_2_3;
