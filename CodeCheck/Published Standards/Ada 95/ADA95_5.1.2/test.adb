-- Test file: uses the word "declare" in a comment to inflate raw count
package body Test_5_1_2 is

   -- Valid: non-nested unnamed block (top-level in procedure)
   procedure Top_Level_Block is
   begin
      declare                                  -- UndCC_Valid
         X : Integer := 0;
      begin
         X := 1;
      end;
   end Top_Level_Block;

   -- Valid: named outer and named inner blocks
   procedure Named_Nested is
   begin
      Outer:
         declare                               -- UndCC_Valid
            X : Integer := 0;
         begin
            Inner:
               declare                        -- UndCC_Valid
                  Y : Integer := 0;
               begin
                  Y := X;
               end Inner;
         end Outer;
   end Named_Nested;

   -- Violation: named outer, unnamed inner block
   procedure Unnamed_Nested is
   begin
      Outer:
         declare                               -- UndCC_Valid
            X : Integer := 0;
         begin
            declare                           -- UndCC_Violation
               Y : Integer := 0;
            begin
               Y := X;
            end;
         end Outer;
   end Unnamed_Nested;

   -- Valid: triply nested blocks, all named
   procedure Triple_Named is
   begin
      A:
         declare                              -- UndCC_Valid
         begin
            B:
               declare                       -- UndCC_Valid
               begin
                  C:
                     declare                 -- UndCC_Valid
                     begin
                        null;
                     end C;
               end B;
         end A;
   end Triple_Named;

   -- Violation: triply nested blocks, innermost unnamed
   procedure Triple_Unnamed_Inner is
   begin
      A:
         declare                              -- UndCC_Valid
         begin
            B:
               declare                       -- UndCC_Valid
               begin
                  declare                    -- UndCC_Violation
                  begin
                     null;
                  end;
               end B;
         end A;
   end Triple_Unnamed_Inner;

   -- "declare" in string literal, identifier, and embedded substring
   procedure Declare_In_Various is
      Declared_Flag : Boolean := False;       -- "declare" as substring in identifier
      Msg           : String  := "declare";   -- "declare" as string literal value
   begin
      -- declare appears here in a comment
      Declared_Flag := True;
      null;
   end Declare_In_Various;

end Test_5_1_2;
