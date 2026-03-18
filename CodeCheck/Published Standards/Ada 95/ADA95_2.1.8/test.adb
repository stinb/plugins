procedure Test is
   File_Handle : File_Type;
   Line        : String(1 .. 100);
   Last        : Natural;
begin
   Open(File_Handle, In_File, "input.txt");

   if End_Of_File(File_Handle) then Close(File_Handle);
   else Get_Line(File_Handle, Line, Last); end if; -- UndCC_Violation
   
   Close (File_Handle);

   Put("A="); Natural_IO.Put (A); New_Line; -- UndCC_FalsePos
end Test;
