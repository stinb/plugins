-- Violation test cases cannot be written for this check: any UndCC annotation
-- placed on a return statement line is itself a comment, which satisfies the
-- same-line comment condition and prevents the violation from firing. Only
-- non-violation cases are tested.

package body Test_5_6_8 is

   function Comment_Before_Return_Valid (X : Integer) return Integer is
   begin
      if X > 0 then
         -- Return the positive value, UndCC_Valid
         return X;
      end if;
      -- Return zero as default, UndCC_Valid
      return 0;
   end Comment_Before_Return_Valid;

   function Blank_Line_Before_Return_Valid (X : Integer) return Integer is
   begin
      if X > 0 then

         return X; -- UndCC_Valid

      end if;

      return 0; -- UndCC_Valid
   end Blank_Line_Before_Return_Valid;

   function Return_After_Begin_Valid (X : Integer) return Integer is
   begin
      return X; -- UndCC_Valid
   end Return_After_Begin_Valid;

   function Case_Branch_Return_Valid (X : Integer) return Integer is
   begin
      case X is
         when 1      =>
            return 100; -- UndCC_Valid

         when 2      =>
            return 200; -- UndCC_Valid

         when others =>
            return 0; -- UndCC_Valid
      end case;
   end Case_Branch_Return_Valid;

   function Inline_Comment_Return_Valid (X : Integer) return Integer is
   begin
      if X > 0 then
         return X;  -- UndCC_Valid

      end if;
      return 0;  -- UndCC_Valid
   end Inline_Comment_Return_Valid;

   procedure Bare_Return_Blank_Line_Valid (C : Boolean) is
   begin
      if C then

         return; -- UndCC_Valid

      end if;
      null;
   end Bare_Return_Blank_Line_Valid;

   function Multiple_Returns_All_Highlighted_Valid
     (X, Y : Integer) return Integer is
   begin
      if X > Y then
         -- X is the greater value, UndCC_Valid
         return X;
      elsif Y > X then
         -- Y is the greater value, UndCC_Valid
         return Y;
      end if;
      -- Values are equal; return either, UndCC_Valid
      return 0;
   end Multiple_Returns_All_Highlighted_Valid;

end Test_5_6_8;
