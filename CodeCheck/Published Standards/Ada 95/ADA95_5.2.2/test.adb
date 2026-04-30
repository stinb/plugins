package body Test_5_2_2 is

   procedure No_Params is null;
   procedure One_Param (A : in Integer) is null;
   procedure Two_Params (A : in Integer; B : in Integer) is null;
   procedure Three_Params (A : in Integer; B : in Integer; C : in Integer) is null;
   procedure Four_Params (A : in Integer; B : in Integer;
                          C : in Integer; D : in Integer) is null;
   procedure Frequent_One (A : in Integer) is null;
   procedure With_Default (A : in Integer; B : in Integer := 0) is null;
   procedure All_Optional (A : in Integer := 0; B : in Integer := 0) is null;

   function F_Two (A : in Integer; B : in Integer) return Integer is
   begin
      return A + B;
   end F_Two;

   procedure Run_Tests is
      X : Integer := 1;
      Y : Integer := 2;
   begin

      -- ERR1: more than two formal parameters, no named association
      Three_Params (1, 2, 3);                             -- UndCC_Violation
      Three_Params (A => 1, B => 2, C => 3);              -- UndCC_Valid
      Four_Params (1, 2, 3, 4);                           -- UndCC_Violation
      Four_Params (A => 1, B => 2, C => 3, D => 4);      -- UndCC_Valid
      Three_Params (1, B => 2, C => 3);                   -- UndCC_Violation
      Three_Params (1, 2, C => 3);                        -- UndCC_Violation

      -- Nested call: inner commas must not inflate the top-level argument count
      Three_Params (F_Two (A => X, B => Y), 2, 3);       -- UndCC_Violation
      Three_Params (A => F_Two (A => X, B => Y), B => 2, C => 3); -- UndCC_Valid

      -- ERR3: called less than 5 times in file, no named association
      One_Param (X);                                      -- UndCC_Violation
      One_Param (A => X);                                 -- UndCC_Valid
      Two_Params (X, Y);                                  -- UndCC_Violation
      Two_Params (X, B => Y);                             -- UndCC_Violation
      Two_Params (A => X, B => Y);                        -- UndCC_Valid

      -- Parameterless call: scanner must stop at semicolon, not steal the next statement's parens
      No_Params;                                          -- UndCC_Valid
      Three_Params (1, 2, 3);                             -- UndCC_Violation

      -- ERR4: actual parameter is a literal (Frequent_One called >= 5 times, so ERR3 does not apply)
      Frequent_One (X);                                   -- UndCC_Valid
      Frequent_One (X);                                   -- UndCC_Valid
      Frequent_One (X);                                   -- UndCC_Valid
      Frequent_One (X);                                   -- UndCC_Valid
      Frequent_One (X);                                   -- UndCC_Valid
      Frequent_One (1);                                   -- UndCC_Violation
      Frequent_One (A => 1);                              -- UndCC_Valid

      -- ERR5: positional arg supplied for an optional parameter
      With_Default (A => X, B => Y);                      -- UndCC_Valid
      With_Default (A => X, B => Y);                      -- UndCC_Valid
      With_Default (A => X, B => Y);                      -- UndCC_Valid
      With_Default (A => X, B => Y);                      -- UndCC_Valid
      With_Default (X, Y);                                -- UndCC_Violation (B is optional, positional)
      With_Default (X, B => Y);                           -- UndCC_Valid (A has no default, B is named)

      -- All params optional (All_Optional called >= 5 times)
      All_Optional;                                       -- UndCC_Valid (no args, all defaulted)
      All_Optional (X);                                   -- UndCC_Violation (A has default, positional)
      All_Optional (X, Y);                                -- UndCC_Violation (both optional, positional)
      All_Optional (A => X);                              -- UndCC_Valid
      All_Optional (A => X, B => Y);                      -- UndCC_Valid

   end Run_Tests;

end Test_5_2_2;
