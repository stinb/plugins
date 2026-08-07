package Test_5_7_5 is

   type Node;
   type Node_Access is access Node;

   -- Violation: has an access component (Next) and relies on the default
   -- equality, which compares the pointer value rather than the pointee.
   type List_Cell is record -- UndCC_Violation
      Value : Integer;
      Next  : Node_Access;
   end record;

   -- Violation: has a Float component and relies on the default equality,
   -- which does an exact comparison rather than an epsilon-tolerant one.
   type Measurement is record -- UndCC_Violation
      Value : Float;
   end record;

   -- Valid: has an access component, but a custom "=" is defined, so the
   -- default pointer-identity equality is not silently relied upon.
   type Safe_List_Cell is record
      Value : Integer;
      Next  : Node_Access;
   end record;

   function "=" (Left, Right : Safe_List_Cell) return Boolean;

   -- Valid: has a Float component, but a custom "=" is defined.
   type Safe_Measurement is record
      Value : Float;
   end record;

   function "=" (Left, Right : Safe_Measurement) return Boolean;

   -- Valid: no access or floating-point component, so the default
   -- equality is not a concern here.
   type Plain_Record is record
      Value : Integer;
   end record;

   type Node is record
      Data : Integer;
   end record;

   type Tri_Bool is (True_Val, False_Val, Unknown_Val);

   type Fuzzy_Int is record
      Value : Integer;
   end record;

   -- Violation: "=" returns a non-Boolean (three-valued) result, so Ada
   -- will not auto-derive "/=" from it, and no "/=" is defined either.
   function "=" (Left, Right : Fuzzy_Int) return Tri_Bool; -- UndCC_Violation

   type Fuzzy_Int2 is record
      Value : Integer;
   end record;

   -- Valid: "=" is three-valued, but a matching "/=" is defined too.
   function "=" (Left, Right : Fuzzy_Int2) return Tri_Bool;
   function "/=" (Left, Right : Fuzzy_Int2) return Tri_Bool;

   package More_Cases is

      -- Violation: an array of Float relies on the default equality,
      -- which compares elements exactly rather than with a tolerance.
      type Float_Vector is array (1 .. 3) of Float; -- UndCC_Violation

      type Base is tagged record
         Value : Float;
      end record;

      function "=" (Left, Right : Base) return Boolean;

      -- Valid: adds no components of its own; the only Float component
      -- is inherited from Base, which already has a proper same-type
      -- "=".
      type Derived_Safe is new Base with null record;

      -- Violation: has a Float component and no same-type "=" of its
      -- own.
      type Base2 is tagged record -- UndCC_Violation
         Value : Float;
      end record;

      -- Known limitation (not flagged): Derived_Risky adds no
      -- components of its own, so the inherited Float component from
      -- Base2 above (which has no custom "=") is not seen. See the
      -- Developer's Note.
      type Derived_Risky is new Base2 with null record;

      -- Violation: the only "=" declared for this type is the asymmetric
      -- one below (Mixed_Type vs. Integer), which does not address
      -- Mixed_Type vs. Mixed_Type comparisons -- those still rely on the
      -- default equality, so this type itself is flagged.
      type Mixed_Type is record -- UndCC_Violation
         Value : Float;
      end record;

      function "=" (Left : Mixed_Type; Right : Integer) return Boolean;

      -- Violation, exactly once: this partial declaration and its full
      -- view below (in the private part) are the same entity with two
      -- separate "Declare"-kind refs. Regression test for a bug where
      -- both were reported as independent violations for the one type
      -- instead of being deduplicated.
      type Private_With_Access is tagged limited private; -- UndCC_Violation

   private

      type Private_With_Access is tagged limited record
         Next : Node_Access;
      end record;

   end More_Cases;

end Test_5_7_5;
