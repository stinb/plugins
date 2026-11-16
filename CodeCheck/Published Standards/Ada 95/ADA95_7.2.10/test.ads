package Test_7_2_10 is

   type Callback is access procedure (X : Integer);
   type Func_Ptr is access function (X : Integer) return Integer;
   type Obj_Ptr is access Integer;

   procedure Do_Thing (X : Integer);

   CB1 : Callback := Do_Thing'Access;
   CB2 : Callback := Do_Thing'Access;

   O1 : Obj_Ptr := new Integer'(1);
   O2 : Obj_Ptr := new Integer'(2);

   function Compute (X : Integer) return Integer;

   FP1 : Func_Ptr := Compute'Access;
   FP2 : Func_Ptr := Compute'Access;

   Ignore : constant Callback := null;

   Also_Ignore : Callback renames Ignore;

   A, B : constant Callback := null;

   function Compare_CB return Boolean;
   function Compare_CB_Ignore return Boolean;
   function Compare_CB_Null return Boolean;
   function Compare_Obj return Boolean;
   function Compare_FP_Null return Boolean;
   function Compare_FP_Ne return Boolean;
   function Compare_Direct_Access return Boolean;
   function Compare_CB_Rename return Boolean;
   function Compare_Multi_Const return Boolean;

end Test_7_2_10;
