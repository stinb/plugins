-- Valid: Test_Used_Pkg is referenced (type usage in spec)
with Test_Used_Pkg;   -- UndCC_Valid
-- Violation: Test_Unused_Pkg is never referenced
with Test_Unused_Pkg; -- UndCC_Violation

package Test_4_2_3 is
   subtype My_Value is Test_Used_Pkg.Base_Value;
   procedure Run;
end Test_4_2_3;
