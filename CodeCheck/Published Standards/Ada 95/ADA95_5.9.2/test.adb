package body Test is

   procedure Body_Free is new Ada.Unchecked_Deallocation (Object => Rec, Name => Rec_Access);  -- UndCC_Valid

   procedure Do_Something is
      P : Rec_Access := new Rec;
   begin
      Body_Free (P);
      null;
   end Do_Something;

   -- A rename of a body-declared instantiation, also declared in the
   -- body, stays fully isolated and must not be flagged.
   procedure Body_Alias (X : in out Rec_Access) renames Body_Free;  -- UndCC_Valid

end Test;
