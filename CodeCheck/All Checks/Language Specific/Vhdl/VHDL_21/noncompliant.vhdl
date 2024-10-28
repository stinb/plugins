library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
 
entity T14_CaseWhenTb is
end entity;
 
architecture sim of T14_CaseWhenTb is
 
    signal Sig1 : unsigned(7 downto 0) := x"AA";
    signal Sig2 : unsigned(7 downto 0) := x"BB";
    signal Sig3 : unsigned(7 downto 0) := x"CC";
    signal Sig4 : unsigned(7 downto 0) := x"DD";
 
    signal Sel : unsigned(1 downto 0) := (others => '0');
 
    signal Output1 : unsigned(7 downto 0);
    signal Output2 : unsigned(7 downto 0);
 
begin
 
    -- Stimuli for the selector signal
    process is
    begin
        wait for 10 ns;
        Sel <= Sel + 1;
        wait for 10 ns;
        Sel <= Sel + 1;
        wait for 10 ns;
        Sel <= Sel + 1;
        wait for 10 ns;
        Sel <= Sel + 1;
        wait for 10 ns;
        Sel <= "UU";
        wait;
    end process;
 
    -- MUX using if-then-else
    process(Sel, Sig1, Sig2, Sig3, Sig4) is
    begin
 
        if Sel = "00" then
            Output1 <= Sig1;
        elsif Sel = "01" then
            Output1 <= Sig2;
        elsif Sel = "10" then
            Output1 <= Sig3;
        elsif Sel = "11" then
            Output1 <= Sig4;
        else -- 'U', 'X', '-' etc.
            Output1 <= (others => 'X');
        end if;
 
    end process;
 
    -- Equivalent MUX using a case statement
    process(Sel, Sig1, Sig2, Sig3, Sig4) is
    begin
 
        case Sel is
            when "00" =>  -- Violation -- UndCC_Violation
                Output2 <= Sig1;
                Output2 <= Sig2;
            when "01" =>
                Output2 <= Sig2;
            when "10" =>
                Output2 <= Sig3;
            when "11" =>
                Output2 <= Sig4;
            when others => -- 'U', 'X', '-', etc. -- violation -- UndCC_Violation
                Output2 <= (others => 'X');
                Output2 <= Sig3;
        end case;
 
    end process;
 
end architecture;
