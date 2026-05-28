package body Test_5_6_4 is

   procedure Exit_In_For_Loop (C : Boolean) is
   begin
      -- Violation (guideline 4): exit inside a for loop
      for I in 1 .. 10 loop
         exit when C; -- UndCC_Violation
      end loop;
   end Exit_In_For_Loop;

   procedure Exit_In_While_Loop (C : Boolean) is
   begin
      -- Violation (guideline 4): unconditional exit inside a while loop
      while C loop
         exit; -- UndCC_Violation
      end loop;
   end Exit_In_While_Loop;

   procedure Exit_In_Bare_Loop_Valid (C1, C2 : Boolean) is
   begin
      -- Valid: exit in a plain loop is the intended pattern
      loop
         exit when C1;
         exit when C2;
      end loop;
   end Exit_In_Bare_Loop_Valid;

   procedure Nested_Exit_Applies_To_Inner_Valid (C : Boolean) is
   begin
      -- Valid: exit applies to the inner bare loop, not the outer for loop
      for I in 1 .. 10 loop
         loop
            exit when C;
         end loop;
      end loop;
   end Nested_Exit_Applies_To_Inner_Valid;

   procedure For_Loop_No_Exit_Valid is
   begin
      -- Valid: for loop with no exit statement
      for I in 1 .. 10 loop
         null;
      end loop;
   end For_Loop_No_Exit_Valid;

   procedure While_Loop_No_Exit_Valid (C : Boolean) is
   begin
      -- Valid: while loop with no exit statement
      while C loop
         null;
      end loop;
   end While_Loop_No_Exit_Valid;

   procedure Three_Exits_Violation (C1, C2, C3 : Boolean) is
   begin
      -- Violation (guideline 5): three exits exceed the maximum of two
      loop
         exit when C1;
         exit when C2;
         exit when C3; -- UndCC_Violation
      end loop;
   end Three_Exits_Violation;

   procedure Two_Exits_Valid (C1, C2 : Boolean) is
   begin
      -- Valid: two exits are within the acceptable limit
      loop
         exit when C1;
         exit when C2;
      end loop;
   end Two_Exits_Valid;

   procedure One_Exit_Valid (C : Boolean) is
   begin
      -- Valid: single exit
      loop
         exit when C;
      end loop;
   end One_Exit_Valid;

   procedure For_Loop_Two_Exits_Both_Violations (C1, C2 : Boolean) is
   begin
      -- Both exits violate guideline 4; two exits stay within guideline 5
      for I in 1 .. 10 loop
         exit when C1; -- UndCC_Violation
         exit when C2; -- UndCC_Violation
      end loop;
   end For_Loop_Two_Exits_Both_Violations;

   procedure Nested_Loops_Exit_Counts_Independent (C1, C2, C3 : Boolean) is
   begin
      -- Exit counts are tracked independently per loop
      -- Outer loop has 2 exits (valid), inner loop has 3 exits (violation)
      loop
         loop
            exit when C1;
            exit when C2;
            exit when C3; -- UndCC_Violation (guideline 5, inner loop)
         end loop;
         exit when C1;
         exit when C2;
      end loop;
   end Nested_Loops_Exit_Counts_Independent;

   procedure Named_Exit_In_For_Loop (C : Boolean) is
   begin
      -- Named exit in a for loop targets that for loop: violation
      My_Loop :
      for I in 1 .. 10 loop
         exit My_Loop when C; -- UndCC_Violation
      end loop My_Loop;
   end Named_Exit_In_For_Loop;

   procedure Named_Exit_In_While_Loop (C : Boolean) is
   begin
      -- Named exit in a while loop targets that while loop: violation
      My_Loop :
      while C loop
         exit My_Loop; -- UndCC_Violation
      end loop My_Loop;
   end Named_Exit_In_While_Loop;

   procedure Named_Exit_From_Inner_To_Outer_For (C1, C2 : Boolean) is
   begin
      -- Named exit from inner bare loop exits the outer for loop: violation
      Outer :
      for I in 1 .. 10 loop
         loop
            exit Outer when C1; -- UndCC_Violation (targets for loop)
         end loop;
      end loop Outer;
   end Named_Exit_From_Inner_To_Outer_For;

   procedure Named_Exit_From_Inner_To_Outer_Bare (C1, C2 : Boolean) is
   begin
      -- Named exit from inner bare loop exits the outer bare loop: valid
      Outer :
      loop
         loop
            exit Outer when C1;
         end loop;
         exit when C2;
      end loop Outer;
   end Named_Exit_From_Inner_To_Outer_Bare;

   procedure Named_Exit_Count_Applies_To_Target_Loop (C1, C2, C3 : Boolean) is
   begin
      -- Named exits to the outer bare loop count against that loop's exit total
      Outer :
      loop
         loop
            exit Outer when C1;
            exit Outer when C2;
            exit Outer when C3; -- UndCC_Violation (3rd exit on Outer)
         end loop;
      end loop Outer;
   end Named_Exit_Count_Applies_To_Target_Loop;

   procedure Bare_In_While_Valid (C1, C2 : Boolean) is
   begin
      -- Valid: exit applies to the inner bare loop, not the enclosing while
      while C1 loop
         loop
            exit when C2;
         end loop;
      end loop;
   end Bare_In_While_Valid;

   procedure While_Two_Exits_Both_Violations (C1, C2 : Boolean) is
   begin
      -- Both exits violate guideline 4; two exits stay within guideline 5
      while C1 loop
         exit when C1; -- UndCC_Violation
         exit when C2; -- UndCC_Violation
      end loop;
   end While_Two_Exits_Both_Violations;

   procedure Inner_For_In_For_Violation (C : Boolean) is
   begin
      -- Violation: exit applies to the inner for loop (guideline 4)
      for I in 1 .. 10 loop
         for J in 1 .. 5 loop
            exit when C; -- UndCC_Violation
         end loop;
      end loop;
   end Inner_For_In_For_Violation;

   procedure Four_Exits_Two_Violations (C1, C2, C3, C4 : Boolean) is
   begin
      -- 3rd and 4th exits each violate guideline 5
      loop
         exit when C1;
         exit when C2;
         exit when C3; -- UndCC_Violation
         exit when C4; -- UndCC_Violation
      end loop;
   end Four_Exits_Two_Violations;

   procedure Labeled_Bare_Loop_Unnamed_Exit_Valid (C : Boolean) is
   begin
      -- Valid: loop has a label but the exit is unnamed, applies to bare loop
      My_Bare_Loop :
      loop
         exit when C;
      end loop My_Bare_Loop;
   end Labeled_Bare_Loop_Unnamed_Exit_Valid;

   procedure Unconditional_Exit_In_For_Violation (C : Boolean) is
   begin
      -- Violation: unconditional exit (no when clause) inside a for loop
      for I in 1 .. 10 loop
         if C then
            exit; -- UndCC_Violation
         end if;
      end loop;
   end Unconditional_Exit_In_For_Violation;

   procedure Nested_While_In_For_Exit_Violation (C1, C2 : Boolean) is
   begin
      -- Violation: exit targets the inner while loop (guideline 4)
      for I in 1 .. 10 loop
         while C1 loop
            exit when C2; -- UndCC_Violation (exit in while)
         end loop;
      end loop;
   end Nested_While_In_For_Exit_Violation;

end Test_5_6_4;
