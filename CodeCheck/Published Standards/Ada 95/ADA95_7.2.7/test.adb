package body Test_7_2_7 is

   function Is_Freezing (T : Temperature) return Boolean is
   begin
      return T = 0.0; -- UndCC_Violation
   end Is_Freezing;

   function Is_Zero (C : Count) return Boolean is
   begin
      return C = 0; -- UndCC_Valid
   end Is_Zero;

   procedure Check_Temp (T : Temperature) is
      Max : constant Temperature := 100.0;
   begin
      if T = 100.0 then -- UndCC_Violation
         null;
      end if;

      if 0.0 = Max then -- UndCC_Violation
         null;
      end if;

      if T = Max then -- UndCC_Violation
         null;
      end if;

      if T <= 100.0 then -- UndCC_Valid
         null;
      end if;

      if T >= 0.0 then -- UndCC_Valid
         null;
      end if;

      -- Violation: /= is also flagged by this check, unlike ADA95_5.5.6
      if T /= Max then -- UndCC_Violation
         null;
      end if;

      loop
         exit when T = Max; -- UndCC_Violation
      end loop;
   end Check_Temp;

   procedure Check_Ratio (R : Ratio) is
      Zero : constant Ratio := 0.0;
   begin
      if R = Zero then -- UndCC_Violation
         null;
      end if;

      if R /= Zero then -- UndCC_Violation
         null;
      end if;

      if R >= Zero then -- UndCC_Valid
         null;
      end if;
   end Check_Ratio;

   procedure Check_Mixed (C : Count; T : Temperature) is
   begin
      if C = 5 then -- UndCC_Valid
         null;
      end if;

      if C /= 5 then -- UndCC_Valid
         null;
      end if;

      if T = Temperature'First then -- UndCC_Violation
         null;
      end if;
   end Check_Mixed;

   procedure Check_Fixed (P : Price) is
      Base : constant Price := 0.0;
   begin
      if P = Base then -- UndCC_Violation
         null;
      end if;

      if P /= Base then -- UndCC_Violation
         null;
      end if;

      if P >= Base then -- UndCC_Valid
         null;
      end if;
   end Check_Fixed;

   procedure Check_Derived (D : Derived_Temp) is
      Ref : constant Derived_Temp := 0.0;
   begin
      if D = Ref then -- UndCC_Violation
         null;
      end if;
   end Check_Derived;

   procedure Check_Expressions (T : Temperature) is
      Max : constant Temperature := 100.0;
   begin
      if (T + 1.0) = Max then -- UndCC_Violation
         null;
      end if;

      -- FalseNeg: both sides are complex expressions, no identifier directly adjacent to =
      if (T + 1.0) = (Max - 1.0) then -- UndCC_FalseNeg
         null;
      end if;

      if not (T = Max) then -- UndCC_Violation
         null;
      end if;
   end Check_Expressions;

   procedure Check_Non_Real (C : Count; Flag : Boolean) is
      Name : Name_String := (others => ' ');
      Other : Name_String := (others => ' ');
   begin
      if Name = Other then -- UndCC_Valid
         null;
      end if;

      if Flag = True then -- UndCC_Valid
         null;
      end if;

      if C = Count'Last then -- UndCC_Valid
         null;
      end if;
   end Check_Non_Real;

end Test_7_2_7;
