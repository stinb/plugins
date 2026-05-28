package body Test_5_6_6 is

   procedure Goto_In_Procedure is
   begin
      -- Violation: goto statement
      goto Skip; -- UndCC_Violation
      null;
      <<Skip>>
      null;
   end Goto_In_Procedure;

   procedure No_Goto_Valid is
   begin
      -- Valid: no goto statement
      null;
   end No_Goto_Valid;

   procedure Multiple_Gotos_Violation (C : Boolean) is
   begin
      -- Both goto statements are violations
      if C then
         goto Label_A; -- UndCC_Violation
      end if;
      goto Label_B; -- UndCC_Violation
      <<Label_A>>
      null;
      <<Label_B>>
      null;
   end Multiple_Gotos_Violation;

   procedure Goto_In_For_Loop (C : Boolean) is
   begin
      -- Violation: goto inside a for loop body
      for I in 1 .. 10 loop
         if C then
            goto Done; -- UndCC_Violation
         end if;
      end loop;
      <<Done>>
      null;
   end Goto_In_For_Loop;

   procedure Goto_In_While_Loop (C1, C2 : Boolean) is
   begin
      -- Violation: goto inside a while loop body
      while C1 loop
         if C2 then
            goto Finish; -- UndCC_Violation
         end if;
      end loop;
      <<Finish>>
      null;
   end Goto_In_While_Loop;

   procedure Backward_Goto_Violation is
      I : Integer := 0;
   begin
      -- Violation: backward goto simulating a loop
      <<Top>>
      I := I + 1;
      if I < 10 then
         goto Top; -- UndCC_Violation
      end if;
   end Backward_Goto_Violation;

   procedure Label_Without_Goto_Valid is
   begin
      -- Valid: label present but no goto statement
      null;
      <<Unused_Label>>
      null;
   end Label_Without_Goto_Valid;

   procedure Goto_In_Exception_Handler is
   begin
      null;
   exception
      when others =>
         goto Recover; -- UndCC_Violation
         <<Recover>>
         null;
   end Goto_In_Exception_Handler;

end Test_5_6_6;
