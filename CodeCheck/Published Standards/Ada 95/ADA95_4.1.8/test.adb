-- Valid: variables in package body are not flagged
package body Test_4_1_8 is

   Body_Counter : Integer := 0;    -- UndCC_Valid
   Line_State   : Line_Number := 1; -- UndCC_Valid

   function Source_Line_Number return Line_Number is
   begin
      return Line_State;
   end Source_Line_Number;

   procedure Reset is
      Local_Temp : Integer := 0;   -- UndCC_Valid (local variable in subprogram)
   begin
      Body_Counter := 0;
      Line_State   := 1;
      Internal_State := Local_Temp;
   end Reset;

end Test_4_1_8;
