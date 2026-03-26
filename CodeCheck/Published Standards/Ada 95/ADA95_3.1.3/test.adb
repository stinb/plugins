with Ada.Calendar;

procedure Test is

   type second_Of_Day is range 0 .. 86_400; -- UndCC_Violation
   type Noon_Relative_Time is (Before_Noon, After_Noon, High_Noon);

   Subtype Morning is Second_Of_Day range 0 .. 86_400 / 2 - 1; -- UndCC_Violation
   subtype Afternoon is Second_Of_Day range Morning'Last + 2 .. 86_400;

   Current_Time : Second_Of_Day;
   Time_Of_Day  : Noon_Relative_Time;

begin

   Current_time := Second_Of_Day (Ada.Calendar.Seconds (Ada.Calendar.Clock)); -- UndCC_Violation

   if Current_Time in Morning then
      Time_Of_Day := Before_Noon;
   elsif Current_Time in Afternoon then
      Time_Of_Day := After_Noon;
   else
      Time_Of_Day := High_Noon;
   end if;

   null;

end Test;
