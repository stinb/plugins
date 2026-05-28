package Test_5_6_3 is

   type Color is (Red, Yellow, Green, Blue, Purple);
   type Day is (Mon, Tue, Wed, Thu, Fri, Sat, Sun);

   subtype Weekday is Day range Mon .. Fri;
   subtype Lower_Case is Character range 'a' .. 'z';
   subtype Weekend is Day range Sat .. Sun;

   procedure Enum_Range_Violation (C : Color);
   procedure Enum_Range_In_Alternative (C : Color);
   procedure Explicit_Enumeration_Valid (C : Color);
   procedure Character_Range_Valid (Ch : Character);
   procedure Subtype_Range_Valid (D : Day);
   procedure Others_Valid (C : Color);
   procedure Single_Enum_Valid (C : Color);
   procedure Multiple_Ranges_Violation (D : Day);

   procedure Nested_Case_Inner_Violation (C : Color; D : Day);
   procedure Nested_Case_Outer_Violation (C : Color; D : Day);
   procedure Range_At_End_Of_Alternatives (C : Color);
   procedure Exception_Handler_When_Valid (C : Color);
   procedure Integer_Range_Valid (X : Integer);
   procedure Two_Ranges_Same_When (C : Color; D : Day);

end Test_5_6_3;
