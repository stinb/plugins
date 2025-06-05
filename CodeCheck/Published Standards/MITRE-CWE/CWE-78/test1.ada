with Ada.Text_IO;
 
procedure Main is
  Str  : String (1 .. 80);
  Last : Natural;
begin
  Ada.Text_IO.Get_Line (Str, Last);
  Ada.Processes.Spawn (Program => "/bin/sh", Str (1 .. Last)); -- UndCC_Violation
end;
