package Test_5_1_4 is                                    -- UndCC_Valid

   -- Subprogram declarations (bodies in .adb)
   procedure Named_End_Procedure;
   procedure Unnamed_End_Procedure;
   function Named_End_Function return Integer;
   function Unnamed_End_Function return Integer;
   procedure Block_With_Declare;
   procedure Block_With_Label;

   -- Operator overloading (designator is a string literal)
   type My_Int is range 0 .. 100;
   function "+" (Left, Right : My_Int) return My_Int;
   function "-" (Left, Right : My_Int) return My_Int;
   function "and" (Left, Right : My_Int) return My_Int;
   function "not" (Right : My_Int) return My_Int;

   -- Valid: task spec end includes defining identifier
   task Named_End_Task is
      entry Start;
   end Named_End_Task;                                   -- UndCC_Valid

   -- Violation: task spec end omits defining identifier
   task Unnamed_End_Task is
      entry Start;
   end;                                                  -- UndCC_Violation

   -- Accept demo tasks (accept end tested in .adb bodies)
   task Named_Accept_Task is
      entry Reset;
   end Named_Accept_Task;                                -- UndCC_Valid

   task Unnamed_Accept_Task is
      entry Reset;
   end Unnamed_Accept_Task;                              -- UndCC_Valid

   -- Valid: protected spec end includes defining identifier
   protected Named_End_Protected is
      procedure Set (Value : in Integer);
   private
      Data : Integer := 0;
   end Named_End_Protected;                              -- UndCC_Valid

   -- Violation: protected spec end omits defining identifier
   protected Unnamed_End_Protected is
      procedure Set (Value : in Integer);
   private
      Data : Integer := 0;
   end;                                                  -- UndCC_Violation

end Test_5_1_4;                                          -- UndCC_Valid
