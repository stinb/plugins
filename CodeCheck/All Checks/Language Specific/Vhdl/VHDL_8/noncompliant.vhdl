library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
 
entity variable_ex is
  port (
    i_clk   : in std_logic;
    o_done  : out std_logic
    );
end variable_ex;
 
architecture rtl of variable_ex is
 
  signal r_Done      : std_logic := '0';
  signal r_Var_Copy1 : natural range 0 to 5 := 0;
  signal r_Var_Copy2 : natural range 0 to 5 := 0;
   
begin
 
  EX_VAR : process (i_clk)
    variable v_Count : natural range 0 to 5 := 0;
  begin
    if rising_edge(i_clk) then
      v_Count := v_Count + 1;  -- UndCC_Violation - v_Count being incremented by a nondescript hard coded value
 
      r_Var_Copy1 <= v_Count;
 
      if v_Count = 5 then  -- UndCC_Violation - v_Count being compared to nondescript hard coded value
        r_Done  <= '1';
        v_Count := 0;
      else
        r_Done <= '0';
      end if;
 
      r_Var_Copy2 <= v_Count;
 
    end if;
  end process EX_VAR;
   
  o_done <= r_Done;
   
end rtl;
