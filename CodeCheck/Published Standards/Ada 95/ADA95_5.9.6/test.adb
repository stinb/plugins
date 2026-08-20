package body Test is

   function Known_First return Position is
   begin
      return (X => 0);
   end Known_First;

   Good_Obj : Position := Known_First;  -- UndCC_Valid

   Bad_Obj : Position := Sense;  -- UndCC_Violation

   -- A second object forward-referencing the same not-yet-elaborated
   -- function must be flagged independently.
   Also_Bad_Obj : Position := Sense;  -- UndCC_Violation

   function Adjust (P : Position) return Position is
   begin
      return P;
   end Adjust;

   -- Known limitation: a forward reference wrapped inside another call
   -- is not traced — only the outermost call target is inspected.
   Bad_Nested_Obj : Position := Adjust (Sense);  -- UndCC_FalseNeg

   function Sense return Position is
   begin
      return (X => 1);
   end Sense;

   Deferred_Obj : Position;

   -- The remaining guideline bullets have no structural detection at all
   -- (see Developer's Note) — these illustrate each one, so the
   -- limitation is documented as a marked test case, not just prose.
   package Non_Automated_Bullets is
      procedure Mix_Letters (Of_String : in out String);
   end Non_Automated_Bullets;

   package body Non_Automated_Bullets is

      -- "Initialize all objects prior to use."
      procedure Uninitialized_Use is
         Total : Integer;
      begin
         Total := Total + 1;  -- UndCC_FalseNeg
      end Uninitialized_Use;

      -- "Use caution when initializing access values." (guideline's own
      -- example: the allocation can raise Storage_Error in the caller,
      -- not here, so it cannot be trapped locally.)
      procedure Mix_Letters (Of_String : in out String) is
         type String_Ptr is access String;
         Ptr : String_Ptr := new String'(Of_String);  -- UndCC_FalseNeg
      begin
         null;
      end Mix_Letters;

      -- "Do not depend on default initialization that is not part of
      -- the language." Ada guarantees no default value for Integer;
      -- assuming the OS zeroed this memory page is not portable.
      Assumed_Zero : Integer;  -- UndCC_FalseNeg

      -- "Derive from a controlled type and override the primitive
      -- procedure to ensure automatic initialization."
      type Resource is record  -- UndCC_FalseNeg
         Handle : Integer;
      end record;

      -- "Use function calls in declarations cautiously." An exception
      -- raised here propagates to the enclosing scope, not handled
      -- locally.
      function Risky (N : Integer) return Integer is
      begin
         return 100 / N;
      end Risky;

      Divisor : Integer := 0;
      Value   : Integer := Risky (Divisor);  -- UndCC_FalseNeg

   end Non_Automated_Bullets;

begin
   Deferred_Obj := Sense;  -- UndCC_Valid
end Test;
