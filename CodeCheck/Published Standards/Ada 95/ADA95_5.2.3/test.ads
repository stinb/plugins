package Test_5_2_3 is

   -- Valid: parameterless or single parameter
   procedure No_Params;                                                                    -- UndCC_Valid
   procedure Single_No_Default (A : in Integer);                                          -- UndCC_Valid
   procedure Single_With_Default (A : in Integer := 0);                                   -- UndCC_Valid

   -- Valid: no defaults anywhere
   procedure No_Defaults (A : in Integer; B : in Integer);                                -- UndCC_Valid

   -- Valid: all parameters have defaults
   procedure All_Defaults (A : in Integer := 0; B : in Integer := 0);                    -- UndCC_Valid
   procedure All_Defaults_Three (A : in Integer := 0; B : in Integer := 0;               -- UndCC_Valid
                                 C : in Integer := 0);

   -- Valid: defaults placed at the end
   procedure Default_At_End (A : in Integer; B : in Integer := 0);                       -- UndCC_Valid
   procedure Two_Defaults_At_End (A : in Integer;                                         -- UndCC_Valid
                                  B : in Integer := 0; C : in Integer := 0);
   procedure Three_Then_Two_Defaults (A : in Integer; B : in Integer; C : in Integer;    -- UndCC_Valid
                                      D : in Integer := 0; E : in Integer := 0);
   function F_Valid (A : in Integer; B : in Integer := 0) return Integer;                 -- UndCC_Valid

   -- Valid: grouped params with defaults at end
   procedure Grouped_No_Default (A, B : in Integer; C, D : in Integer := 0);             -- UndCC_Valid

   -- Valid: in out param (cannot have a default) before an in param with default
   procedure Mode_Before_Default (A : in out Integer; B : in Integer := 0);              -- UndCC_Valid

   -- Violations: a default parameter is followed by a non-default parameter
   procedure Default_Before_NonDefault (A : in Integer := 0; B : in Integer);            -- UndCC_Violation
   procedure Default_In_Middle (A : in Integer;                                           -- UndCC_Violation
                                B : in Integer := 0; C : in Integer);
   procedure Default_First_Of_Three (A : in Integer := 0;                                -- UndCC_Violation
                                     B : in Integer := 0; C : in Integer);
   function F_Violation (A : in Integer := 0; B : in Integer) return Integer;            -- UndCC_Violation

   -- Violation: grouped params where the group with defaults comes before a non-default
   procedure Grouped_Default_Before (A, B : in Integer := 0; C : in Integer);           -- UndCC_Violation

   -- Violation: interleaved — non-default appears between two default params
   procedure Interleaved (A : in Integer; B : in Integer := 0;                           -- UndCC_Violation
                          C : in Integer; D : in Integer := 0);

   -- Violation: default before an in out param (in out cannot have a default)
   procedure Default_Before_Out (A : in Integer := 0; B : in out Integer);              -- UndCC_Violation

   -- Violation: violation deep in a long list (6th param has no default after defaults)
   procedure Deep_Violation (A : in Integer; B : in Integer; C : in Integer;            -- UndCC_Violation
                             D : in Integer := 0; E : in Integer := 0; F : in Integer);

   procedure Run_Tests;

end Test_5_2_3;
