package body Test_5_1_1 is

   -- Valid: named outer and inner loops with named exits
   procedure Named_Loops is
   begin
      Outer:
         loop                                        -- UndCC_Valid
            Inner:
               loop                                 -- UndCC_Valid
                  exit Inner;
               end loop Inner;
            exit Outer;
         end loop Outer;
   end Named_Loops;

   -- Violation: unnamed loop containing exit
   procedure Unnamed_With_Exit is
   begin
      loop                                           -- UndCC_Valid
         exit;                                       -- UndCC_Violation
      end loop;
   end Unnamed_With_Exit;

   -- Violation: unnamed nested loop
   procedure Unnamed_Nested is
   begin
      Outer2:
         loop                                        -- UndCC_Valid
            loop                                    -- UndCC_Violation
               null;
            end loop;
            exit Outer2;
         end loop Outer2;
   end Unnamed_Nested;

   -- Valid: named loop with exit
   procedure Named_With_Exit is
   begin
      My_Loop:
         loop                                        -- UndCC_Valid
            exit My_Loop;
         end loop My_Loop;
   end Named_With_Exit;

   -- Violation: unnamed nested while loop
   procedure Unnamed_While_Nested is
      Done : Boolean := False;
   begin
      Outer3:
         loop                                        -- UndCC_Valid
            while not Done loop                     -- UndCC_Violation
               Done := True;
            end loop;
            exit Outer3;
         end loop Outer3;
   end Unnamed_While_Nested;

   -- Valid: named nested while loop
   procedure Named_While_Nested is
      Done : Boolean := False;
   begin
      Outer4:
         loop                                        -- UndCC_Valid
            Inner4:
               while not Done loop                  -- UndCC_Valid
                  Done := True;
               end loop Inner4;
            exit Outer4;
         end loop Outer4;
   end Named_While_Nested;

   -- Violation: unnamed nested for loop
   procedure Unnamed_For_Nested is
   begin
      Outer5:
         loop                                        -- UndCC_Valid
            for I in 1 .. 10 loop                   -- UndCC_Violation
               null;
            end loop;
            exit Outer5;
         end loop Outer5;
   end Unnamed_For_Nested;

   -- Valid: named nested for loop
   procedure Named_For_Nested is
   begin
      Outer6:
         loop                                        -- UndCC_Valid
            Inner6:
               for I in 1 .. 10 loop               -- UndCC_Valid
                  null;
               end loop Inner6;
            exit Outer6;
         end loop Outer6;
   end Named_For_Nested;

   -- Violation: unnamed while loop with exit
   procedure Unnamed_While_Exit is
      Done : Boolean := False;
   begin
      while not Done loop                            -- UndCC_Valid
         exit;                                       -- UndCC_Violation
      end loop;
   end Unnamed_While_Exit;

   -- Violation: unnamed for loop with exit
   procedure Unnamed_For_Exit is
   begin
      for I in 1 .. 10 loop                         -- UndCC_Valid
         exit;                                       -- UndCC_Violation
      end loop;
   end Unnamed_For_Exit;

end Test_5_1_1;
