package body Test is

   procedure Do_Something (Arr : in out String; Idx : Integer) is
      pragma Suppress (Overflow_Check);  -- UndCC_Violation
      X : Integer;
   begin
      X := Idx + 1;

      declare
         pragma Suppress (Index_Check);  -- UndCC_Valid
      begin
         Arr (Idx) := 'A';
      end;
   end Do_Something;

   -- A nested subprogram declared before the pragma, whether or not it
   -- has its own internal declare block, must not be mistaken for an
   -- enclosing scope of the pragma that follows it.
   package More_Cases is
      procedure Foo;
   end More_Cases;

   package body More_Cases is

      procedure Foo is
         procedure Bar is
         begin
            declare
               Y : Integer;
            begin
               null;
            end;
         end Bar;
         pragma Suppress (Overflow_Check);  -- UndCC_Violation
      begin
         Bar;

         declare
            pragma Suppress (Index_Check);  -- UndCC_Valid
         begin
            null;
         end;
      end Foo;

   end More_Cases;

   -- Case-insensitive keywords, and a doubly-nested declare block, must
   -- still resolve correctly.
   procedure Nested_Blocks is
   begin
      declare
      begin
         declare
            PRAGMA SUPPRESS (Index_Check);  -- UndCC_Valid
         begin
            null;
         end;
      end;
   end Nested_Blocks;

end Test;
