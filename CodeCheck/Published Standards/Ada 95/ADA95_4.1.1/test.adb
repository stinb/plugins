-- Test file for ADA95_4.1.1: Separate Compilation Capabilities
-- Only guideline 7 is checked: no packages nested inside package bodies.

package body Test_4_1_1 is -- Test_4_1_1

   -- Violation: package nested directly in a package body
   package Local_State is -- UndCC_Violation
      type Mode is (Active, Idle);
      Current : Mode := Idle;
   end Local_State;

   -- Valid: nested procedure in body (not a package)
   procedure Helper is -- UndCC_Valid
   begin
      null;
   end Helper;

   -- Violation: another nested package in the same body
   package Internal_Cache is -- UndCC_Violation
      Max_Size : constant := 64;
   end Internal_Cache;

   procedure Run is
   begin
      null;
   end Run;

end Test_4_1_1;
