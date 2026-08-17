package Test is

   type Rec is record
      Value : Integer;
   end record;

   type Rec_Access is access all Rec;

   Global_Obj : aliased Rec;

   Bad_Spec_Ptr : Rec_Access := Global_Obj'Unchecked_Access;  -- UndCC_Violation

   Ok_Access_Ptr : Rec_Access := Global_Obj'Access;  -- UndCC_Valid

   procedure Do_Something;

end Test;
