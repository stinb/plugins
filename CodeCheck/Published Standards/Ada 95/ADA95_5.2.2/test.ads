with Ada.Text_IO;
package Test_5_2_2 is

   procedure No_Params;
   procedure One_Param (A : in Integer);
   procedure Two_Params (A : in Integer; B : in Integer);
   procedure Three_Params (A : in Integer; B : in Integer; C : in Integer);
   procedure Four_Params (A : in Integer; B : in Integer;
                          C : in Integer; D : in Integer);
   procedure Frequent_One (A : in Integer);
   function F_Two (A : in Integer; B : in Integer) return Integer;
   procedure With_Default (A : in Integer; B : in Integer := 0);
   procedure All_Optional (A : in Integer := 0; B : in Integer := 0);

   -- Generic instantiation examples
   package Int_IO_Named    is new Ada.Text_IO.Integer_IO (Num => Integer);   -- UndCC_Valid
   package Int_IO_Positional is new Ada.Text_IO.Integer_IO (Integer);        -- UndCC_Violation

   procedure Run_Tests;

end Test_5_2_2;
