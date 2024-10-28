library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
 
entity wait_statement is
end wait_statement;
 
architecture behave of wait_statement is
 
  constant c_CLK_PERIOD : time := 10 ns;  -- 100 MHz
 
  signal r_RESET  : std_logic := '1';   -- start in reset
  signal r_CLK_TB : std_logic := '0';
   
begin
 
  -- Demonstrates: WAIT FOR
  -- Generates a clock that is used by the test bench.
  p_CLK : process
  begin
    r_CLK_TB <= not(r_CLK_TB);
    wait for c_CLK_PERIOD/2; -- UndCC_Violation
  end process p_CLK;
 
   
  -- Demonstrates: WAIT ON
  p_RESET : process
  begin
    wait on r_CLK_TB;                   -- look for first clock -- UndCC_Violation
 
    wait until r_CLK_TB'event and r_CLK_TB = '1'; -- UndCC_Violation
 
    r_RESET <= '0' after 40 ns;         -- relese reset
 
    wait;                               -- wait forever -- UndCC_Violation
  end process;
 
 
  -- Main Test Bench Process
  process is
  begin
 
    -- Demonstrates: WAIT UNTIL
    wait until r_RESET = '0'; -- UndCC_Violation
     
    wait;                               -- example of a wait forever -- UndCC_Violation
  end process;  
   
end behave;