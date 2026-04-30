package body Test_5_1_3 is

   -- Valid: single (non-nested) loop, exit without name is fine
   procedure Single_Loop_Unnamed_Exit is
   begin
      loop
         exit;                                -- UndCC_Valid
      end loop;
   end Single_Loop_Unnamed_Exit;

   -- Valid: nested loops, all exits use loop names
   procedure Named_Nested_Named_Exits is
   begin
      Outer:
         loop
            Inner:
               loop
                  exit Inner;                -- UndCC_Valid
               end loop Inner;
            exit Outer;                      -- UndCC_Valid
         end loop Outer;
   end Named_Nested_Named_Exits;

   -- Violation: nested loops, inner exit has no loop name
   procedure Named_Nested_Unnamed_Exit is
   begin
      Outer:
         loop
            Inner:
               loop
                  exit;                      -- UndCC_Violation
               end loop Inner;
            exit Outer;                      -- UndCC_Valid
         end loop Outer;
   end Named_Nested_Unnamed_Exit;

   -- Violation: nested loops, exit when without loop name
   procedure Named_Nested_Exit_When is
      Done : Boolean := False;
   begin
      Outer:
         loop
            Inner:
               loop
                  exit when Done;            -- UndCC_Violation
               end loop Inner;
            exit Outer when Done;            -- UndCC_Valid
         end loop Outer;
   end Named_Nested_Exit_When;

   -- Valid: triple nested, all exits use loop names
   procedure Triple_Nested_All_Named is
   begin
      A:
         loop
            B:
               loop
                  C:
                     loop
                        exit C;             -- UndCC_Valid
                     end loop C;
                  exit B;                   -- UndCC_Valid
               end loop B;
            exit A;                         -- UndCC_Valid
         end loop A;
   end Triple_Nested_All_Named;

   -- Violation: triple nested, middle loop exit has no name
   procedure Triple_Nested_Unnamed_Exit is
   begin
      A:
         loop
            B:
               loop
                  C:
                     loop
                        exit C;             -- UndCC_Valid
                     end loop C;
                  exit;                     -- UndCC_Violation
               end loop B;
            exit A;                         -- UndCC_Valid
         end loop A;
   end Triple_Nested_Unnamed_Exit;

end Test_5_1_3;
