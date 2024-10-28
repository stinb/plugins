library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

package Func1_Package is
  -- Function declaration
  function Func1 (A, B: in integer; MaxValue: in integer) return integer; -- UndCC_Violation(0)
end Func1_Package;

package body Func1_Package is
  -- Function implementation
  function Func1 (A, B: in integer; MaxValue: in integer) return integer is
    variable Temp, Max: integer;
  begin
    Temp:= A + B;
    if Temp > MaxValue then
      Max:= MaxValue;
    else
      Max:= Temp;
    end if;
    return Max;
  end Func1;
end Func1_Package;

