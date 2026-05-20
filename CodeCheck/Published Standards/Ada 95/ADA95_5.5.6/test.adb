package body Test_5_5_6 is

   function Is_Freezing (T : Temperature) return Boolean is
   begin
      -- Violation: = with float type on left side
      return T = 0.0; -- UndCC_Violation
   end Is_Freezing;

   function Is_Zero (C : Count) return Boolean is
   begin
      -- Valid: = with integer type
      return C = 0; -- UndCC_Valid
   end Is_Zero;

   procedure Check_Temp (T : Temperature) is
      Max : constant Temperature := 100.0;
   begin
      -- Violation: = with float on left side
      if T = 100.0 then -- UndCC_Violation
         null;
      end if;

      -- Violation: = with float on right side (Max is Temperature)
      if 0.0 = Max then -- UndCC_Violation
         null;
      end if;

      -- Violation: = between two float variables
      if T = Max then -- UndCC_Violation
         null;
      end if;

      -- Valid: <= with float
      if T <= 100.0 then -- UndCC_Valid
         null;
      end if;

      -- Valid: >= with float
      if T >= 0.0 then -- UndCC_Valid
         null;
      end if;

      -- Valid: /= with float
      if T /= Max then -- UndCC_Valid
         null;
      end if;

      -- Violation: exit when using = on float (the most dangerous case)
      loop
         exit when T = Max; -- UndCC_Violation
      end loop;
   end Check_Temp;

   procedure Check_Ratio (R : Ratio) is
      Zero : constant Ratio := 0.0;
   begin
      -- Violation: = with fixed/float type
      if R = Zero then -- UndCC_Violation
         null;
      end if;

      -- Valid: >= instead
      if R >= Zero then -- UndCC_Valid
         null;
      end if;
   end Check_Ratio;

   procedure Check_Mixed (C : Count; T : Temperature) is
   begin
      -- Valid: = with integer type
      if C = 5 then -- UndCC_Valid
         null;
      end if;

      -- Violation: = with float type
      if T = Temperature'First then -- UndCC_Violation
         null;
      end if;
   end Check_Mixed;

   procedure Check_Fixed (P : Price) is
      Base : constant Price := 0.0;
   begin
      -- Violation: = with fixed-point type (delta)
      if P = Base then -- UndCC_Violation
         null;
      end if;

      -- Valid: >= with fixed-point type
      if P >= Base then -- UndCC_Valid
         null;
      end if;
   end Check_Fixed;

   procedure Check_Derived (D : Derived_Temp) is
      Ref : constant Derived_Temp := 0.0;
   begin
      -- Violation: derived float type resolved via Derivefrom ref chain
      if D = Ref then -- UndCC_Violation
         null;
      end if;
   end Check_Derived;

   procedure Check_Expressions (T : Temperature) is
      Max : constant Temperature := 100.0;
   begin
      -- Violation: left side complex expression, right side is identifier (caught)
      if (T + 1.0) = Max then -- UndCC_Violation
         null;
      end if;

      -- FalseNeg: both sides are complex expressions, no identifier directly adjacent to =
      if (T + 1.0) = (Max - 1.0) then -- UndCC_FalseNeg
         null;
      end if;

      -- Violation: negated float comparison still fires
      if not (T = Max) then -- UndCC_Violation
         null;
      end if;
   end Check_Expressions;

   procedure Check_Non_Real (C : Count; Flag : Boolean) is
      Name : Name_String := (others => ' ');
      Other : Name_String := (others => ' ');
   begin
      -- Valid: = with string type
      if Name = Other then -- UndCC_Valid
         null;
      end if;

      -- Valid: = with boolean type
      if Flag = True then -- UndCC_Valid
         null;
      end if;

      -- Valid: = with integer type
      if C = Count'Last then -- UndCC_Valid
         null;
      end if;
   end Check_Non_Real;

end Test_5_5_6;
