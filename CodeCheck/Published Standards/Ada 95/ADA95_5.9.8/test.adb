package body Test is

   procedure Finalize (Self : in out Good_Resource) is  -- UndCC_Valid
   begin
      Self.Handle := 0;
   exception
      when others =>
         null;
   end Finalize;

   procedure Adjust (Self : in out Good_Resource) is  -- UndCC_Valid
   begin
      Self.Handle := Self.Handle + 1;
   exception
      when others =>
         null;
   end Adjust;

   procedure Finalize (Self : in out No_Handler_Resource) is  -- UndCC_Violation
   begin
      Self.Handle := 0;
   end Finalize;

   procedure Finalize (Self : in out Partial_Handler_Resource) is  -- UndCC_Violation
   begin
      Self.Handle := 0;
   exception
      when Constraint_Error =>
         null;
   end Finalize;

   procedure Finalize (Self : in out Nested_Handler_Resource) is  -- UndCC_Violation
   begin
      begin
         Self.Handle := 0;
      exception
         when others =>
            null;
      end;

      Self.Handle := Self.Handle + 1;
   end Finalize;

   procedure Finalize (Value : Integer) is  -- UndCC_Valid
   begin
      null;
   end Finalize;

   procedure Finalize (Self : in out Reraise_Resource) is  -- UndCC_Violation
   begin
      Self.Handle := 0;
   exception
      when others =>
         raise;
   end Finalize;

   procedure Finalize (Self : in out Base_Resource) is  -- UndCC_Valid
   begin
      if Self.Handle > 0 then
         declare
            Temp : Integer := Self.Handle;
         begin
            Self.Handle := Temp;
         end;
      end if;
   EXCEPTION
      WHEN OTHERS =>
         null;
   end Finalize;

   procedure Finalize (Self : in out Derived_Resource) is  -- UndCC_Violation
   begin
      Self.Extra := 0;
   end Finalize;

end Test;
