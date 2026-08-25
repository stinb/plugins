with Ada.Finalization;

package Test is

   type Good_Resource is new Ada.Finalization.Controlled with record
      Handle : Integer;
   end record;

   overriding procedure Finalize (Self : in out Good_Resource);
   overriding procedure Adjust (Self : in out Good_Resource);

   type No_Handler_Resource is new Ada.Finalization.Controlled with record
      Handle : Integer;
   end record;

   overriding procedure Finalize (Self : in out No_Handler_Resource);

   type Partial_Handler_Resource is new Ada.Finalization.Controlled with record
      Handle : Integer;
   end record;

   overriding procedure Finalize (Self : in out Partial_Handler_Resource);

   type Nested_Handler_Resource is new Ada.Finalization.Controlled with record
      Handle : Integer;
   end record;

   overriding procedure Finalize (Self : in out Nested_Handler_Resource);

   -- Unrelated procedure that happens to be named Finalize, not an
   -- override of Ada.Finalization.Controlled's own primitive.
   procedure Finalize (Value : Integer);

   -- A handler that re-raises does not prevent propagation at all, and
   -- must still be flagged.
   type Reraise_Resource is new Ada.Finalization.Controlled with record
      Handle : Integer;
   end record;

   overriding procedure Finalize (Self : in out Reraise_Resource);

   -- Limited_Controlled must be recognized the same as Controlled, and
   -- a nested if/declare block combined with a top-level handler must
   -- not be mistaken for the handler itself being nested.
   type Base_Resource is new Ada.Finalization.Limited_Controlled with record
      Handle : Integer;
   end record;

   overriding procedure Finalize (Self : in out Base_Resource);

   -- Multi-level derivation: this type's own Finalize overrides
   -- Base_Resource's Finalize, not Ada.Finalization.Finalize directly.
   type Derived_Resource is new Base_Resource with record
      Extra : Integer;
   end record;

   overriding procedure Finalize (Self : in out Derived_Resource);

end Test;
