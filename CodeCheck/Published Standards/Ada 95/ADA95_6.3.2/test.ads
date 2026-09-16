with Ada.Unchecked_Deallocation;

package Test_6_3_2 is

   task type No_Terminate is
      entry Retrieve;
      entry Store;
   end No_Terminate;

   task type Has_Terminate is
      entry Retrieve;
      entry Store;
   end Has_Terminate;

   task type Has_Delay is
      entry Retrieve;
   end Has_Delay;

   task type Has_Else is
      entry Retrieve;
   end Has_Else;

   task type Timed_Call_Only is
      entry Retrieve;
   end Timed_Call_Only;

   task type Nested_Select is
      entry Go;
      entry Stop;
   end Nested_Select;

   type Worker_Access is access No_Terminate;
   procedure Free is new Ada.Unchecked_Deallocation (No_Terminate, Worker_Access);

   type Non_Task_Access is access Integer;
   procedure Free_Int is new Ada.Unchecked_Deallocation (Integer, Non_Task_Access);

end Test_6_3_2;
