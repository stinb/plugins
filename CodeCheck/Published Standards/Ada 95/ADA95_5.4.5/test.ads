package Test_5_4_5 is

   type Integer_Access is access Integer;
   type Float_Access   is access Float;

   -- Violation: access variable with no explicit initializer
   P1 : Integer_Access;                                      -- UndCC_Violation

   -- Valid: explicit null initialization
   P2 : Integer_Access := null;                              -- UndCC_Valid

   -- Valid: initialized with an allocator
   P3 : Integer_Access := new Integer'(0);                   -- UndCC_Valid

   -- Valid: initialized by copying another access value
   P4 : Integer_Access := P2;                                -- UndCC_Valid

   -- Violation: second access type, no initializer
   P5 : Float_Access;                                        -- UndCC_Violation

   -- Valid: explicit null for float access
   P6 : Float_Access := null;                                -- UndCC_Valid

   -- Valid: non-access variables are not flagged (no initializer is fine)
   X : Integer := 0;                                         -- UndCC_Valid
   Y : Integer;                                              -- UndCC_Valid

   -- Violation: anonymous access type without initializer
   A1 : access Integer;                                      -- UndCC_Violation

   -- Valid: anonymous access type with explicit null
   A2 : access Integer := null;                              -- UndCC_Valid

   -- Violation: anonymous access-to-constant type without initializer
   A3 : access constant Integer;                             -- UndCC_Violation

   -- Valid: anonymous access-to-constant with explicit null
   A4 : access constant Integer := null;                     -- UndCC_Valid

   -- Valid: constant access variable — Ada requires an initializer on constants,
   -- so this can never be uninitialized
   PC : constant Integer_Access := null;                     -- UndCC_Valid

   -- Access-to-subprogram types (excluded from check — not data pointers)
   type Callback is access procedure (X : Integer);
   type Func_Ref is access function return Integer;

   -- Valid: access-to-procedure without initializer (not a data pointer)
   CB1 : Callback;                                           -- UndCC_Valid

   -- Valid: access-to-procedure with explicit null
   CB2 : Callback := null;                                   -- UndCC_Valid

   -- Valid: access-to-function without initializer (not a data pointer)
   FR1 : Func_Ref;                                           -- UndCC_Valid

   -- Valid: access-to-function with explicit null
   FR2 : Func_Ref := null;                                   -- UndCC_Valid

   -- Valid: record components with access-to-subprogram types (not data pointers)
   type Parse_State is
      record
         Handler : Callback;                                  -- UndCC_Valid
         Func    : Func_Ref;                                  -- UndCC_Valid
      end record;

   -- General access type (access all)
   type Gen_Int is access all Integer;

   -- Violation: general access variable without initializer
   G1 : Gen_Int;                                             -- UndCC_Violation

   -- Valid: general access variable with explicit null
   G2 : Gen_Int := null;                                     -- UndCC_Valid

   -- Record with access components (FalseNeg: record components are not reached
   -- by filerefs("Declare", "Object, Member") in Understand's Ada model)
   type Node;
   type Node_Access is access Node;

   type Node is
      record
         Value : Integer := 0;
         Next  : Node_Access;                                -- UndCC_Violation
      end record;

   type Initialized_Node is
      record
         Value : Integer := 0;
         Next  : Node_Access := null;                        -- UndCC_Valid
      end record;

   -- Record with mixed access and non-access components
   type Mixed is
      record
         Count : Integer := 0;                               -- UndCC_Valid
         Link  : Node_Access;                                -- UndCC_Violation
         Label : Float_Access := null;                       -- UndCC_Valid
      end record;

end Test_5_4_5;
