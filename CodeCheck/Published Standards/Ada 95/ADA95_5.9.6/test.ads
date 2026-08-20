package Test is

   type Position is record
      X : Integer;
   end record;

   function Sense return Position;
   function Known_First return Position;

   -- A spec-level constant whose function's body lives only in the
   -- corresponding package body is unconditionally unsafe: the spec
   -- always fully elaborates before the body does.
   Bad_Cross_File_Const : constant Position := Sense;  -- UndCC_Violation

end Test;
