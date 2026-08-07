package body Test_5_7_5 is

   function "=" (Left, Right : Safe_List_Cell) return Boolean is
   begin
      return Left.Value = Right.Value;
   end "=";

   function "=" (Left, Right : Safe_Measurement) return Boolean is
   begin
      return abs (Left.Value - Right.Value) <= 0.0001;
   end "=";

   function "=" (Left, Right : Fuzzy_Int) return Tri_Bool is
   begin
      return True_Val;
   end "=";

   function "=" (Left, Right : Fuzzy_Int2) return Tri_Bool is
   begin
      return True_Val;
   end "=";

   function "/=" (Left, Right : Fuzzy_Int2) return Tri_Bool is
   begin
      return False_Val;
   end "/=";

   package body More_Cases is
      function "=" (Left, Right : Base) return Boolean is
      begin
         return abs (Left.Value - Right.Value) <= 0.0001;
      end "=";

      function "=" (Left : Mixed_Type; Right : Integer) return Boolean is
      begin
         return Left.Value = Float (Right);
      end "=";
   end More_Cases;

end Test_5_7_5;
