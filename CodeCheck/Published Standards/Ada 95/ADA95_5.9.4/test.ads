with System;
with System.Storage_Elements;

package Test is

   type Hardware_Reg is record
      Value : Integer;
   end record;

   Reg_Addr : constant System.Address := System.Storage_Elements.To_Address (16#4000_0000#);

   Good_Reg : Hardware_Reg;
   for Good_Reg'Address use Reg_Addr;  -- UndCC_Valid

   Overlay_Source : Hardware_Reg;
   Bad_Overlay : Hardware_Reg;
   for Bad_Overlay'Address use Overlay_Source'Address;  -- UndCC_Violation

   procedure Imported_Proc;
   pragma Import (C, Imported_Proc, "some_c_func");
   for Imported_Proc'Address use System.Storage_Elements.To_Address (16#0800_0000#);  -- UndCC_Valid

   procedure Not_Imported_Proc;
   for Not_Imported_Proc'Address use System.Storage_Elements.To_Address (16#0800_1000#);  -- UndCC_Violation

   task Handler_Task is
      entry Interrupt_Entry;
      for Interrupt_Entry use at System.Storage_Elements.To_Address (16#0900_0000#);  -- UndCC_Violation
   end Handler_Task;

   -- Case-insensitive attribute names and keywords must still resolve
   -- correctly, and function targets are treated the same as procedures.
   package More_Cases is

      Overlay_Source_2 : Hardware_Reg;
      Bad_Overlay_Lc : Hardware_Reg;
      for Bad_Overlay_Lc'ADDRESS use Overlay_Source_2'address;  -- UndCC_Violation

      function Imported_Func return Integer;
      pragma Import (C, Imported_Func, "some_c_func");
      for Imported_Func'Address use System.Storage_Elements.To_Address (16#0800_2000#);  -- UndCC_Valid

      function Not_Imported_Func return Integer;
      for Not_Imported_Func'Address use System.Storage_Elements.To_Address (16#0800_3000#);  -- UndCC_Violation

      task Handler_Task2 is
         entry E1;
         entry E2;  -- UndCC_Valid
         for E1 USE AT System.Storage_Elements.To_Address (16#0900_1000#);  -- UndCC_Violation
      end Handler_Task2;

   end More_Cases;

end Test;
