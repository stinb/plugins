with Ada.Text_IO; use Ada.Text_IO;

procedure Test is
   A : Integer := 10;
begin
   Put_Line("Short line"); 
   Put_Line("Limit source code line lengths to a maximum of 72 characters."); -- UndCC_Violation
end Test;