package body Test is

   Good_Body_Ptr : Rec_Access := Global_Obj'Unchecked_Access;  -- UndCC_Valid

   procedure Do_Something is
      Local_Obj : aliased Rec;
      Local_Ptr : Rec_Access;
   begin
      Local_Ptr := Local_Obj'Unchecked_Access;  -- UndCC_Violation
      null;
   end Do_Something;

   -- Case-insensitive attribute name, array-element and selected-
   -- component prefixes, and protected/nested-package scoping must all
   -- still resolve to the right object for the library-level check.
   package More_Cases is
      procedure Run;
   end More_Cases;

   package body More_Cases is

      type Rec_Array is array (1 .. 3) of aliased Rec;

      Lib_Arr : Rec_Array;

      package Lib_Nested is
         Lib_Obj : aliased Rec;
      end Lib_Nested;

      protected Guard is
         entry Wait;
      end Guard;

      protected body Guard is
         entry Wait when True is
            Prot_Local : aliased Rec;
            Prot_Ptr : Rec_Access;
         begin
            Prot_Ptr := Prot_Local'unchecked_access;  -- UndCC_Violation
         end Wait;
      end Guard;

      -- A protected object's own private component is part of its
      -- persistent state, not a transient local like an entry/procedure
      -- body's own declarations — if the protected object itself is
      -- library level, so is its state.
      protected State_Guard is
         procedure Touch;
      private
         State : aliased Rec;
         State_Ptr : Rec_Access;
      end State_Guard;

      protected body State_Guard is
         procedure Touch is
         begin
            State_Ptr := State'Unchecked_Access;  -- UndCC_Valid
         end Touch;
      end State_Guard;

      task Worker is
      end Worker;

      task body Worker is
         Task_Local : aliased Rec;
         Task_Ptr : Rec_Access;
      begin
         Task_Ptr := Task_Local'Unchecked_Access;  -- UndCC_Violation
      end Worker;

      procedure Run is
         Ptr : Rec_Access;

         package Local_Nested is
            Local_Obj : aliased Rec;
         end Local_Nested;

         Local_Arr : Rec_Array;
      begin
         Ptr := Lib_Arr (1)'Unchecked_Access;  -- UndCC_Valid

         Ptr := Lib_Nested.Lib_Obj'Unchecked_Access;  -- UndCC_Valid

         Ptr := Local_Arr (1)'Unchecked_Access;  -- UndCC_Violation

         Ptr := Local_Nested.Local_Obj'Unchecked_Access;  -- UndCC_Violation

         Guard.Wait;
         State_Guard.Touch;
      end Run;

   end More_Cases;

end Test;
