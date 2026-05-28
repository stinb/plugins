package Test_5_6_4 is

   procedure Exit_In_For_Loop (C : Boolean);
   procedure Exit_In_While_Loop (C : Boolean);
   procedure Exit_In_Bare_Loop_Valid (C1, C2 : Boolean);
   procedure Nested_Exit_Applies_To_Inner_Valid (C : Boolean);
   procedure For_Loop_No_Exit_Valid;
   procedure While_Loop_No_Exit_Valid (C : Boolean);

   procedure Three_Exits_Violation (C1, C2, C3 : Boolean);
   procedure Two_Exits_Valid (C1, C2 : Boolean);
   procedure One_Exit_Valid (C : Boolean);

   procedure For_Loop_Two_Exits_Both_Violations (C1, C2 : Boolean);
   procedure Nested_Loops_Exit_Counts_Independent (C1, C2, C3 : Boolean);

   procedure Named_Exit_In_For_Loop (C : Boolean);
   procedure Named_Exit_In_While_Loop (C : Boolean);
   procedure Named_Exit_From_Inner_To_Outer_For (C1, C2 : Boolean);
   procedure Named_Exit_From_Inner_To_Outer_Bare (C1, C2 : Boolean);
   procedure Named_Exit_Count_Applies_To_Target_Loop (C1, C2, C3 : Boolean);

   procedure Bare_In_While_Valid (C1, C2 : Boolean);
   procedure While_Two_Exits_Both_Violations (C1, C2 : Boolean);
   procedure Inner_For_In_For_Violation (C : Boolean);
   procedure Four_Exits_Two_Violations (C1, C2, C3, C4 : Boolean);
   procedure Labeled_Bare_Loop_Unnamed_Exit_Valid (C : Boolean);
   procedure Unconditional_Exit_In_For_Violation (C : Boolean);
   procedure Nested_While_In_For_Exit_Violation (C1, C2 : Boolean);

end Test_5_6_4;
