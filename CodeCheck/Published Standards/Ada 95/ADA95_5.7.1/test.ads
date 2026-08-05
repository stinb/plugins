with Ada.Text_IO;
use Ada.Text_IO; -- UndCC_Violation (use clause on the package's context clause affects the whole spec and body)

with Ada.Strings.Unbounded;

package Test_5_7_1 is

   use type Ada.Strings.Unbounded.Unbounded_String; -- UndCC_Valid (use type is recommended for operator visibility)

   package ASU renames Ada.Strings.Unbounded; -- UndCC_Valid (renames instead of use)

   procedure Localized_Use_Valid;

   procedure Package_Body_Use_Violation;

   -- Nested package: a plain use clause at a nested package's own spec
   -- level is just as non-localized as one at the top-level package.
   package Inner_Pkg is
      use Ada.Strings.Unbounded; -- UndCC_Violation
      procedure Show;
   end Inner_Pkg;

   -- Multi-name use clause: each named package gets its own use reference,
   -- and each is independently checked against the same scoping rule.
   use Ada.Text_IO, Ada.Strings.Unbounded; -- UndCC_Violation (both names)

   task type Counter is
      entry Increment;
      entry Reset;
   end Counter;

private

   -- Private part: still part of the package's own declarative region,
   -- so a use clause here is just as non-localized as one in the visible
   -- part.
   use Ada.Strings.Unbounded; -- UndCC_Violation

end Test_5_7_1;
