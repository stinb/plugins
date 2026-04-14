-- Violation: configuration pragma before a generic package
pragma Normalize_Scalars;  -- UndCC_Violation

generic
   type Element is private;
package Test_Generic is
   procedure Push (Item : Element);
   procedure Pop  (Item : out Element);
end Test_Generic;
