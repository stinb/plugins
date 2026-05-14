package body Test_5_4_5 is

   -- Violation: allocates but has no Storage_Error handler
   procedure No_Handler is                                  -- UndCC_Violation
      X : Integer_Access := null;
   begin
      X := new Integer'(0);
   end No_Handler;

   -- Valid: allocates and handles Storage_Error explicitly
   procedure Storage_Handler is                             -- UndCC_Valid
      X : Integer_Access := null;
   begin
      X := new Integer'(0);
   exception
      when Storage_Error =>
         X := null;
   end Storage_Handler;

   -- Valid: allocates and handles with 'when others'
   procedure Others_Handler is                              -- UndCC_Valid
      X : Integer_Access := null;
   begin
      X := new Integer'(0);
   exception
      when others =>
         null;
   end Others_Handler;

   -- Valid: no allocation, no handler needed
   procedure No_Allocation is                               -- UndCC_Valid
      X : Integer := 0;
   begin
      X := X + 1;
   end No_Allocation;

   -- Violation: function that allocates but has no handler
   function Make_Int return Integer_Access is               -- UndCC_Violation
   begin
      return new Integer'(42);
   end Make_Int;

   -- Valid: function that allocates with Storage_Error handler
   function Safe_Make_Int return Integer_Access is          -- UndCC_Valid
   begin
      return new Integer'(42);
   exception
      when Storage_Error =>
         return null;
   end Safe_Make_Int;

   -- Violation: multiple allocations, no handler
   procedure Multi_No_Handler is                            -- UndCC_Violation
      X : Integer_Access := null;
      Y : Float_Access := null;
   begin
      X := new Integer'(0);
      Y := new Float'(0.0);
   end Multi_No_Handler;

   -- Valid: multiple allocations with handler
   procedure Multi_With_Handler is                          -- UndCC_Valid
      X : Integer_Access := null;
      Y : Float_Access := null;
   begin
      X := new Integer'(0);
      Y := new Float'(0.0);
   exception
      when Storage_Error =>
         null;
   end Multi_With_Handler;

   -- Valid: type derivation uses 'new' keyword but is not an allocator
   procedure Type_Derivation is                             -- UndCC_Valid
      type My_Int is new Integer;
      X : My_Int := 0;
   begin
      X := X + 1;
   end Type_Derivation;

   -- Violation: allocates inside a loop, no handler
   procedure Alloc_In_Loop is                               -- UndCC_Violation
      X : Integer_Access := null;
   begin
      for I in 1 .. 10 loop
         X := new Integer'(I);
      end loop;
   end Alloc_In_Loop;

   -- Valid: allocates inside a loop with Storage_Error handler
   procedure Alloc_In_Loop_With_Handler is                  -- UndCC_Valid
      X : Integer_Access := null;
   begin
      for I in 1 .. 10 loop
         X := new Integer'(I);
      end loop;
   exception
      when Storage_Error =>
         null;
   end Alloc_In_Loop_With_Handler;

   -- Violation: allocates inside an if, no handler
   procedure Alloc_In_If is                                 -- UndCC_Violation
      X : Integer_Access := null;
      Flag : Boolean := True;
   begin
      if Flag then
         X := new Integer'(1);
      end if;
   end Alloc_In_If;

   -- Violation (ERR1 on X, ERR2 on procedure): uninitialized access var and no handler
   procedure Uninit_And_Alloc is                            -- UndCC_Violation
      X : Integer_Access;                                   -- UndCC_Violation
   begin
      X := new Integer'(0);
   end Uninit_And_Alloc;

   -- Valid: no direct allocation, only calls a subprogram that allocates
   procedure Calls_Allocator is                             -- UndCC_Valid
   begin
      No_Handler;
   end Calls_Allocator;

end Test_5_4_5;
