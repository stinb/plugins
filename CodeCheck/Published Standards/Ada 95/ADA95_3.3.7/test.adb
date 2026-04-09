procedure Test_3_3_7 is

   -- Package with declarations and correct name comment: valid
   package Compliant_Pkg is
      X : Integer;
   end Compliant_Pkg;

   package body Compliant_Pkg is
      Y : Integer := 0;
   begin  -- Compliant_Pkg
      null;
   end Compliant_Pkg;

   -- Package with declarations but missing name comment: violation
   package Non_Compliant_Pkg is
      X : Integer;
   end Non_Compliant_Pkg;

   package body Non_Compliant_Pkg is
      Y : Integer := 0;
   begin                                -- UndCC_Violation
      null;
   end Non_Compliant_Pkg;

   -- Package with no declarations: valid (no begin preceded by declarations)
   package No_Decls_Pkg is
   end No_Decls_Pkg;

   package body No_Decls_Pkg is
   begin
      null;
   end No_Decls_Pkg;

   -- Procedure with declarations and correct name comment: valid
   procedure Compliant_Proc is
      X : Integer := 0;
   begin  -- Compliant_Proc
      null;
   end Compliant_Proc;

   -- Procedure with declarations but missing name comment: violation
   procedure Non_Compliant_Proc is
      X : Integer := 0;
   begin                                -- UndCC_Violation
      null;
   end Non_Compliant_Proc;

   -- Procedure with no declarations: valid
   procedure No_Decls_Proc is
   begin
      null;
   end No_Decls_Proc;

   -- Procedure with nested subprogram body and correct outer comment: valid
   procedure Outer_With_Nested is
      procedure Inner is
      begin
         null;
      end Inner;
   begin  -- Outer_With_Nested
      null;
   end Outer_With_Nested;

   -- Procedure with nested subprogram body and missing outer comment: violation
   procedure Outer_Missing_Comment is
      procedure Inner is
      begin
         null;
      end Inner;
   begin                                -- UndCC_Violation
      null;
   end Outer_Missing_Comment;

   -- Procedure with record type declaration and correct comment: valid
   procedure With_Record_Type is
      type Point is record
         X : Integer;
         Y : Integer;
      end record;
   begin  -- With_Record_Type
      null;
   end With_Record_Type;

   -- Procedure with record type declaration and missing comment: violation
   procedure With_Record_No_Comment is
      type Point is record
         X : Integer;
         Y : Integer;
      end record;
   begin                                -- UndCC_Violation
      null;
   end With_Record_No_Comment;

   -- Nested body containing a loop: outer begin should still be detected correctly
   procedure With_Inner_Loop is
      procedure Inner_Loop_Proc is
      begin
         for I in 1 .. 10 loop
            null;
         end loop;
      end Inner_Loop_Proc;
   begin  -- With_Inner_Loop
      null;
   end With_Inner_Loop;

   -- Same but missing outer comment: violation
   procedure Missing_Comment_Inner_Loop is
      procedure Inner_Loop_Proc is
      begin
         for I in 1 .. 10 loop
            null;
         end loop;
      end Inner_Loop_Proc;
   begin                                -- UndCC_Violation
      null;
   end Missing_Comment_Inner_Loop;

   -- Nested body containing an if statement: outer begin detected correctly
   procedure With_Inner_If is
      procedure Inner_If_Proc is
      begin
         if True then
            null;
         end if;
      end Inner_If_Proc;
   begin  -- With_Inner_If
      null;
   end With_Inner_If;

   -- Same but missing outer comment: violation
   procedure Missing_Comment_Inner_If is
      procedure Inner_If_Proc is
      begin
         if True then
            null;
         end if;
      end Inner_If_Proc;
   begin                                -- UndCC_Violation
      null;
   end Missing_Comment_Inner_If;

   -- Nested body containing loop > if > case: outer begin detected correctly
   procedure With_Nested_Block_Openers is
      procedure Inner is
      begin
         for I in 1 .. 10 loop
            if I > 5 then
               case I is
                  when 6      => null;
                  when others => null;
               end case;
            end if;
         end loop;
      end Inner;
   begin  -- With_Nested_Block_Openers
      null;
   end With_Nested_Block_Openers;

   -- Same but missing outer comment: violation
   procedure Missing_Nested_Block_Openers is
      procedure Inner is
      begin
         for I in 1 .. 10 loop
            if I > 5 then
               case I is
                  when 6      => null;
                  when others => null;
               end case;
            end if;
         end loop;
      end Inner;
   begin                                -- UndCC_Violation
      null;
   end Missing_Nested_Block_Openers;

   -- Nested: loop (BLOCK_OPENER) containing case..is (NON_BODY_IS_PREV_KW + BLOCK_OPENER)
   -- containing if (BLOCK_OPENER): outer begin detected correctly
   procedure With_Nested_Block_And_NonBody_Is is
      procedure Inner is
      begin
         for I in 1 .. 10 loop
            case I is
               when 1 =>
                  if I > 0 then
                     null;
                  end if;
               when others => null;
            end case;
         end loop;
      end Inner;
   begin  -- With_Nested_Block_And_NonBody_Is
      null;
   end With_Nested_Block_And_NonBody_Is;

   -- Same but missing outer comment: violation
   procedure Missing_Nested_Block_And_NonBody_Is is
      procedure Inner is
      begin
         for I in 1 .. 10 loop
            case I is
               when 1 =>
                  if I > 0 then
                     null;
                  end if;
               when others => null;
            end case;
         end loop;
      end Inner;
   begin                                -- UndCC_Violation
      null;
   end Missing_Nested_Block_And_NonBody_Is;

   -- Record type (BLOCK_OPENER via 'type..is record') combined with nested case..is
   procedure With_Record_And_Case is
      type Point is record
         X : Integer;
         Y : Integer;
      end record;
      procedure Inner is
      begin
         for I in 1 .. 5 loop
            case I is
               when 1 | 2  => null;
               when others => null;
            end case;
         end loop;
      end Inner;
   begin  -- With_Record_And_Case
      null;
   end With_Record_And_Case;

   -- Same but missing outer comment: violation
   procedure Missing_Record_And_Case is
      type Point is record
         X : Integer;
         Y : Integer;
      end record;
      procedure Inner is
      begin
         for I in 1 .. 5 loop
            case I is
               when 1 | 2  => null;
               when others => null;
            end case;
         end loop;
      end Inner;
   begin                                -- UndCC_Violation
      null;
   end Missing_Record_And_Case;

   -- Generic package (Template) used for instantiation tests below
   generic
   package Template is
   end Template;

   -- Generic procedure instantiation ('procedure X is new Y') in declarative part:
   -- 'is new' pushes spurious 'is' onto stack, outer begin may be missed
   package body With_Proc_Generic_Inst is
      procedure Free is new Template;  -- instantiation using 'is new'
      X : Integer := 0;
   begin                                -- UndCC_Violation
      null;
   end With_Proc_Generic_Inst;

   -- Package instantiation ('package X is new Y') in declarative part: same bug
   package body With_Pkg_Generic_Inst is
      package Inst is new Template;
      X : Integer := 0;
   begin                                -- UndCC_Violation
      null;
   end With_Pkg_Generic_Inst;

   -- Declare block inside nested body: outer begin still detected correctly
   procedure With_Declare_Block is
      procedure Inner is
      begin
         declare
            Temp : Integer := 0;
         begin
            null;
         end;
      end Inner;
   begin  -- With_Declare_Block
      null;
   end With_Declare_Block;

   -- Same but missing outer comment: violation
   procedure Missing_Declare_Block is
      procedure Inner is
      begin
         declare
            Temp : Integer := 0;
         begin
            null;
         end;
      end Inner;
   begin                                -- UndCC_Violation
      null;
   end Missing_Declare_Block;

   -- Named block inside nested body: outer begin still detected correctly
   procedure With_Named_Block is
      procedure Inner is
      begin
         Named : begin
            null;
         end Named;
      end Inner;
   begin  -- With_Named_Block
      null;
   end With_Named_Block;

   -- Same but missing outer comment: violation
   procedure Missing_Named_Block is
      procedure Inner is
      begin
         Named : begin
            null;
         end Named;
      end Inner;
   begin                                -- UndCC_Violation
      null;
   end Missing_Named_Block;

   -- Only declaration is inside a declare block AFTER begin: not preceded by declarations,
   -- so no violation even though refs("Declare") returns a ref after begin_lex.line_begin()
   procedure Only_Decl_After_Begin is
   begin
      declare
         Y : Integer := 1;
      begin
         null;
      end;
   end Only_Decl_After_Begin;

   -- Declaration both before begin (X) and after begin (Y in declare block):
   -- first_decl is X (before begin) so violation is correctly detected
   procedure Decls_Before_And_After_Begin is
      X : Integer := 0;
   begin                                -- UndCC_Violation
      declare
         Y : Integer := 1;
      begin
         null;
      end;
   end Decls_Before_And_After_Begin;

   -- Same but with correct comment: valid
   procedure Compliant_Decls_Before_And_After is
      X : Integer := 0;
   begin  -- Compliant_Decls_Before_And_After
      declare
         Y : Integer := 1;
      begin
         null;
      end;
   end Compliant_Decls_Before_And_After;

begin  -- Test_3_3_7
   null;
end Test_3_3_7;
