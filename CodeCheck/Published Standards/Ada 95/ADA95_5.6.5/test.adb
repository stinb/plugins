package body Test_5_6_5 is

   procedure If_Then_Exit_Violation (C : Boolean) is
   begin
      -- Violation: should be 'exit when C'
      loop
         if C then -- UndCC_Violation
            exit;
         end if;
      end loop;
   end If_Then_Exit_Violation;

   procedure Exit_When_Valid (C : Boolean) is
   begin
      -- Valid: already using preferred form
      loop
         exit when C;
      end loop;
   end Exit_When_Valid;

   procedure If_Then_Exit_With_Else_Valid (C : Boolean) is
   begin
      -- Valid: else branch prevents simplification to exit when
      loop
         if C then
            exit;
         else
            null;
         end if;
      end loop;
   end If_Then_Exit_With_Else_Valid;

   procedure If_Then_Exit_With_Elsif_Valid (C1, C2 : Boolean) is
   begin
      -- Valid: elsif branch prevents simplification
      loop
         if C1 then
            exit;
         elsif C2 then
            null;
         end if;
      end loop;
   end If_Then_Exit_With_Elsif_Valid;

   procedure If_Then_Multiple_Statements_Valid (C : Boolean) is
   begin
      -- Valid: if body has more than just exit
      loop
         if C then
            null;
            exit;
         end if;
      end loop;
   end If_Then_Multiple_Statements_Valid;

   procedure Multiple_Violations (C1, C2 : Boolean) is
   begin
      -- Both if-then-exit patterns are violations
      loop
         if C1 then -- UndCC_Violation
            exit;
         end if;
         if C2 then -- UndCC_Violation
            exit;
         end if;
      end loop;
   end Multiple_Violations;

   procedure And_Then_Condition_Violation (C1, C2 : Boolean) is
   begin
      -- Violation: condition uses 'and then' but pattern still applies
      loop
         if C1 and then C2 then -- UndCC_Violation
            exit;
         end if;
      end loop;
   end And_Then_Condition_Violation;

   procedure Exit_In_For_Loop_Violation (C : Boolean) is
   begin
      -- Violation: if-then-exit inside a for loop
      for I in 1 .. 10 loop
         if C then -- UndCC_Violation
            exit;
         end if;
      end loop;
   end Exit_In_For_Loop_Violation;

   procedure Named_Exit_In_If_Valid (C : Boolean) is
   begin
      -- Valid: named exit (exit Label) is not checked
      My_Loop :
      loop
         if C then
            exit My_Loop;
         end if;
      end loop My_Loop;
   end Named_Exit_In_If_Valid;

   procedure Unconditional_Exit_Valid is
   begin
      -- Valid: unconditional exit not inside if-then
      loop
         exit;
      end loop;
   end Unconditional_Exit_Valid;

   procedure Or_Else_Condition_Violation (C1, C2 : Boolean) is
   begin
      -- Violation: 'or else' condition still applies
      loop
         if C1 or else C2 then -- UndCC_Violation
            exit;
         end if;
      end loop;
   end Or_Else_Condition_Violation;

   procedure Not_Condition_Violation (C : Boolean) is
   begin
      -- Violation: negated condition still applies
      loop
         if not C then -- UndCC_Violation
            exit;
         end if;
      end loop;
   end Not_Condition_Violation;

   procedure Nested_If_Then_Exit_Violation (C1, C2 : Boolean) is
   begin
      -- Violation: inner if-then-exit is independently flagged
      loop
         if C1 then
            if C2 then -- UndCC_Violation
               exit;
            end if;
         end if;
      end loop;
   end Nested_If_Then_Exit_Violation;

   procedure Exit_When_Inside_If_Valid (C1, C2 : Boolean) is
   begin
      -- Valid: exit when inside if body is not the if-then-exit anti-pattern
      loop
         if C1 then
            exit when C2;
         end if;
      end loop;
   end Exit_When_Inside_If_Valid;

   procedure If_Then_Exit_In_While_Violation (C1, C2 : Boolean) is
   begin
      -- Violation: if-then-exit inside a while loop
      while C1 loop
         if C2 then -- UndCC_Violation
            exit;
         end if;
      end loop;
   end If_Then_Exit_In_While_Violation;

   procedure If_Then_Exit_In_Labeled_Loop_Violation (C : Boolean) is
   begin
      -- Violation: label on loop does not affect the pattern
      My_Loop :
      loop
         if C then -- UndCC_Violation
            exit;
         end if;
      end loop My_Loop;
   end If_Then_Exit_In_Labeled_Loop_Violation;

   procedure Exit_After_Statement_Valid (C : Boolean) is
   begin
      -- Valid: exit follows another statement so 'then' is not its predecessor
      loop
         if C then
            null;
         end if;
         exit;
      end loop;
   end Exit_After_Statement_Valid;

end Test_5_6_5;
