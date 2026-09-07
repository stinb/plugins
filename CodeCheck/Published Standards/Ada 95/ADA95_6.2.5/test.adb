package body Test_6_2_5 is

   task body Current_Position is
      X, Y : Integer;
   begin
      accept Request_New_Coordinates (X : out Integer; Y : out Integer) do
         X := 0;
         Y := 0;
      end Request_New_Coordinates;
   end Current_Position;

   task body Calculate_Flightpath_Bad is
      X, Y : Integer;
   begin
      loop
         select  -- UndCC_Violation
            Position.Request_New_Coordinates (X, Y);
            null;
         else
            null;
         end select;
      end loop;
   end Calculate_Flightpath_Bad;

   task body Calculate_Flightpath_Good is
      X, Y : Integer;
      Time_To_Execute : Integer := 0;
      Period : constant Integer := 1;
   begin
      loop
         select  -- UndCC_Valid
            Position.Request_New_Coordinates (X, Y);
            null;
         else
            null;
            delay until Time_To_Execute;
            Time_To_Execute := Time_To_Execute + Period;
         end select;
      end loop;
   end Calculate_Flightpath_Good;

   task body Buffer_Messages is
      Time_To_Execute : Integer := 0;
      Period : constant Integer := 1;
      Msg : Integer;
   begin
      loop
         delay until Time_To_Execute;

         select  -- UndCC_Valid
            accept Get_New_Message (Message : out Integer) do
               Message := 0;
            end Get_New_Message;
         else
            null;
         end select;

         Time_To_Execute := Time_To_Execute + Period;
      end loop;
   end Buffer_Messages;

   task body One_Shot_Caller is
      X, Y : Integer;
   begin
      select  -- UndCC_Valid
         Position.Request_New_Coordinates (X, Y);
      else
         null;
      end select;
   end One_Shot_Caller;

   task body Delay_Outside_Loop is
      X, Y : Integer;
      Cond : Boolean := False;
   begin
      -- A delay before the loop does not pace its repeated iterations.
      delay 1.0;
      loop
         if Cond then
            select  -- UndCC_Violation
               Position.Request_New_Coordinates (X, Y);
            else
               null;
            end select;
         end if;
      end loop;
   end Delay_Outside_Loop;

   task body Delay_In_Outer_Loop_Only is
      X, Y : Integer;
   begin
      loop
         delay 1.0;
         loop
            select  -- UndCC_Violation
               Position.Request_New_Coordinates (X, Y);
            else
               exit;
            end select;
         end loop;
      end loop;
   end Delay_In_Outer_Loop_Only;

end Test_6_2_5;
