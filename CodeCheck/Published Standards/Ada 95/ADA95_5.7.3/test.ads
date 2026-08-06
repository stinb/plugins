package Test_5_7_3 is

   type Shape is tagged null record;

   procedure Draw (Self : Shape);
   procedure Resize (Self : Shape; Factor : Integer);

   type Circle is new Shape with null record;

   -- Valid: explicitly marked, so the override is clearly intentional.
   overriding procedure Draw (Self : Circle);

   -- Violation: this profile matches the inherited Resize closely enough
   -- to override it, but nothing marks the declaration as intentional.
   procedure Resize (Self : Circle; Factor : Integer); -- UndCC_Violation

   -- Valid: explicitly marked "not overriding" -- a genuinely new
   -- operation, unrelated to any inherited primitive.
   not overriding procedure Grow (Self : Circle; Amount : Integer);

   -- Valid: a genuinely new operation with a different name, so there is
   -- nothing to accidentally override in the first place.
   procedure Highlight (Self : Circle);

   -- Multi-level inheritance, functions, and parameter-name independence.
   package Multi_Level is

      type Base is abstract tagged null record;
      procedure Draw (Self : Base) is abstract;
      function Area (Self : Base) return Float is abstract;

      type Mid is new Base with null record;
      overriding procedure Draw (Self : Mid);
      overriding function Area (Self : Mid) return Float;

      type Leaf is new Mid with null record;

      -- Violation: overrides Mid's Draw (itself an override of Base), but
      -- not marked. Tests a two-level (transitive) override chain.
      procedure Draw (Self : Leaf); -- UndCC_Violation

      -- Valid: overrides Mid's Area, marked correctly. Also tests that a
      -- function (not just a procedure) is handled.
      overriding function Area (Self : Leaf) return Float;

      procedure Resize (Self : Base; Factor : Integer);

      type Sized is new Base with null record;
      overriding procedure Draw (Self : Sized);
      overriding function Area (Self : Sized) return Float;

      -- Violation: same types/modes as Base.Resize, but the formal
      -- parameter is named "Amount" instead of "Factor". Ada matches by
      -- type, not by name, so this still overrides.
      procedure Resize (Self : Sized; Amount : Integer); -- UndCC_Violation

   end Multi_Level;

   -- Case-insensitivity of the overriding keyword.
   package Case_Insensitive is

      type Base is tagged null record;
      procedure Draw (Self : Base);

      type Circle is new Base with null record;

      -- Valid: "overriding" written with different case; Ada keywords are
      -- case-insensitive.
      OVERRIDING procedure Draw (Self : Circle);

   end Case_Insensitive;

   -- Body-only tagged type: a primitive declared and overridden entirely
   -- within the package body, with no separate spec entry at all. The
   -- expected violation marker is in the body, on the actual override.
   package Body_Only is

      type Widget is tagged null record;
      procedure Refresh (Self : Widget);

   end Body_Only;

end Test_5_7_3;
