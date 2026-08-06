package Test_5_7_2 is

   type Rational is private;

   function "+" (X, Y : Rational) return Rational;

   -- Violation: renaming an operator instead of providing a use type clause.
   function Add (X, Y : Rational) return Rational renames "+"; -- UndCC_Violation

   procedure Actual_Work (X, Y : Integer; Result : out Integer);
   function Actual_Work_Fn (X, Y : Integer) return Integer;

   -- Violation: pure pass-through wrapper (function form); should be a
   -- renaming declaration instead.
   function Forward_Fn (X, Y : Integer) return Integer;

   -- Violation: pure pass-through wrapper (procedure form).
   procedure Forward_Proc (X, Y : Integer; Result : out Integer);

   -- Valid: real logic, not a pass-through.
   function Real_Logic (X, Y : Integer) return Integer;

   -- Valid: forwards to another function, but transforms an argument first.
   function Transforms_Argument (X, Y : Integer) return Integer;

   -- Valid: forwards to another function, but reorders the arguments.
   function Reorders_Arguments (X, Y : Integer) return Integer;

   -- Valid: recursive call to itself is not delegation to another
   -- subprogram.
   function Factorial (N : Natural) return Natural;

   -- Valid: delegation done correctly, via a renaming declaration rather
   -- than a pass-through wrapper body.
   function Correct_Delegation (X, Y : Integer) return Integer renames Actual_Work_Fn;

   -- Violation: renaming one operator symbol as another operator symbol is
   -- still renaming an operator; a use type clause should be used instead.
   function "-" (X, Y : Rational) return Rational renames "+"; -- UndCC_Violation

   procedure Do_Nothing;
   function Actual_Get_Value return Integer;

   -- Violation: pure pass-through wrapper with no parameters at all (Ada
   -- has no parens for a parameterless call).
   function Forward_No_Params return Integer;

   -- Violation: pure pass-through wrapper (procedure form), no parameters.
   procedure Forward_Proc_No_Params;

   -- Valid: named parameter association, not a plain positional forward.
   function Named_Association (X, Y : Integer) return Integer;

   -- Valid: an extra statement (even a null statement) before the call.
   function Extra_Statement (X, Y : Integer) return Integer;

   -- Valid: has an exception handler, so the body is not a single bare
   -- statement.
   function Has_Exception_Handler (X, Y : Integer) return Integer;

   package Helper is
      function Actual_Work (X, Y : Integer) return Integer;
   end Helper;

   -- Violation: pure pass-through, but the call target is a qualified
   -- name (Package.Name) rather than a bare identifier.
   function Forward_Qualified (X, Y : Integer) return Integer;

   -- Valid: overloaded same-name functions. Combine/1 forwards to the
   -- different-signature Combine/2 with an extra argument supplied, so it
   -- is not a plain forward of its own parameters -- and the two
   -- overloads must not be mistaken for one recursive function.
   function Combine (X : Integer) return Integer;
   function Combine (X, Y : Integer) return Integer;

private

   type Rational is record
      Num : Integer;
      Den : Integer;
   end record;

end Test_5_7_2;
