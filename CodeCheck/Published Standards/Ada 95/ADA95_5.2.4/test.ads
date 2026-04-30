package Test_5_2_4 is

   -- Valid: all parameters have explicit modes
   procedure No_Params;                                                                   -- UndCC_Valid
   procedure All_In (A : in Integer; B : in Integer);                                    -- UndCC_Valid
   procedure Mixed_Modes (A : in Integer; B : out Integer; C : in out Integer);          -- UndCC_Valid
   procedure Single_Out (A : out Integer);                                                -- UndCC_Valid
   function F_Explicit (A : in Integer; B : in Integer) return Integer;                  -- UndCC_Valid

   -- Valid: access parameters carry no mode keyword
   procedure Access_Param (A : access Integer; B : in Integer);                          -- UndCC_Valid

   -- Valid: grouped parameters with explicit mode
   procedure Grouped_Explicit (A, B : in Integer; C : out Integer);                     -- UndCC_Valid

   -- Valid: default value present alongside explicit mode
   procedure Default_With_Mode (A : in Integer := 0; B : out Integer);                  -- UndCC_Valid

   -- Valid: multiline, all modes explicit
   procedure Multi_Line_Valid (A : in Integer;                                            -- UndCC_Valid
                               B : out Integer; C : in out Integer);

   -- Violations: one or more parameters missing explicit mode
   procedure Single_No_Mode (A : Integer);                                               -- UndCC_Violation
   procedure First_Missing (A : Integer; B : in Integer);                                -- UndCC_Violation
   procedure Last_Missing (A : in Integer; B : Integer);                                 -- UndCC_Violation
   function F_No_Mode (A : Integer) return Integer;                                      -- UndCC_Violation

   -- Violation: each missing-mode parameter on its own line
   procedure All_Missing (A : Integer;                                                    -- UndCC_Violation
                          B : Integer);                                                   -- UndCC_Violation

   -- Violation: middle parameter missing mode in a three-param list
   procedure Middle_Missing (A : in Integer;
                             B : Integer;                                                 -- UndCC_Violation
                             C : out Integer);

   -- Violation: grouped params split across lines, each missing mode
   procedure Grouped_No_Mode (A,                                                          -- UndCC_Violation
                              B : Integer);                                               -- UndCC_Violation

   -- Violation: default value present but mode missing (colon vs := must not be confused)
   procedure Default_No_Mode (A : Integer := 0; B : out Integer);                       -- UndCC_Violation

   -- Violation: subtype parameter, missing mode
   procedure Subtype_No_Mode (A : Positive; B : out Integer);                           -- UndCC_Violation

   -- Violation: access param is fine, but adjacent regular param is missing mode
   procedure Access_Then_No_Mode (A : access Integer; B : Integer);                     -- UndCC_Violation

   -- Violation: function with multiple params both missing mode
   function F_Multi_No_Mode (A : Integer;                                                -- UndCC_Violation
                             B : Integer) return Integer;                                -- UndCC_Violation

   -- Violation: long list where only the last param is missing mode
   procedure Last_Of_Many (A : in Integer; B : in Integer;                              -- UndCC_Valid
                           C : out Integer; D : Integer);                               -- UndCC_Violation

   procedure Run_Tests;

end Test_5_2_4;
