-- Spec file for ADA95_4.1.1 test

package Test_4_1_1 is

   -- Valid: package declared in spec (not in body)
   package Spec_Nested is -- UndCC_Valid
      type Color is (Red, Green, Blue);
   end Spec_Nested;

   procedure Run;

end Test_4_1_1;
