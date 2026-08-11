package Test_5_8_3 is

   My_Error : exception;

   -- Violation: the handler does nothing but re-raise the exact exception
   -- it caught -- no cleanup, no logging, no translation. Removing the
   -- handler entirely would behave identically.
   procedure Bare_Reraise;

   -- Violation: same pattern, but with a named occurrence ("when E : ...")
   -- that is declared and never used.
   procedure Named_Bare_Reraise;

   -- Violation: same pattern, with a combined exception choice list.
   procedure Combined_Choice_Reraise;

   -- Valid: the handler performs real work (here, logging) before
   -- re-raising -- a deliberate, informed decision to propagate, not a
   -- reflexive one.
   procedure Reraise_With_Cleanup;

   -- Valid: the handler raises a *different* exception than the one it
   -- caught. This is deliberate exception translation, not passive
   -- relay, so it is not flagged even though it also propagates.
   procedure Translate_Exception;

   -- Violation: a "null;" statement adds no behavior, so this is
   -- functionally identical to a bare reraise.
   procedure Null_Then_Reraise;

   -- Valid: the handler discards the exception silently (no reraise at
   -- all). This is a different anti-pattern (silent swallow, not
   -- pass-through propagation) and is out of scope for this guideline.
   procedure Silent_Null_Only;

   -- Valid: the handler performs genuine recovery and does not
   -- propagate at all.
   procedure Real_Recovery;

   package Edge_Cases is

      Sub_Error : exception renames Test_5_8_3.My_Error;

      -- Valid: a mixed-branch handler that performs real work (logging)
      -- on at least one path is not flagged, even though another path
      -- only reraises.
      procedure Mixed_Branch_With_Call;

      -- Violation: a bare reraise reachable only when a condition holds;
      -- when it does not hold, the handler falls through having done
      -- nothing at all either way.
      procedure Conditional_Bare_Reraise_No_Else;

      -- Violation: explicitly re-raising the same exception by name is
      -- behaviorally identical to a bare "raise;".
      procedure Explicit_Same_Name_Reraise;

      -- Violation: catching an exception through a renamed name does not
      -- change that a bare reraise still adds nothing.
      procedure Renamed_Exception_Reraise;

      -- Two independent handlers in the same subprogram: a nested
      -- block's own handler (a pure reraise) followed by more ordinary
      -- code, then the subprogram-level handler (real recovery). Each
      -- must be judged on its own actual extent, not conflated with
      -- unrelated code that happens to sit between them.
      procedure Multiple_Handlers_Mixed;

      -- Violation: the same pure-reraise pattern inside a function body.
      function Func_Bare_Reraise return Integer;

      -- Violation: Ada is case-insensitive, so uppercase "RAISE"/"WHEN"
      -- must be recognized the same as lowercase.
      procedure Uppercase_Raise;

      task type Dummy_Task_Type;

      -- Valid: a plain assignment before the reraise is real cleanup
      -- (e.g. resetting a lock or state flag), not a no-op -- and it
      -- generates no Call ref, unlike a procedure invocation.
      procedure Assignment_Cleanup_Then_Reraise;

      -- Valid: an "abort" statement is real action, and likewise
      -- generates no Call ref.
      procedure Abort_Task_Then_Reraise;

      -- Valid: an explicit reraise of the same exception that also
      -- attaches a message adds forensic detail a bare relay does not
      -- have, so it is not treated as a pure relay.
      procedure Message_Same_Name_Reraise;

   end Edge_Cases;

end Test_5_8_3;
