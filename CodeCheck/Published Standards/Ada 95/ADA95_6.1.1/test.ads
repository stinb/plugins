package Test is

   Global_Flag : Boolean := False;
   Global_Const : constant Integer := 5;

   protected type Bad_Visible_Type is  -- UndCC_Violation
      entry Get (X : out Integer);
   private
      Count : Integer := 0;
   end Bad_Visible_Type;

   protected Bad_Visible_Obj is  -- UndCC_Violation
      entry Get_Bad (X : out Integer);
      entry Get_Good (X : out Integer);
      function Is_Ready return Boolean;
   private
      Count : Integer := 0;
   end Bad_Visible_Obj;

   procedure Do_Something;

   -- The full view of a private type completed by a protected type must
   -- itself be declared in the private part, but Understand does not mark
   -- it "Local" the way an ordinary private-part declaration is marked.
   type Hidden_Behind_Private is limited private;

private

   protected type Good_Private_Type is  -- UndCC_Valid
      entry Get (X : out Integer);
   private
      Count : Integer := 0;
   end Good_Private_Type;

   -- A barrier referencing a package-level constant (which can never
   -- change) is not the risk this guideline warns about, and must not
   -- be mistaken for a global variable.
   protected type Const_Barrier_Type is  -- UndCC_Valid
      entry Get (X : out Integer);
   private
      Count : Integer := 0;
   end Const_Barrier_Type;

   protected type Hidden_Behind_Private is  -- UndCC_Valid
      entry Get (X : out Integer);
   private
      Count : Integer := 0;
   end Hidden_Behind_Private;

end Test;
