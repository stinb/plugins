package body Test_5_7_2 is

   function "+" (X, Y : Rational) return Rational is
   begin
      return (Num => X.Num + Y.Num, Den => X.Den);
   end "+";

   procedure Actual_Work (X, Y : Integer; Result : out Integer) is
   begin
      Result := X * Y + 1;
   end Actual_Work;

   function Actual_Work_Fn (X, Y : Integer) return Integer is
   begin
      return X * Y + 1;
   end Actual_Work_Fn;

   function Forward_Fn (X, Y : Integer) return Integer is
   begin
      return Actual_Work_Fn (X, Y); -- UndCC_Violation
   end Forward_Fn;

   procedure Forward_Proc (X, Y : Integer; Result : out Integer) is
   begin
      Actual_Work (X, Y, Result); -- UndCC_Violation
   end Forward_Proc;

   function Real_Logic (X, Y : Integer) return Integer is
      Temp : Integer;
   begin
      Temp := X + Y;
      return Temp * 2;
   end Real_Logic;

   function Transforms_Argument (X, Y : Integer) return Integer is
   begin
      return Actual_Work_Fn (X + 1, Y);
   end Transforms_Argument;

   function Reorders_Arguments (X, Y : Integer) return Integer is
   begin
      return Actual_Work_Fn (Y, X);
   end Reorders_Arguments;

   function Factorial (N : Natural) return Natural is
   begin
      if N = 0 then
         return 1;
      end if;
      return N * Factorial (N - 1);
   end Factorial;

   procedure Do_Nothing is
   begin
      null;
   end Do_Nothing;

   function Actual_Get_Value return Integer is
   begin
      return 42;
   end Actual_Get_Value;

   function Forward_No_Params return Integer is
   begin
      return Actual_Get_Value; -- UndCC_Violation
   end Forward_No_Params;

   procedure Forward_Proc_No_Params is
   begin
      Do_Nothing; -- UndCC_Violation
   end Forward_Proc_No_Params;

   function Named_Association (X, Y : Integer) return Integer is
   begin
      return Actual_Work_Fn (X => X, Y => Y);
   end Named_Association;

   function Extra_Statement (X, Y : Integer) return Integer is
   begin
      null;
      return Actual_Work_Fn (X, Y);
   end Extra_Statement;

   function Has_Exception_Handler (X, Y : Integer) return Integer is
   begin
      return Actual_Work_Fn (X, Y);
   exception
      when others =>
         return 0;
   end Has_Exception_Handler;

   package body Helper is
      function Actual_Work (X, Y : Integer) return Integer is
      begin
         return X * Y + 1;
      end Actual_Work;
   end Helper;

   function Forward_Qualified (X, Y : Integer) return Integer is
   begin
      return Helper.Actual_Work (X, Y); -- UndCC_Violation
   end Forward_Qualified;

   function Combine (X : Integer) return Integer is
   begin
      return Combine (X, 0);
   end Combine;

   function Combine (X, Y : Integer) return Integer is
   begin
      return X + Y;
   end Combine;

end Test_5_7_2;
