-- Test file for ADA95_2.1.7: Pagination
-- Nested program unit bodies must be preceded by a dash separator line.

procedure Test_2_1_7 is

   ----------------------------------------------------------------
   function Max (Left, Right : Integer) return Integer is  -- UndCC_Valid
   begin
      if Right < Left then
         return Left;
      else
         return Right;
      end if;
   end Max;

   ----------------------------------------------------------------
   function Min (Left, Right : Integer) return Integer is  -- UndCC_Valid
   begin
      if Left < Right then
         return Left;
      else
         return Right;
      end if;
   end Min;

   -- Missing separator before this function
   function Add (Left, Right : Integer) return Integer is  -- UndCC_Violation
   begin
      return Left + Right;
   end Add;

   Some_Code : Integer := 0;

   procedure No_Sep_Procedure is                          -- UndCC_Violation
   begin
      null;
   end No_Sep_Procedure;

   ----------------------------------------------------------------
   procedure Valid_Procedure is                           -- UndCC_Valid
   begin
      null;
   end Valid_Procedure;

   ----------------------------------------------------------------

begin
   null;
end Test_2_1_7;
