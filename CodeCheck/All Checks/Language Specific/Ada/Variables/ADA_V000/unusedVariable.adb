    with Ada.Text_IO; use Ada.Text_IO;
    procedure Hello is
		usedVar : String := "Hello World!"; -- UndCC_Valid
		unUsedVar: character := 'Z';        -- UndCC_Violation
    begin
       Put_Line (usedVar);
    end Hello;
