library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity sequential_example is
    Port (
        clk           : in  STD_LOGIC;
        reset         : in  STD_LOGIC;
        someOtherStuff : in  STD_LOGIC;
        someMoreStuff  : in  STD_LOGIC;
        Get_ready     : out STD_LOGIC;
        Marwen        : out STD_LOGIC
    );
end sequential_example;

architecture Behavioral of sequential_example is
    signal Get_ready_internal : STD_LOGIC := '0';
    signal Marwen_internal    : STD_LOGIC := '0';
begin
    
    -- Connect internal signals to outputs
    Get_ready <= Get_ready_internal;
    Marwen    <= Marwen_internal;
    
    -- Sequential process with synchronous reset
    Process (clk) begin
        If rising_edge(clk) then
            If reset = '0' then
                Get_ready_internal <= '1';
                Marwen_internal    <= '1';
            Else
                If (someOtherStuff = '1') then
                    Get_ready_internal <= '1';
                Elsif (someMoreStuff = '1') then
                    Marwen_internal    <= '1';  -- UndCC_Valid, Marwen_internal only assigned within this one sequential process
                    Get_ready_internal <= '0';
                Else
                    Get_ready_internal <= '0';
                End if;
            End if;
        End if; -- rising edge
    End process;

end Behavioral;
