with Ada.Text_IO; use Ada.Text_IO;
    procedure commenttest is
    -- Great variable
		hasComment1 : String := "Hello World!";   -- UndCC_Violation
		hasComment2: character := 'A';  -- Not so good
    hasComment3: character := 'B';  -- Happy Variable
    noComment1: character := 'C'; -- UndCC_Violation

    begin
       Put_Line (usedVar);
    end commenttest;
