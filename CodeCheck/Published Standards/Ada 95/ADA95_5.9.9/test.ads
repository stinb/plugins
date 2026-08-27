package Test is

   task type Worker is
      entry Signal;
   end Worker;

   protected Guard is
      procedure Do_Delay;
      procedure Do_Abort (W : access Worker);
      procedure Do_Select (W : access Worker);
      procedure Call_Entry (W : access Worker);
      procedure Declare_Task;
      procedure Clean;
      procedure Calls_Blocking_Helper (W : access Worker);

      -- Case-insensitive keyword, a protected function (not just a
      -- procedure/entry), and mutual recursion (must terminate cleanly,
      -- not hang or crash) must all resolve correctly.
      procedure Uppercase_Delay;
      function Clean_Func return Integer;
      procedure Mutual_A;
      procedure Mutual_B;
   end Guard;

end Test;
