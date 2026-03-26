with Ada.Text_IO; use Ada.Text_IO;

procedure Test is

   Value : Integer := 10;

   -- Procedure with the same name as a local object
   procedure Value is -- UndCC_Violation
   begin
      Put_Line ("Inside procedure Value");
   end Value;

begin
   Put_Line ("Object Value = " & Integer'Image (Value));

   Value;  -- Calls the procedure, not the object

end Test;
