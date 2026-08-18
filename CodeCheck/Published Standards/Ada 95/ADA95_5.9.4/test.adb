package body Test is

   procedure Not_Imported_Proc is
   begin
      null;
   end Not_Imported_Proc;

   task body Handler_Task is
   begin
      accept Interrupt_Entry;
   end Handler_Task;

   package body More_Cases is

      function Not_Imported_Func return Integer is
      begin
         return 0;
      end Not_Imported_Func;

      task body Handler_Task2 is
      begin
         accept E1;
         accept E2;
      end Handler_Task2;

   end More_Cases;

end Test;
