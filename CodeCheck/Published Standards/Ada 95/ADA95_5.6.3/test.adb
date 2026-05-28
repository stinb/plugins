package body Test_5_6_3 is

   procedure Enum_Range_Violation (C : Color) is
   begin
      -- Violation: range of enumeration literals
      case C is
         when Red .. Blue => null; -- UndCC_Violation
         when Purple      => null;
         when Yellow      => null;
      end case;
   end Enum_Range_Violation;

   procedure Enum_Range_In_Alternative (C : Color) is
   begin
      -- Violation: enum range mixed with single value using |
      case C is
         when Red | Blue .. Purple => null; -- UndCC_Violation
         when Yellow | Green       => null;
      end case;
   end Enum_Range_In_Alternative;

   procedure Explicit_Enumeration_Valid (C : Color) is
   begin
      -- Valid: each value listed explicitly with |
      case C is
         when Red | Yellow => null;
         when Green | Blue => null;
         when Purple       => null;
      end case;
   end Explicit_Enumeration_Valid;

   procedure Character_Range_Valid (Ch : Character) is
   begin
      -- Valid: character literal ranges are the accepted exception
      case Ch is
         when 'a' .. 'z' => null;
         when 'A' .. 'Z' => null;
         when '0' .. '9' => null;
         when others     => null;
      end case;
   end Character_Range_Valid;

   procedure Subtype_Range_Valid (D : Day) is
   begin
      -- Valid: named subtype used as a choice, not a raw enum literal range
      case D is
         when Weekday => null;
         when Weekend => null;
      end case;
   end Subtype_Range_Valid;

   procedure Others_Valid (C : Color) is
   begin
      -- Valid: others clause is a separate guideline (not automated)
      case C is
         when Red    => null;
         when others => null;
      end case;
   end Others_Valid;

   procedure Single_Enum_Valid (C : Color) is
   begin
      -- Valid: single enum literals, no ranges
      case C is
         when Red    => null;
         when Yellow => null;
         when Green  => null;
         when Blue   => null;
         when Purple => null;
      end case;
   end Single_Enum_Valid;

   procedure Multiple_Ranges_Violation (D : Day) is
   begin
      -- Two separate enum ranges in different when clauses: two violations
      case D is
         when Mon .. Wed => null; -- UndCC_Violation
         when Thu .. Fri => null; -- UndCC_Violation
         when Sat | Sun  => null;
      end case;
   end Multiple_Ranges_Violation;

   procedure Nested_Case_Inner_Violation (C : Color; D : Day) is
   begin
      -- Outer case valid, inner case has the violation
      case C is
         when Red | Yellow =>
            case D is
               when Mon .. Fri => null; -- UndCC_Violation
               when Sat | Sun  => null;
            end case;
         when Green | Blue | Purple => null;
      end case;
   end Nested_Case_Inner_Violation;

   procedure Nested_Case_Outer_Violation (C : Color; D : Day) is
   begin
      -- Outer case has a violation, inner case is valid
      case C is
         when Red .. Blue =>          -- UndCC_Violation
            case D is
               when Mon | Tue => null;
               when others    => null;
            end case;
         when Yellow | Purple => null;
      end case;
   end Nested_Case_Outer_Violation;

   procedure Range_At_End_Of_Alternatives (C : Color) is
   begin
      -- Enum range appears after a | separator
      case C is
         when Yellow | Red .. Purple => null; -- UndCC_Violation
      end case;
   end Range_At_End_Of_Alternatives;

   procedure Exception_Handler_When_Valid (C : Color) is
   begin
      -- Valid: 'when' in exception handler is not inside a case statement
      case C is
         when Red    => null;
         when others => null;
      end case;
   exception
      when Constraint_Error => null;
      when others           => null;
   end Exception_Handler_When_Valid;

   procedure Integer_Range_Valid (X : Integer) is
   begin
      -- Valid: integer literal ranges are not enum literal ranges
      case X is
         when 1 .. 5  => null;
         when 6 .. 10 => null;
         when others  => null;
      end case;
   end Integer_Range_Valid;

   procedure Two_Ranges_Same_When (C : Color; D : Day) is
   begin
      -- Two enum ranges in a single 'when' alternative: two violations
      case C is
         when Red .. Yellow => null; -- UndCC_Violation
         when Green         => null;
         when Blue | Purple => null;
      end case;

      case D is
         when Mon | Tue .. Thu | Fri => null; -- UndCC_Violation
         when Sat | Sun              => null;
      end case;
   end Two_Ranges_Same_When;

end Test_5_6_3;
