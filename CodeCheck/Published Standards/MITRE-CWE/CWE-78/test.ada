with Ada.Text_IO; use Ada.Text_IO;
with Ada.Command_Line; use Ada.Command_Line;
with Ada.Processes;

procedure Vulnerable_Command_Execution is
begin
    if Argument_Count >= 1 then
        -- Directly use the first command-line argument as part of the command
        Ada.Processes.Spawn (Program => "/bin/sh", Args => (1 => "-c", 2 => Argument(1))); -- UndCC_Violation
    end if;
end Vulnerable_Command_Execution;
