package Test is

   task type Dispatch is
      entry Transmit;
      entry Receive;
   end Dispatch;

   D : Dispatch;

   protected type Bounded_Buffer is
      entry Put;
   end Bounded_Buffer;

   PB : Bounded_Buffer;

end Test;
