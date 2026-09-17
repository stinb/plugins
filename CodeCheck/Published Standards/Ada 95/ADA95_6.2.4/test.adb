package body Test_6_2_4 is

   task body Bad_Manager is
   begin
      loop
         select
            accept Make_Request (Num : Natural) do
               Num_Requested := Num;
            end Make_Request;
         or
            when Num_Requested > 0 =>  -- UndCC_Violation
               accept Get_Container;
         end select;
         if Solo_Flag then  -- used by only this task: no ERR2
            null;
         end if;
      end loop;
   end Bad_Manager;

   task body Good_Manager is
      Local_Num_Requested : Natural := 0;
   begin
      loop
         select
            accept Make_Request (Num : Natural) do
               Local_Num_Requested := Num;
            end Make_Request;
         or
            when Local_Num_Requested <= Personal_Limit  -- UndCC_Valid
                 and Local_Num_Requested < Max_Limit =>
               accept Get_Container;
         end select;
         if Item_Count > 0 then  -- Volatile, shared with Bad_Manager: no ERR2
            null;
         end if;
      end loop;
   end Good_Manager;

   protected body Test_And_Set is
      entry Seize when True is
      begin
         null;
      end Seize;

      procedure Release is
      begin
         null;
      end Release;
   end Test_And_Set;

   task body Reader is
   begin
      accept Go;
      if Shared_Counter > 0 then
         null;
      end if;
      if Outside_Shared > 0 then
         null;
      end if;
      if Flag_Atomic then
         null;
      end if;
      Single_Task.Seize;
   end Reader;

   task body Writer is
   begin
      accept Go;
      Shared_Counter := Shared_Counter + 1;
      Flag_Atomic := True;
      Single_Task.Release;
   end Writer;

   procedure Touch_Outside_Shared is
   begin
      Outside_Shared := Outside_Shared + 1;
   end Touch_Outside_Shared;

   task body Nested_Then_Guard is
      Cond : Boolean := False;
   begin
      loop
         select
            accept Go do
               if Cond then
                  null;
               else
                  null;
               end if;
            end Go;
         or
            when Nested_Guard_Flag =>  -- UndCC_Violation
               accept Retrieve;
         end select;
      end loop;
   end Nested_Then_Guard;

   task body Cross_File_Toucher is
   begin
      accept Go;
      Cross_File_Shared := Cross_File_Shared + 1;
   end Cross_File_Toucher;

end Test_6_2_4;
