with Ada.Finalization;

package Test is

   type Resource is new Ada.Finalization.Controlled with record
      Handle : Integer;
   end record;

   overriding procedure Initialize (Self : in out Resource);
   overriding procedure Finalize (Self : in out Resource);
   overriding procedure Adjust (Self : in out Resource);

   task type Some_Task;

   task type Worker is
      entry Signal;
      entry Wait_Item;
   end Worker;

   protected Guard is
      procedure Do_Async_Select (W : access Worker);
      procedure Do_Task_Create;
      procedure Do_Timed_Call (W : access Worker);
      procedure Clean;

      -- Case-insensitive keywords, and "requeue ... with abort" (which
      -- pairs "with" and "abort", never "then" and "abort"), must not
      -- be mistaken for an asynchronous select statement.
      procedure Uppercase_Async_Select;
      procedure Do_Requeue (W : access Worker);
   end Guard;

   -- Not an abort-deferred operation: an ordinary subprogram may freely
   -- use an asynchronous select or create a task.
   procedure Ordinary_Subprogram;

end Test;
