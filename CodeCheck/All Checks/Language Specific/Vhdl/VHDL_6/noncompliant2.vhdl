library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity Mux_2to1 is
    Port ( data0 : in STD_LOGIC;
           data1 : in STD_LOGIC;
           sel : in STD_LOGIC;
           output : out STD_LOGIC);
end Mux_2to1;

architecture Behavioral of Mux_2to1 is
begin
    process(sel)
    begin
        if sel = '0' then
            output <= data0;
        -- No explicit assignment for sel = '1'
        end if;
    end process;
end Behavioral;
