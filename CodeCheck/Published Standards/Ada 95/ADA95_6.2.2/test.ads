package Test_6_2_2 is

   task type No_Handler is
      entry Entry_1;
      entry Entry_2;
   end No_Handler;

   task type Has_Program_Error_Handler is
      entry Entry_1;
      entry Entry_2;
   end Has_Program_Error_Handler;

   task type Has_Others_Handler is
      entry Entry_1;
      entry Entry_2;
   end Has_Others_Handler;

   task type Has_Else_No_Handler is
      entry Entry_1;
      entry Entry_2;
   end Has_Else_No_Handler;

   task type Has_Unguarded_Alternative is
      entry Entry_1;
      entry Entry_2;
   end Has_Unguarded_Alternative;

   task type Timed_Entry_Call is
      entry Entry_1;
   end Timed_Entry_Call;

   task type Tricky_Nested is
      entry Entry_1;
      entry Entry_2;
   end Tricky_Nested;

   task type Else_After_Nested_If is
      entry Entry_1;
   end Else_After_Nested_If;

   task type Async_Select is
   end Async_Select;

   Target : No_Handler;

end Test_6_2_2;
