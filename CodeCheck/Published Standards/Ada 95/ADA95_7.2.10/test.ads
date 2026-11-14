package Test_7_2_10 is

   task type Worker is
      pragma Storage_Size (1024);  -- UndCC_Valid
   end Worker;

   for Worker'Storage_Size use 1024;  -- UndCC_Violation

   My_Task : Worker;
   for My_Task'Storage_Size use 2048;  -- UndCC_Violation

   type Handle is access Integer;
   for Handle'Storage_Size use 4096;  -- UndCC_Violation

   type Byte is range 0 .. 255;
   for Byte'Size use 8;  -- UndCC_Valid

   Default_Pool_Size : constant := 512;

   type Handle2 is access Integer;
   for Handle2'Storage_Size use Default_Pool_Size * 2;  -- UndCC_Violation

   type Handle3 is access Integer;
   for Handle3'STORAGE_SIZE use 100;  -- UndCC_Violation

   type Handle4 is access Integer;
   for Handle4 'Storage_Size use 100;  -- UndCC_Violation

   Queried_Size : constant Integer := Handle'Storage_Size;  -- UndCC_Valid

end Test_7_2_10;
