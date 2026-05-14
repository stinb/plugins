package Test_5_4_6 is

   -- Valid: aliased constant strings ('Access not required — constants exempt)
   Msg_Ok       : aliased constant String := "OK";           -- UndCC_Valid
   Msg_Shutdown : aliased constant String := "Shutdown";     -- UndCC_Valid

   -- Violation: aliased integer, 'Access never taken
   Counter : aliased Integer := 0;                           -- UndCC_Violation

   -- Valid: regular (non-aliased) variable
   Regular : Integer := 0;                                   -- UndCC_Valid

   -- Valid: regular constant (no aliased keyword)
   Max : constant Integer := 100;                            -- UndCC_Valid

   -- Violation: aliased user-defined integer subtype, 'Access never taken
   type Sensor_Reading is range 0 .. 1023;
   Current_Reading : aliased Sensor_Reading := 0;            -- UndCC_Violation

   -- Violation: aliased float, 'Access never taken
   Pi_Approx : aliased Float := 3.14159;                     -- UndCC_Violation

   -- Violation: aliased Boolean, 'Access never taken
   Flag : aliased Boolean := False;                          -- UndCC_Violation

   -- Violation: aliased enumeration, 'Access never taken
   type Color is (Red, Green, Blue);
   Current_Color : aliased Color := Red;                     -- UndCC_Violation

   -- Violation: aliased access-type variable, 'Access never taken
   type Int_Access is access Integer;
   Ptr : aliased Int_Access := null;                         -- UndCC_Violation

   -- Violation: aliased array variable, 'Access never taken
   type Buffer is array (1 .. 80) of Character;
   Shared_Buffer : aliased Buffer;                           -- UndCC_Violation

   -- Valid: aliased constant array (constants exempt)
   type Lookup_Table is array (1 .. 4) of Integer;
   Primes : aliased constant Lookup_Table := (2, 3, 5, 7);  -- UndCC_Valid

   -- Valid: aliased constant scalar subtype (constants exempt)
   type Priority is range 1 .. 10;
   Max_Priority : aliased constant Priority := 10;           -- UndCC_Valid

   -- Valid: aliased record components not flagged (style guide endorses this pattern)
   type Cell is
      record
         Value : aliased Integer;                            -- UndCC_Valid
         Next  : Integer;                                    -- UndCC_Valid
      end record;

   -- Valid: non-aliased record components
   type Plain_Record is
      record
         X : Integer;                                        -- UndCC_Valid
         Y : Integer;                                        -- UndCC_Valid
      end record;

   -- Valid: ragged array pattern — 'Access IS taken to populate the row table
   type Row is array (Integer range <>) of Integer;
   type Row_Ptr is access all Row;
   Row_Short : aliased Row (1 .. 4);                         -- UndCC_Valid
   Row_Long  : aliased Row (1 .. 10);                        -- UndCC_Valid
   type Row_Table_T is array (1 .. 2) of Row_Ptr;
   Row_Table : Row_Table_T := (Row_Short'Access, Row_Long'Access);

   -- Violation: aliased standalone variable of a record type, 'Access never taken
   C1 : aliased Cell;                                        -- UndCC_Violation

   -- Valid: aliased constant of a record type (constants exempt)
   C2 : aliased constant Cell := (Value => 0, Next => 0);    -- UndCC_Valid

   -- 'Access-taken examples using general access types
   type Int_All_Ptr   is access all Integer;
   type Float_All_Ptr is access all Float;

   -- Valid: aliased integer whose 'Access IS taken (used in pointer initializer)
   Active_Int : aliased Integer := 0;                        -- UndCC_Valid
   Int_Ref    : Int_All_Ptr := Active_Int'Access;

   -- Violation: aliased integer whose 'Access is NEVER taken
   Idle_Int : aliased Integer := 0;                          -- UndCC_Violation

   -- Valid: aliased float whose 'Access IS taken
   Active_Float : aliased Float := 0.0;                      -- UndCC_Valid
   Float_Ref    : Float_All_Ptr := Active_Float'Access;

   -- Violation: aliased float whose 'Access is NEVER taken
   Idle_Float : aliased Float := 0.0;                        -- UndCC_Violation

   -- Nested package: mixed aliased variables, some with 'Access taken
   package Inner is
      type Inner_Int_Ptr is access all Integer;

      -- Valid: 'Access IS taken
      Inner_Active : aliased Integer := 0;                   -- UndCC_Valid
      Inner_Ref    : Inner_Int_Ptr := Inner_Active'Access;

      -- Violation: 'Access never taken
      Inner_Idle : aliased Integer := 0;                     -- UndCC_Violation

      -- Valid: aliased constant (constants exempt)
      Inner_Const : aliased constant Integer := 1;           -- UndCC_Valid
   end Inner;

private

   -- Violation: aliased variable in private section, 'Access never taken
   Internal_State : aliased Integer := 0;                    -- UndCC_Violation

   -- Valid: aliased variable in private section whose 'Access IS taken
   type Priv_Int_Ptr is access all Integer;
   Private_Active : aliased Integer := 0;                    -- UndCC_Valid
   Private_Ref    : Priv_Int_Ptr := Private_Active'Access;

end Test_5_4_6;
