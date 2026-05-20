package body Test_5_5_1 is

   procedure Do_Work is
      type Temp_Values is array (Temperature) of Integer;
      Vals   : Temp_Values := (others => 0);
      Slice  : Temp_Values;
      X      : Integer := 0;
      I      : Temperature := Temperature'First;
   begin
      -- Violation: for loop using T'First .. T'Last
      for J in Temperature'First .. Temperature'Last loop -- UndCC_Violation
         X := Integer(J);
      end loop;

      -- Valid: for loop using 'Range
      for J in Temperature'Range loop -- UndCC_Valid
         X := Integer(J);
      end loop;

      -- Valid: for loop using type name
      for J in Temperature loop -- UndCC_Valid
         X := Integer(J);
      end loop;

      -- Violation: reverse for loop using T'First .. T'Last
      for J in reverse Temperature'First .. Temperature'Last loop -- UndCC_Violation
         X := Integer(J);
      end loop;

      -- Violation: membership test using T'First .. T'Last
      if I in Temperature'First .. Temperature'Last then -- UndCC_Violation
         null;
      end if;

      -- Valid: membership test using 'Range
      if I in Temperature'Range then -- UndCC_Valid
         null;
      end if;

      -- Violation: array slice using T'First .. T'Last
      Slice := Vals(Temperature'First .. Temperature'Last); -- UndCC_Violation

      -- Valid: array slice using 'Range
      Slice := Vals(Temperature'Range); -- UndCC_Valid

      -- Violation: multi-line T'First .. T'Last (whitespace is skipped)
      for J in
         Temperature'First .. -- UndCC_Violation
         Temperature'Last loop
         null;
      end loop;

      -- Violation: Color enum range
      for C in Color'First .. Color'Last loop -- UndCC_Violation
         null;
      end loop;

      -- Valid: Color using 'Range
      for C in Color'Range loop -- UndCC_Valid
         null;
      end loop;

      -- Valid: type-converted bounds are a different pattern (Integer range, not Temperature)
      if X in Integer(Temperature'First) .. Integer(Temperature'Last) then -- UndCC_Valid
         null;
      end if;

      -- Valid: only 'First used standalone (no .. follows)
      X := Integer(Temperature'First); -- UndCC_Valid

      -- Valid: only 'Last used standalone
      X := Integer(Temperature'Last); -- UndCC_Valid

      -- Valid: 'Pred attribute (not 'First/'Last, should not trigger)
      X := Integer(Color'Pos(Color'Pred(Color'Last))); -- UndCC_Valid
   end Do_Work;

end Test_5_5_1;
