library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
 
entity variable_vs_signal is
  port (
    i_clk   : in std_logic;
    o_var_done : out std_logic;
    o_sig_done : out std_logic
    );
end variable_vs_signal;
 
architecture rtl of variable_vs_signal is
 
  signal r_Var_Done : std_logic            := '0';
  signal r_Count    : std_logic                  ;  -- UndCC_Violation Signal is declared but not initialized
  signal r_Sig_Done : std_logic            := '0';
  signal test_Sig   : natural                    ;  -- UndCC_Violation Signal is initialized after being used (line 28)
   
begin
 
  VAR_VS_SIG : process (i_clk)
    variable v_Count : natural range 0 to 5 := 0;
  begin
    if rising_edge(i_clk) then
      v_Count := v_Count + 1;           -- Variable
      r_Count <= r_Count + 1;           -- Signal is used before initialization
      test_Sig <= test_Sig + 1;
      test_Sig := 0;                    -- Signal is set after being used
 
      -- Variable Checking
      if v_Count = 5 then
        r_Var_Done <= '1';
        v_Count := 0;
      else
        r_Var_Done <= '0';
      end if;
 
      -- Signal Checking
      if r_Count = 5 then
        r_Sig_Done <= '1';
        r_Count    <= 0;
      else
        r_Sig_Done <= '0';
      end if;
 
    end if;
  end process VAR_VS_SIG;
 
  o_var_done <= r_Var_Done;
  o_sig_done <= r_Sig_Done;
   
end rtl;
