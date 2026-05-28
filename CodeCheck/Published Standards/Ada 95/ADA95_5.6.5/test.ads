package Test_5_6_5 is

   procedure If_Then_Exit_Violation (C : Boolean);
   procedure Exit_When_Valid (C : Boolean);
   procedure If_Then_Exit_With_Else_Valid (C : Boolean);
   procedure If_Then_Exit_With_Elsif_Valid (C1, C2 : Boolean);
   procedure If_Then_Multiple_Statements_Valid (C : Boolean);
   procedure Multiple_Violations (C1, C2 : Boolean);
   procedure And_Then_Condition_Violation (C1, C2 : Boolean);
   procedure Exit_In_For_Loop_Violation (C : Boolean);
   procedure Named_Exit_In_If_Valid (C : Boolean);
   procedure Unconditional_Exit_Valid;
   procedure Or_Else_Condition_Violation (C1, C2 : Boolean);
   procedure Not_Condition_Violation (C : Boolean);
   procedure Nested_If_Then_Exit_Violation (C1, C2 : Boolean);
   procedure Exit_When_Inside_If_Valid (C1, C2 : Boolean);
   procedure If_Then_Exit_In_While_Violation (C1, C2 : Boolean);
   procedure If_Then_Exit_In_Labeled_Loop_Violation (C : Boolean);
   procedure Exit_After_Statement_Valid (C : Boolean);

end Test_5_6_5;
