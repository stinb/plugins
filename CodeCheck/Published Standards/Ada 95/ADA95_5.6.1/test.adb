package body Test_5_6_1 is

   procedure Five_Levels (C1, C2, C3, C4, C5 : Boolean; X : Integer) is
   begin
      -- Valid: exactly 5 levels of nesting
      if C1 then                       -- level 1
         if C2 then                    -- level 2
            for I in 1 .. 10 loop     -- level 3
               case X is              -- level 4
                  when 1 =>
                     if C5 then       -- level 5 (valid)
                        null;
                     end if;
                  when others =>
                     null;
               end case;
            end loop;
         end if;
      end if;
   end Five_Levels;

   procedure Six_Levels (C1, C2, C3, C4, C5, C6 : Boolean; X : Integer) is
   begin
      if C1 then                       -- level 1
         if C2 then                    -- level 2
            for I in 1 .. 10 loop     -- level 3
               case X is              -- level 4
                  when 1 =>
                     if C5 then       -- level 5
                        if C6 then   -- level 6 (violation) -- UndCC_Violation
                           null;
                        end if;
                     end if;
                  when others =>
                     null;
               end case;
            end loop;
         end if;
      end if;
   end Six_Levels;

   procedure Elsif_Does_Not_Add_Level (C1, C2, C3 : Boolean) is
   begin
      -- Valid: elsif stays at same level as outer if
      if C1 then         -- level 1
         if C2 then      -- level 2
            null;
         elsif C3 then   -- still level 2, not a new level
            null;
         else
            null;
         end if;
      end if;
   end Elsif_Does_Not_Add_Level;

   procedure Nested_Loops (C1 : Boolean) is
   begin
      -- Violation: six loop levels
      for A in 1 .. 2 loop             -- level 1
         for B in 1 .. 2 loop          -- level 2
            for C in 1 .. 2 loop       -- level 3
               for D in 1 .. 2 loop    -- level 4
                  for E in 1 .. 2 loop -- level 5
                     for F in 1 .. 2 loop -- level 6 (violation) -- UndCC_Violation
                        null;
                     end loop;
                  end loop;
               end loop;
            end loop;
         end loop;
      end loop;
   end Nested_Loops;

   procedure Four_Levels (C1, C2, C3, C4 : Boolean) is
   begin
      -- Valid: only 4 levels
      if C1 then        -- level 1
         if C2 then     -- level 2
            if C3 then  -- level 3
               if C4 then -- level 4
                  null;
               end if;
            end if;
         end if;
      end if;
   end Four_Levels;

   procedure While_Loop_Nesting (C : Boolean) is
   begin
      -- Valid: 5 levels using while loops
      while C loop                  -- level 1
         while C loop               -- level 2
            while C loop            -- level 3
               while C loop         -- level 4
                  while C loop      -- level 5 (valid)
                     null;
                  end loop;
               end loop;
            end loop;
         end loop;
      end loop;
   end While_Loop_Nesting;

   procedure Seven_Levels (C1, C2, C3, C4, C5, C6, C7 : Boolean) is
   begin
      -- Two consecutive violations at levels 6 and 7
      if C1 then             -- level 1
         if C2 then          -- level 2
            if C3 then       -- level 3
               if C4 then    -- level 4
                  if C5 then -- level 5
                     if C6 then -- level 6 (violation) -- UndCC_Violation
                        if C7 then -- level 7 (violation) -- UndCC_Violation
                           null;
                        end if;
                     end if;
                  end if;
               end if;
            end if;
         end if;
      end if;
   end Seven_Levels;

   procedure Violation_Then_Recovery (C1, C2, C3, C4, C5, C6 : Boolean) is
   begin
      -- First block: goes to level 6 (violation)
      if C1 then          -- level 1
         if C2 then       -- level 2
            if C3 then    -- level 3
               if C4 then -- level 4
                  if C5 then -- level 5
                     if C6 then -- level 6 (violation) -- UndCC_Violation
                        null;
                     end if;
                  end if;
               end if;
            end if;
         end if;
      end if;

      -- Second block in same procedure: depth resets to 0, reaches only 5 (valid)
      if C1 then          -- level 1
         if C2 then       -- level 2
            if C3 then    -- level 3
               if C4 then -- level 4
                  if C5 then -- level 5 (valid)
                     null;
                  end if;
               end if;
            end if;
         end if;
      end if;
   end Violation_Then_Recovery;

   procedure Single_If (C : Boolean) is
   begin
      -- Valid: single level
      if C then
         null;
      end if;
   end Single_If;

   procedure Conditional_Expression_Not_Counted
     (C1, C2, C3, C4, C5, C6 : Boolean; X, Y : Integer)
   is
      Z : Integer;
   begin
      -- Ada 2012 conditional expression: (if ... then ... else ...)
      -- These are inside parens and must NOT count toward nesting depth
      Z :=
        (if C1
         then (if C2
               then (if C3
                     then (if C4
                           then (if C5
                                 then (if C6 then X else Y)
                                 else Y)
                           else Y)
                     else Y)
               else Y)
         else Y);

      -- Even with 6 levels of conditional expressions, no violation
      -- because they are inside parentheses
      if C1 then          -- level 1
         if C2 then       -- level 2
            if C3 then    -- level 3
               if C4 then -- level 4
                  if C5 then -- level 5 (valid, conditional exprs don't count)
                     null;
                  end if;
               end if;
            end if;
         end if;
      end if;
   end Conditional_Expression_Not_Counted;

   procedure Case_Expression_Not_Counted (X : Integer) is
      Y : Integer;
   begin
      -- Ada 2012 case expression inside parens: must NOT count
      Y :=
        (case X is
            when 1 => 10,
            when 2 => 20,
            when others => 0);

      -- Statement-level case still counts
      if X > 0 then          -- level 1
         if X > 1 then       -- level 2
            if X > 2 then    -- level 3
               if X > 3 then -- level 4
                  case X is  -- level 5 (valid)
                     when others => null;
                  end case;
               end if;
            end if;
         end if;
      end if;
   end Case_Expression_Not_Counted;

   procedure Mixed_Expr_And_Statement (C1, C2, C3, C4, C5 : Boolean; X : Integer) is
      Z : Integer;
   begin
      -- Conditional expression nested deep inside statement nesting
      if C1 then    -- level 1
         if C2 then -- level 2
            if C3 then -- level 3
               if C4 then -- level 4
                  if C5 then -- level 5
                     -- Conditional expression here does NOT add a 6th level
                     Z := (if X > 0 then X else -X);
                     null;
                  end if;
               end if;
            end if;
         end if;
      end if;
   end Mixed_Expr_And_Statement;

end Test_5_6_1;
