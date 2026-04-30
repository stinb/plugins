package body Test_5_1_4 is                               -- UndCC_Valid

   -- Valid: procedure end includes designator
   procedure Named_End_Procedure is
   begin
      null;
   end Named_End_Procedure;                              -- UndCC_Valid

   -- Violation: procedure end omits designator
   procedure Unnamed_End_Procedure is
   begin
      null;
   end;                                                  -- UndCC_Violation

   -- Valid: function end includes designator
   function Named_End_Function return Integer is
   begin
      return 0;
   end Named_End_Function;                               -- UndCC_Valid

   -- Violation: function end omits designator
   function Unnamed_End_Function return Integer is
   begin
      return 0;
   end;                                                  -- UndCC_Violation

   -- Valid: task body end includes defining identifier
   task body Named_End_Task is
   begin
      accept Start;
   end Named_End_Task;                                   -- UndCC_Valid

   -- Violation: task body end omits defining identifier
   task body Unnamed_End_Task is
   begin
      accept Start;
   end;                                                  -- UndCC_Violation

   -- Valid: accept statement end includes entry identifier
   task body Named_Accept_Task is
   begin
      accept Reset do
         null;
      end Reset;                                         -- UndCC_Valid
   end Named_Accept_Task;                                -- UndCC_Valid

   -- Violation: accept statement end omits entry identifier
   task body Unnamed_Accept_Task is
   begin
      accept Reset do
         null;
      end;                                               -- UndCC_Violation
   end Unnamed_Accept_Task;                              -- UndCC_Valid

   -- Valid: protected body end includes defining identifier
   protected body Named_End_Protected is
      procedure Set (Value : in Integer) is
      begin
         Data := Value;
      end Set;                                           -- UndCC_Valid
   end Named_End_Protected;                              -- UndCC_Valid

   -- Violation: protected body end omits defining identifier
   protected body Unnamed_End_Protected is
      procedure Set (Value : in Integer) is
      begin
         Data := Value;
      end Set;                                           -- UndCC_Valid
   end;                                                  -- UndCC_Violation

   -- Violation: nested package end omits program unit name
   package Inner is
      X : Integer := 0;
   end;                                                     -- UndCC_Violation

   -- Valid: nested package end includes program unit name
   package Outer is
      Y : Integer := 0;
   end Outer;                                               -- UndCC_Valid

   -- Valid: operator function end includes operator symbol (string literal designator)
   function "+" (Left, Right : My_Int) return My_Int is
   begin
      return My_Int (Integer (Left) + Integer (Right));
   end "+";                                                 -- UndCC_Valid

   -- Violation: operator function end omits operator symbol
   function "-" (Left, Right : My_Int) return My_Int is
   begin
      return My_Int (Integer (Left) - Integer (Right));
   end;                                                     -- UndCC_Violation

   -- Valid: keyword-as-string operator end includes operator symbol
   function "and" (Left, Right : My_Int) return My_Int is
   begin
      return My_Int (Integer (Left) and Integer (Right));
   end "and";                                               -- UndCC_Valid

   -- Violation: keyword-as-string operator end omits operator symbol
   function "not" (Right : My_Int) return My_Int is
   begin
      return My_Int (not Integer (Right));
   end;                                                     -- UndCC_Violation

   -- Not flagged: declare block end (block statements are 5.1.2 territory)
   procedure Block_With_Declare is
   begin
      declare
         X : Integer := 0;
      begin
         X := 1;
      end;                                                  -- UndCC_Valid
      Labeled_Block:
         declare
            Y : Integer := 0;
         begin
            Y := 1;
         end Labeled_Block;                                 -- UndCC_Valid
   end Block_With_Declare;                                  -- UndCC_Valid

   -- Not flagged: label-only block end (no declare section)
   procedure Block_With_Label is
   begin
      Outer_Block:
         begin
            Inner_Block:
               begin
                  null;
               end Inner_Block;                             -- UndCC_Valid
         end Outer_Block;                                   -- UndCC_Valid
   end Block_With_Label;                                    -- UndCC_Valid

end Test_5_1_4;                                            -- UndCC_Valid
