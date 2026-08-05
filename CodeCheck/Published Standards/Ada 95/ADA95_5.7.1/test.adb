package body Test_5_7_1 is

   -- Valid: use clause localized to this subprogram's declarative part
   -- only. It does not affect any other subprogram in this body.
   procedure Localized_Use_Valid is
      use Ada.Strings.Unbounded;
      Message : Unbounded_String := To_Unbounded_String("hello");
   begin
      Put_Line(To_String(Message));
   end Localized_Use_Valid;

   -- Violation: use clause in the package body's declarative part affects
   -- every subprogram that follows it in this body, rather than being
   -- localized to just the one subprogram that needs it.
   use Ada.Strings.Unbounded; -- UndCC_Violation

   procedure Package_Body_Use_Violation is
      Message : Unbounded_String := To_Unbounded_String("hi");
   begin
      Put_Line(To_String(Message));
   end Package_Body_Use_Violation;

   package body Inner_Pkg is
      procedure Show is
         Message : Unbounded_String := To_Unbounded_String("inner");
      begin
         Put_Line(To_String(Message));
      end Show;
   end Inner_Pkg;

   -- Valid: a task body has no shared declarative part the way a package
   -- body does (each task type has exactly one body), so a use clause
   -- here can never affect any sibling unit.
   task body Counter is
      use Ada.Strings.Unbounded;
      Total : Unbounded_String := To_Unbounded_String("0");
   begin
      loop
         select
            accept Increment do
               Total := To_Unbounded_String("1");
            end Increment;
         or
            accept Reset do
               Total := To_Unbounded_String("0");
            end Reset;
         or
            terminate;
         end select;
      end loop;
   end Counter;

end Test_5_7_1;
