package Test_5_6_6 is

   procedure Goto_In_Procedure;
   procedure No_Goto_Valid;
   procedure Multiple_Gotos_Violation (C : Boolean);
   procedure Goto_In_For_Loop (C : Boolean);
   procedure Goto_In_While_Loop (C1, C2 : Boolean);
   procedure Backward_Goto_Violation;
   procedure Label_Without_Goto_Valid;
   procedure Goto_In_Exception_Handler;

end Test_5_6_6;
