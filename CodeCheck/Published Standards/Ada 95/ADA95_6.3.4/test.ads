package Test_6_3_4 is

   task type No_Handler is
      entry Go;
   end No_Handler;

   task type Has_Others_Handler is
      entry Go;
   end Has_Others_Handler;

   task type Has_Specific_Handler_Only is
      entry Go;
   end Has_Specific_Handler_Only;

   task type Handler_Reraises is
      entry Go;
   end Handler_Reraises;

   -- No handler at the task's own top level, only on a nested named
   -- block: a known limitation, since Understand reports a task's own
   -- handlers as a flat set regardless of which nested block declares
   -- them (see ADA95_6.2.2's Developer's Note for the same trade-off).
   task type Only_Nested_Block_Handler is
      entry Go;
   end Only_Nested_Block_Handler;

end Test_6_3_4;
