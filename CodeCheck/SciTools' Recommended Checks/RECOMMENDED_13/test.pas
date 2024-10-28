(* function returning the max between two numbers *)
function max(num1, num2: integer): integer; (* UndCC_Violation(0) *)

var
   (* local variable declaration *)
   result: integer;

begin
   if (num1 > num2) then
      result := num1
   
   else
      result := num2;
   max := result;
end;
