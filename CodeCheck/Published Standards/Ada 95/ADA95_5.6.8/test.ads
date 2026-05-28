package Test_5_6_8 is

   function Comment_Before_Return_Valid (X : Integer) return Integer;
   function Blank_Line_Before_Return_Valid (X : Integer) return Integer;
   function Return_After_Begin_Valid (X : Integer) return Integer;
   function Case_Branch_Return_Valid (X : Integer) return Integer;
   function Inline_Comment_Return_Valid (X : Integer) return Integer;
   procedure Bare_Return_Blank_Line_Valid (C : Boolean);
   function Multiple_Returns_All_Highlighted_Valid (X, Y : Integer) return Integer;

end Test_5_6_8;
