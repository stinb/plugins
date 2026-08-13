package Test_5_8_4 is

   Custom_Error : exception;
   My_Error     : exception;

   -- Violation: a predefined exception (Constraint_Error) is translated
   -- into a different exception, discarding everything Ada.Exceptions
   -- could have said about what actually went wrong.
   procedure Translate_No_Info;

   -- Violation: same pattern, with a different predefined exception.
   procedure Translate_Program_Error_No_Info;

   -- Valid: the original exception's message is carried forward into
   -- the new exception, so the forensic detail is not lost.
   procedure Translate_With_Message_Info;

   -- Valid: the original exception's information is logged before
   -- translating -- preserved, just not carried into the new exception
   -- itself.
   procedure Translate_With_Logged_Info;

   -- Valid: the exception being translated is user-defined, not
   -- predefined -- out of scope for this guideline, which only calls
   -- out predefined/implementation-defined exceptions as unlocalizable.
   procedure Translate_User_Defined_Source;

   -- Valid: the handler logs and recovers locally; it does not
   -- translate to a different exception at all.
   procedure No_Translate_Just_Log;

   -- Valid: the handler re-raises the exact same predefined exception
   -- (not a translation). A pure relay like this is ADA95_5.8.3's
   -- concern, not this guideline's.
   procedure Reraise_Same_Predefined_No_Info;

   package Edge_Cases is

      Sub_CE : exception renames Constraint_Error;

      -- Violation: Ada is case-insensitive, so uppercase "WHEN"/"RAISE"
      -- must be recognized the same as lowercase.
      procedure Uppercase_Choice;

      -- Violation: a combined choice list with a predefined exception
      -- listed first is still checked, even though only the first choice
      -- is resolvable to an entity.
      procedure Mixed_Predefined_First;

      -- Valid (documented limitation): a combined choice list with a
      -- user-defined exception listed first is not checked, even though
      -- a predefined exception is also caught by the same handler --
      -- only the first-listed choice is resolvable to an entity here.
      procedure Mixed_User_Defined_First;

      -- Valid: Ada.Exceptions.Save_Occurrence is itself one of the
      -- Ada.Exceptions facilities for capturing exception detail, so
      -- using it counts as preserving information, the same as calling
      -- Exception_Name/Message/Information.
      procedure Save_Occurrence_Used;

      -- Violation: translating one predefined exception into another
      -- still discards the original's detail if nothing preserves it.
      procedure Predefined_To_Predefined_No_Info;

      -- Violation: catching a predefined exception through a renamed
      -- name does not change that it is still predefined.
      procedure Renamed_Predefined_Source;

      -- Two independent handlers in the same subprogram: a nested
      -- block's handler that translates with no preserved info, and the
      -- subprogram-level handler that translates and does preserve it.
      -- Each must be judged on its own actual extent.
      procedure Multiple_Handlers_Mixed;

      -- Violation: the same no-info translation pattern inside a
      -- function body.
      function Func_Translate_No_Info return Integer;

   end Edge_Cases;

end Test_5_8_4;
