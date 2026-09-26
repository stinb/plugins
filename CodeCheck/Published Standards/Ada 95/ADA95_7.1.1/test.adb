package body Test_7_1_1 is

   package body Gen is
      function Check_It (X : Item) return Boolean is
      begin
         return X'Constrained;  -- UndCC_Violation
      end Check_It;
   end Gen;

   procedure Use_It (P : Priv; V : Visible_Rec) is
      B1 : Boolean := P'Constrained;       -- UndCC_Violation
      B2 : Boolean := V'Constrained;       -- UndCC_Valid
      C  : Character := ASCII.LC_A;        -- UndCC_Violation
   begin
      begin
         null;
      exception
         when Numeric_Error =>             -- UndCC_Violation
            null;
      end;

      begin
         null;
      exception
         when Constraint_Error =>          -- UndCC_Valid
            null;
      end;
   end Use_It;

end Test_7_1_1;
