package body Test_7_2_10 is

   procedure Do_Thing (X : Integer) is
   begin
      null;
   end Do_Thing;

   function Compute (X : Integer) return Integer is
   begin
      return X;
   end Compute;

   function Compare_CB return Boolean is
   begin
      return CB1 = CB2; -- UndCC_Violation
   end Compare_CB;

   -- A comparison against the literal null is not the hazard this
   -- guideline names (Ada defines it unambiguously), so it is not
   -- flagged; this also regression-tests that a second use of the same
   -- access-to-procedure object in the file still resolves correctly.
   function Compare_CB_Null return Boolean is
   begin
      return CB1 = null; -- UndCC_Valid
   end Compare_CB_Null;

   -- A named constant that is itself initialized to null is just null
   -- under an alias, so comparing against it is not flagged either.
   function Compare_CB_Ignore return Boolean is
   begin
      return CB1 /= Ignore; -- UndCC_Valid
   end Compare_CB_Ignore;

   function Compare_Obj return Boolean is
   begin
      return O1 = O2; -- UndCC_Valid
   end Compare_Obj;

   -- Known gap: comparing two distinct access-to-function objects
   -- directly to each other is not resolved by Understand (neither
   -- operand's entity is found), unlike comparing access-to-procedure
   -- objects to each other (see Compare_CB above) or comparing an
   -- access-to-function object against null (see below), both of which
   -- work correctly.
   function Compare_FP_Ne return Boolean is
   begin
      return FP1 = FP2; -- UndCC_FalseNeg
   end Compare_FP_Ne;

   function Compare_FP_Null return Boolean is
   begin
      return FP1 = null; -- UndCC_Valid
   end Compare_FP_Null;

   -- Two direct, separate 'Access evaluations compared to each other is
   -- the literal hazard the guideline's own Rationale quotes (RM 4.5.2:
   -- "distinct evaluations of Access attribute references").
   function Compare_Direct_Access return Boolean is
   begin
      return Do_Thing'Access = Do_Thing'Access; -- UndCC_Violation
   end Compare_Direct_Access;

   -- Accepted scope boundary, not a gap: a rename of a null-valued
   -- constant is not traced back to the thing it renames, so Also_Ignore
   -- is treated as an ordinary access-to-subprogram value and this is
   -- flagged like any other such comparison.
   function Compare_CB_Rename return Boolean is
   begin
      return CB1 /= Also_Ignore; -- UndCC_Violation
   end Compare_CB_Rename;

   -- A multi-name constant declaration ("A, B : constant ... := null;")
   -- is still recognized: the last name before the colon (B here) is
   -- what the scan matches, and only one operand needs to match for the
   -- whole comparison to be excluded.
   function Compare_Multi_Const return Boolean is
   begin
      return A = B; -- UndCC_Valid
   end Compare_Multi_Const;

end Test_7_2_10;
