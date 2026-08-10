package Test_5_8_2 is

   -- Valid: the "others" handler logs details via the anonymous form
   -- (Ada.Exceptions.Current_Exception).
   procedure Logged_Catch;

   -- Violation: the "others" handler does nothing but discard the
   -- exception.
   procedure Silent_Catch;

   -- Valid: the "others" handler logs details via the named-occurrence
   -- form ("when E : others =>").
   procedure Named_Handler_Catch;

   -- Violation: Exception_Information is called somewhere in the normal
   -- body, but the "others" handler itself is still silent. A check that
   -- only asks "does this subprogram call Exception_Information anywhere"
   -- would miss this.
   procedure Unrelated_Call_Then_Silent;

   -- Valid: logs via Exception_Message specifically, not Information.
   procedure Logged_With_Message;

   -- Valid: logs via Exception_Name specifically.
   procedure Logged_With_Name;

   -- Valid: has no "others" handler at all (only named handlers), so
   -- there is nothing for this check to flag.
   procedure No_Others_Handler;

   -- Violation: the "others" handler does log something, but not the
   -- exception's own details -- a plain message gives no forensic value.
   procedure Logs_Unrelated_Message;

   package Edge_Cases is

      -- Violation: Ada is case-insensitive, so an uppercase "WHEN OTHERS"
      -- must be recognized the same as lowercase.
      procedure Upper_Case_Others;

      -- Two independent "others" handlers in the same subprogram: a
      -- nested block's own handler (which logs) and the subprogram-level
      -- handler further down (which is silent). Each must be judged on
      -- its own, not conflated with the other.
      procedure Nested_Then_Outer_Others;

      -- Violation: several named handlers precede "others", which is
      -- still silent.
      procedure Many_Named_Then_Others;

      -- Violation: a bare "raise;" propagates the exception but never
      -- captures its details at this handler.
      procedure Reraise_Only;

      -- Valid: logs via an unqualified call, reachable through a
      -- "use Ada.Exceptions;" clause rather than full dotted names.
      procedure Logged_Via_Use_Clause;

      -- Valid: the call to Exception_Information happens inside the
      -- handler even though its result is passed into a local helper
      -- procedure instead of Ada.Text_IO directly.
      procedure Logged_Indirectly;

      -- Valid: the logging call is nested inside an "if" statement
      -- within the handler.
      procedure Logged_Inside_Nested_If;

   end Edge_Cases;

end Test_5_8_2;
