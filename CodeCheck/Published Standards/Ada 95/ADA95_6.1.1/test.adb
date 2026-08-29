package body Test is

   protected Good_Body_Obj is  -- UndCC_Valid
      entry Get (X : out Integer);
   private
      Count : Integer := 0;
   end Good_Body_Obj;

   procedure Do_Something is
   begin
      null;
   end Do_Something;

   protected body Bad_Visible_Type is
      entry Get (X : out Integer) when Count > 0 is  -- UndCC_Valid
      begin
         X := Count;
         Count := Count - 1;
      end Get;
   end Bad_Visible_Type;

   protected body Bad_Visible_Obj is

      function Is_Ready return Boolean is
      begin
         return Count > 0;
      end Is_Ready;

      entry Get_Bad (X : out Integer) when Global_Flag is  -- UndCC_Violation
      begin
         X := Count;
      end Get_Bad;

      entry Get_Good (X : out Integer) when Is_Ready is  -- UndCC_Valid
      begin
         X := Count;
         Count := Count - 1;
      end Get_Good;

   end Bad_Visible_Obj;

   protected body Good_Private_Type is
      entry Get (X : out Integer) when Count > 0 is  -- UndCC_Valid
      begin
         X := Count;
         Count := Count - 1;
      end Get;
   end Good_Private_Type;

   protected body Good_Body_Obj is
      entry Get (X : out Integer) when Count > 0 is  -- UndCC_Valid
      begin
         X := Count;
         Count := Count - 1;
      end Get;
   end Good_Body_Obj;

   protected body Const_Barrier_Type is
      entry Get (X : out Integer) when Count > Global_Const is  -- UndCC_Valid
      begin
         X := Count;
         Count := Count - 1;
      end Get;
   end Const_Barrier_Type;

end Test;
