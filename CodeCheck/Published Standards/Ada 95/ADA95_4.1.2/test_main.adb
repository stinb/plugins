-- Valid: configuration pragmas before a procedure (could be main subprogram)
pragma Normalize_Scalars;                    -- UndCC_Valid
pragma Restrictions (No_Dynamic_Allocation); -- UndCC_Valid

procedure Test_Main is
begin
   null;
end Test_Main;
