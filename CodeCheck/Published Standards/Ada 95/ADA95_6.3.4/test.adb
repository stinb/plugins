package body Test_6_3_4 is

   task body No_Handler is  -- UndCC_Violation
   begin
      accept Go;
   end No_Handler;

   task body Has_Others_Handler is  -- UndCC_Valid
   begin
      accept Go;
   exception
      when others =>
         null;
   end Has_Others_Handler;

   task body Has_Specific_Handler_Only is  -- UndCC_Violation
   begin
      accept Go;
   exception
      when Constraint_Error =>
         null;
   end Has_Specific_Handler_Only;

   task body Handler_Reraises is  -- UndCC_Valid
   begin
      accept Go;
   exception
      when others =>
         raise;
   end Handler_Reraises;

   task body Only_Nested_Block_Handler is  -- UndCC_FalseNeg
   begin
      Inner:
      begin
         accept Go;
      exception
         when others =>
            null;
      end Inner;
   end Only_Nested_Block_Handler;

end Test_6_3_4;
