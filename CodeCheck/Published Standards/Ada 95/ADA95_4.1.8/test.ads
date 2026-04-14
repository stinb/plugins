-- Violations: variables declared in package specification
package Test_4_1_8 is

   type Line_Number is range 1 .. 2_500_000;

   Current_Line   : Line_Number := 1;       -- UndCC_Violation
   Error_Count    : Integer     := 0;       -- UndCC_Violation

   -- Valid: constants are not variables
   Max_Lines      : constant Line_Number := 2_500_000;  -- UndCC_Valid
   Version        : constant String      := "1.0";      -- UndCC_Valid

   -- Valid: subprogram declarations
   function Source_Line_Number return Line_Number;       -- UndCC_Valid
   procedure Reset;                                      -- UndCC_Valid

   -- Valid: type declarations
   type Status is (Active, Inactive);                   -- UndCC_Valid

private

   -- Violation: variable in private part of spec
   Internal_State : Integer := 0;                       -- UndCC_Violation

   -- Valid: constant in private part
   Hidden_Max     : constant Integer := 999;            -- UndCC_Valid

end Test_4_1_8;
