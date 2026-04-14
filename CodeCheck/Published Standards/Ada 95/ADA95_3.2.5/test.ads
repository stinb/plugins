package Test_3_2_5 is

   type Stack is private;
   type List  is private;

   -- Valid: predicate at start
   function Is_Empty         (S : Stack) return Boolean;  -- UndCC_Valid
   function Is_Full          (S : Stack) return Boolean;  -- UndCC_Valid
   function Is_Last_Item     (S : Stack) return Boolean;  -- UndCC_Valid
   function Has_Space        (S : Stack) return Boolean;  -- UndCC_Valid
   function Can_Accept       (S : Stack) return Boolean;  -- UndCC_Valid
   function Will_Overflow    (S : Stack) return Boolean;  -- UndCC_Valid
   function Was_Modified     (S : Stack) return Boolean;  -- UndCC_Valid
   function Should_Retry     return Boolean;              -- UndCC_Valid
   function Not_Ready        return Boolean;              -- UndCC_Valid

   -- Valid: predicate in the middle of the name
   function List_Is_Empty    (L : List)  return Boolean;  -- UndCC_Valid
   function Stack_Has_Space  (S : Stack) return Boolean;  -- UndCC_Valid
   function User_Can_Login   return Boolean;              -- UndCC_Valid
   function Token_Was_Seen   return Boolean;              -- UndCC_Valid

   -- Violation: adjective with no predicate auxiliary
   function Empty            (S : Stack) return Boolean;  -- UndCC_Violation
   function Full             (S : Stack) return Boolean;  -- UndCC_Violation
   function Ready            return Boolean;              -- UndCC_Violation
   function Active           return Boolean;              -- UndCC_Violation

   -- Violation: noun with no predicate auxiliary
   function Status           return Boolean;              -- UndCC_Violation
   function Error_Check      return Boolean;              -- UndCC_Violation
   function Availability     return Boolean;              -- UndCC_Violation

   -- Non-Boolean functions: must NOT be flagged regardless of name
   function Top              (S : Stack) return Integer;  -- UndCC_Valid
   function Length           (L : List)  return Integer;  -- UndCC_Valid
   function Successor        (N : Integer) return Integer; -- UndCC_Valid

private
   type Stack is null record;
   type List  is null record;

end Test_3_2_5;
