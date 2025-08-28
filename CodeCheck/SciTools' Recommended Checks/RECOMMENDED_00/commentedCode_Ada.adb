with Ada.Text_IO;
use Ada.Text_IO;

procedure Main_Program is

   -- ## Non-Compliant ##
   -- These examples show common mistakes where code snippets are left in comments.

   -- Non-compliant: Code for a simple loop is commented out.
   -- for I in 1 .. 10 loop
   --    Put_Line ("This is a non-compliant loop");
   -- end loop;

   -- Non-compliant: A commented-out variable declaration.
   -- My_Var : Integer := 10;

   -- Non-compliant: An entire procedure is commented out.
   -- procedure Do_Something is
   -- begin
   --    null;
   -- end Do_Something;

   --------------------------------------------------------------------------------

   -- ## Compliant ##
   -- These examples demonstrate proper use of comments to explain code,
   -- without containing executable code themselves.

   -- Compliant: A simple, descriptive comment explaining the purpose of a variable.
   -- The variable `Maximum_Attempts` stores the maximum number of login retries allowed.
   Maximum_Attempts : constant Integer := 3;

   -- Compliant: A comment describing the purpose of a loop.
   -- This loop iterates from 1 to 5 to demonstrate a compliant comment.
   -- The code inside is functional.
begin
   -- The main program body starts here.

   -- Demonstrating the compliant loop.
   for I in 1 .. 5 loop
      Put_Line ("This is a compliant loop iteration " & Integer'Image (I));
   end loop;
   
   Put_Line ("Program finished.");

end Main_Program;