-- Valid: Test_Used_Pkg is called in the body
with Test_Used_Pkg;   -- UndCC_Valid
-- Violation: Test_Unused_Pkg is never referenced
with Test_Unused_Pkg; -- UndCC_Violation

package body Test_4_2_3 is
   procedure Run is
   begin
      Test_Used_Pkg.Do_Work;
   end Run;
end Test_4_2_3;
