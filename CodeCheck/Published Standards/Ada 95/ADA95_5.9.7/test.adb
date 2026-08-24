package body Test is

   procedure Do_Something is
      F  : Color_Direct_IO.File_Type;
      SF : Color_Seq_IO.File_Type;
      RF : Rec_Direct_IO.File_Type;
      C1 : Color;
      C2 : Color;
      C3 : Color;
      R1 : Rec;
   begin
      Color_Direct_IO.Read (F, C1);

      if C1'Valid then
         null;
      end if;

      Color_Direct_IO.Read (F, C2);  -- UndCC_Violation

      Color_Seq_IO.Read (SF, C3);  -- UndCC_Violation

      Rec_Direct_IO.Read (RF, R1);  -- UndCC_Valid

      null;
   end Do_Something;

   -- Unqualified generic access via a use-clause must still resolve
   -- correctly, and a call routed through a rename of Read is a known
   -- limitation: Understand does not attach a traceable Rename ref for
   -- a rename of a generic-instantiated procedure, so it is not tracked.
   package More_Cases is
      procedure Run;
   end More_Cases;

   package body More_Cases is
      use Ada;

      package Lc_IO is new direct_io (Color);

      procedure Renamed_Read (F : in out Lc_IO.File_Type; Item : out Color)
         renames Lc_IO.Read;

      procedure Run is
         F  : Lc_IO.File_Type;
         C1 : Color;
         C2 : Color;
      begin
         Lc_IO.Read (F, C1);  -- UndCC_Violation

         Renamed_Read (F, C2);  -- UndCC_FalseNeg
      end Run;

   end More_Cases;

end Test;
