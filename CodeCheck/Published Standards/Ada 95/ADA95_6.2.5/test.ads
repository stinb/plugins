package Test_6_2_5 is

   task type Current_Position is
      entry Request_New_Coordinates (X : out Integer; Y : out Integer);
   end Current_Position;

   Position : Current_Position;

   task type Calculate_Flightpath_Bad is
      entry Dummy;
   end Calculate_Flightpath_Bad;

   task type Calculate_Flightpath_Good is
      entry Dummy;
   end Calculate_Flightpath_Good;

   task type Buffer_Messages is
      entry Get_New_Message (Message : out Integer);
   end Buffer_Messages;

   task type One_Shot_Caller is
      entry Dummy;
   end One_Shot_Caller;

   task type Delay_Outside_Loop is
      entry Dummy;
   end Delay_Outside_Loop;

   task type Delay_In_Outer_Loop_Only is
      entry Dummy;
   end Delay_In_Outer_Loop_Only;

   task type Nested_If_Before_Else is
      entry Dummy;
   end Nested_If_Before_Else;

end Test_6_2_5;
