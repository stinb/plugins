-- for loop with label and without label

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity parity_check is
port(
  i_clk             : in  std_logic;
  i_data            : in  std_logic_vector(7 downto 0);
  o_parity          : out std_logic);
end parity_check;

architecture rtl of parity_check is

signal r_data            : std_logic_vector(7 downto 0);

begin

p_parity_check : process (i_clk)
variable vparity           : std_logic;
begin
  if rising_edge(i_clk) then
    r_data  <= i_data;

    vparity := '0';
    l_parity : for k in 0 to r_data'length-1 loop
      vparity := vparity xor r_data(k);
    end loop l_parity;
    
    o_parity <= vparity;
  end if;
end process p_parity_check;

end rtl;


architecture ExampleArch1 of ExampleEntity is
begin
  process -- UndCC_Violation
  begin
    -- Unlabeled for loop
    for i in 0 to 3 loop -- UndCC_Violation
      -- Your logic here, using the loop index 'i'
      report "Iteration " & integer'image(i);
      
      -- Additional logic
    end loop;
    
    -- More process logic
    
  end process;
  
  -- Other architecture statements
  
end ExampleArch1;


architecture ExampleArch of ExampleEntity is
begin
  process -- UndCC_Violation
  begin
    -- Labeled for loop
    outer_loop: for i in 0 to 2 loop
      inner_loop: for j in 0 to 2 loop
        -- Your logic here, using loop indices 'i' and 'j'
        report "Outer Iteration " & integer'image(i) & " Inner Iteration " & integer'image(j);
        
        -- Additional logic
      end loop inner_loop;
    end loop outer_loop;
    
    -- More process logic
    
  end process;
  
  -- Other architecture statements
  
end ExampleArch;
