-- Test file for ADA95_2.1.2: Indentation
-- Use spaces for indentation, not tab characters.

procedure Test_2_1_2 is
   Counter : Integer := 0;
begin
   -- Valid: indented with spaces
   if Counter > 0 then                    -- UndCC_Valid
      Counter := Counter - 1;
   end if;

   -- Violation: line indented with a single tab
	Counter := 1;                          -- UndCC_Violation

   -- Violation: line indented with consecutive tabs
		Counter := 2;                          -- UndCC_Violation

   if Counter = 0 then                    -- UndCC_Valid
      null;
   end if;
end Test_2_1_2;
