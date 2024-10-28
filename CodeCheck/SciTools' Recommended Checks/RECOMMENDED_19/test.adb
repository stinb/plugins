    with Ada.Text_IO; use Ada.Text_IO;
    procedure Hello is
		usedVar : String := "Hello World!"; 
		unUsedVar: character := 'Z';        -- UndCC_Violation FIXME: do something
    begin
       Put_Line (usedVar);  -- UndCC_Violation FIX-ME: do something here too
    end Hello;

