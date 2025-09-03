with Ada.Text_IO;
use Ada.Text_IO;

procedure Main_Program is

   -- ## Non-Compliant ##


   -- for I in 1 .. 10 loop  -- UndCC_Violation
   --    Put_Line ("This is a non-compliant loop");
   -- end loop;


   -- My_Var : Integer := 10;  -- UndCC_FalseNeg


   -- procedure Do_Something is  -- UndCC_Violation
   -- begin
   --    null;
   -- end Do_Something;

   --------------------------------------------------------------------------------

   -- ## Compliant ##

   -- Compliant: A simple, descriptive comment explaining the purpose of a variable.
   -- The variable `Maximum_Attempts` stores the maximum number of login retries allowed.
   Maximum_Attempts : constant Integer := 3;

   -- Compliant
   -- This statement iterates from 1 to 5 to demonstrate a compliant comment.
   -- Code functional.
begin



   for I in 1 .. 5 loop
      Put_Line ("This is a compliant loop iteration " & Integer'Image (I));
   end loop;
   
   Put_Line ("Program finished.");

end Main_Program;
