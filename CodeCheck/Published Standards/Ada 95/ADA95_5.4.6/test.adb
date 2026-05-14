package body Test_5_4_6 is

   type Int_All_Ptr is access all Integer;

   -- Valid: aliased constant in package body (constants exempt)
   Body_Const : aliased constant String := "const";          -- UndCC_Valid

   -- Valid: aliased body variable whose 'Access IS taken
   Body_Active : aliased Integer := 0;                       -- UndCC_Valid
   Body_Ref    : Int_All_Ptr := Body_Active'Access;

   -- Violation: aliased body variable whose 'Access is NEVER taken
   Body_Idle : aliased Integer := 0;                         -- UndCC_Violation

   procedure Test_Locals is
      type Local_Int_Ptr is access all Integer;

      -- Valid: aliased local variable whose 'Access IS taken
      Local_Active : aliased Integer := 42;                  -- UndCC_Valid
      Local_Ref    : Local_Int_Ptr := Local_Active'Access;

      -- Violation: aliased local variable whose 'Access is NEVER taken
      Local_Idle : aliased Integer := 0;                     -- UndCC_Violation

      -- Valid: aliased local constant (constants exempt)
      Local_Const : aliased constant Integer := 99;          -- UndCC_Valid
   begin
      null;
   end Test_Locals;

   -- Valid: aliased parameter-like usage — 'Access passed to a subprogram
   procedure Use_Access (P : Int_All_Ptr) is
   begin
      null;
   end Use_Access;

   procedure Pass_Access is
      -- Valid: 'Access is passed as argument to Use_Access
      Arg : aliased Integer := 7;                            -- UndCC_Valid
   begin
      Use_Access (Arg'Access);
   end Pass_Access;

   procedure No_Pass_Access is
      -- Violation: aliased but 'Access is never passed or stored
      Unused : aliased Integer := 7;                         -- UndCC_Violation
   begin
      null;
   end No_Pass_Access;

end Test_5_4_6;
