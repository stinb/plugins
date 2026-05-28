package Test_5_6_1 is

   procedure Five_Levels (C1, C2, C3, C4, C5 : Boolean; X : Integer);
   procedure Six_Levels (C1, C2, C3, C4, C5, C6 : Boolean; X : Integer);
   procedure Elsif_Does_Not_Add_Level (C1, C2, C3 : Boolean);
   procedure Nested_Loops (C1 : Boolean);
   procedure Four_Levels (C1, C2, C3, C4 : Boolean);

   procedure While_Loop_Nesting (C : Boolean);
   procedure Seven_Levels (C1, C2, C3, C4, C5, C6, C7 : Boolean);
   procedure Violation_Then_Recovery (C1, C2, C3, C4, C5, C6 : Boolean);
   procedure Single_If (C : Boolean);

   procedure Conditional_Expression_Not_Counted (C1, C2, C3, C4, C5, C6 : Boolean; X, Y : Integer);
   procedure Case_Expression_Not_Counted (X : Integer);
   procedure Mixed_Expr_And_Statement (C1, C2, C3, C4, C5 : Boolean; X : Integer);

end Test_5_6_1;
