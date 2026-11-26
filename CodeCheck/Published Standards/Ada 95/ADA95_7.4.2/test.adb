package body Test_7_4_2 is

   procedure Raise_Vendor is
   begin
      raise GNAT.Sockets.Socket_Error;  -- UndCC_Violation
   end Raise_Vendor;

   -- Brought into direct visibility by "use GNAT.Sockets", referenced
   -- without the dotted prefix: parent() still resolves to GNAT.Sockets
   -- regardless of how the name was spelled at the call site.
   procedure Raise_Vendor_Via_Use is
   begin
      raise Socket_Error;  -- UndCC_Violation
   end Raise_Vendor_Via_Use;

   -- A user package whose name merely starts with "GNAT_" (not "GNAT.")
   -- is not the vendor namespace and must not be flagged.
   procedure Raise_Lookalike is
   begin
      raise GNAT_Lookalike.Lookalike_Error;  -- UndCC_Valid
   end Raise_Lookalike;

   procedure Raise_User is
   begin
      raise User_Error;  -- UndCC_Valid
   end Raise_User;

   procedure Raise_Predefined is
   begin
      raise Constraint_Error;  -- UndCC_Valid
   end Raise_Predefined;

   -- A rename does not hide the vendor origin: resolved back through the
   -- rename to the GNAT.Sockets.Socket_Error it renames.
   procedure Raise_Renamed_Vendor is
   begin
      raise Renamed_Vendor_Error;  -- UndCC_Violation
   end Raise_Renamed_Vendor;

   procedure Raise_Bare_Reraise is
   begin
      null;
   exception
      when others =>
         raise;  -- UndCC_Valid
   end Raise_Bare_Reraise;

end Test_7_4_2;
