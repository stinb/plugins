-- Test file for ADA95_3.1.2: Numbers
-- Rules: uppercase E in scientific notation; uppercase hex digits in based literals.

procedure Test_3_1_2 is

   -- Valid: uppercase E in scientific notation
   Avogadro   : constant := 6.02216_9E+23;           -- UndCC_Valid
   Small      : constant := 1.5E-10;                  -- UndCC_Valid

   -- Violation: lowercase e in scientific notation
   Bad_Sci_1  : constant := 6.02216_9e+23;            -- UndCC_Violation
   Bad_Sci_2  : constant := 1.5e-10;                  -- UndCC_Violation

   -- Valid: uppercase hex digits in based literals
   Hex_Valid  : constant := 16#FFFF#;                 -- UndCC_Valid
   Hex_Valid2 : constant := 16#CAFE_BABE#;            -- UndCC_Valid
   Oct_Valid  : constant := 8#777#;                   -- UndCC_Valid

   -- Violation: lowercase hex digits in based literals
   Hex_Bad_1  : constant := 16#ffff#;                 -- UndCC_Violation
   Hex_Bad_2  : constant := 16#cafe_babe#;            -- UndCC_Violation
   Hex_Mixed  : constant := 16#CafE#;                 -- UndCC_Violation

   -- Valid: 'e' in string literals should not be flagged (different token type)
   Str_1 : constant String := "1.5e10 is not a number here";  -- UndCC_Valid
   Str_2 : constant String := "cafe babe";                    -- UndCC_Valid
   Str_3 : constant String := "6.022e+23";                    -- UndCC_Valid

   -- Valid: 'e' in a character literal should not be flagged
   Char_1 : constant Character := 'e';                        -- UndCC_Valid

   -- Valid: identifiers containing 'e' are not literals
   Temperature : constant Integer := 100;                     -- UndCC_Valid
   Electron    : constant Float   := 1.6E-19;                 -- UndCC_Valid

   -- Valid: based literal with no alphabetic digits (no case issue)
   Bin_Valid   : constant := 2#1010_1010#;                    -- UndCC_Valid
   Oct_Valid2  : constant := 8#777_000#;                      -- UndCC_Valid

begin
   null;
end Test_3_1_2;
