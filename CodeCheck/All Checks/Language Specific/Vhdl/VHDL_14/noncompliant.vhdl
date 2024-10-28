library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
 
entity T21_TrafficLights is
generic(ClockFrequencyHz : natural);
port(
    Clk         : in std_logic;
    nRst        : in std_logic; -- Negative reset
    NorthRed    : out std_logic;
    NorthYellow : out std_logic;
    NorthGreen  : out std_logic;
    WestRed     : out std_logic;
    WestYellow  : out std_logic;
    WestGreen   : out std_logic);
end entity;
 
architecture rtl of T21_TrafficLights is
 
    -- Enumerated type declaration and state signal declaration
    type t_State is (NorthNext, StartNorth, North, StopNorth,
                        WestNext, StartWest, West, StopWest);
    signal State : t_State;
 
    -- Calculate the number of clock cycles in minutes/seconds
    function CounterVal(Minutes : integer := 0;  -- Violation: CounterVal has 9 input parameters -- UndCC_Violation
                        Seconds : integer := 0;
                        Empty_1 : integer := 0;
                        Empty_2 : integer := 0;
                        Empty_3 : integer := 0;
                        Empty_4 : integer := 0;
                        Empty_5 : integer := 0;
                        Empty_6 : integer := 0;
                        Empty_7 : integer := 0) return integer is
        variable TotalSeconds : integer;
    begin
        TotalSeconds := Seconds + Minutes * 60;
        return TotalSeconds * ClockFrequencyHz -1;
    end function;
 
    -- Counter for counting clock periods, 1 minute max
    signal Counter : integer range 0 to CounterVal(Minutes => 1) +1;
 
begin
 
    process(Clk) is
    begin
 
        if rising_edge(Clk) then
            if nRst = '0' then
                -- Reset values
                NorthRed    <= '1';
                NorthYellow <= '0';
                NorthGreen  <= '0';
                WestRed     <= '1';
                WestYellow  <= '0';
                WestGreen   <= '0';
                State       <= NorthNext;
                Counter     <= 0;
 
            else
                -- Default values
                NorthRed    <= '0';
                NorthYellow <= '0';
                NorthGreen  <= '0';
                WestRed     <= '0';
                WestYellow  <= '0';
                WestGreen   <= '0';
 
                Counter <= Counter + 1;
 
                case State is
 
                    -- Red light in all directions
                    when NorthNext =>
                        NorthRed <= '1';
                        WestRed  <= '1';
                        -- If 5 seconds have passed
                        if Counter = CounterVal(Seconds => 5) then
                            Counter <= 0;
                            State   <= StartNorth;
                        end if;
 
                    -- Yellow light in north/south directions
                    when StartNorth =>
                        NorthRed    <= '1';
                        NorthYellow <= '1';
                        WestRed     <= '1';
                        -- If 5 seconds have passed
                        if Counter = CounterVal(Seconds => 5) then
                            Counter <= 0;
                            State   <= North;
                        end if;
 
                    -- Green light in north/south directions
                    when North =>
                        NorthGreen <= '1';
                        WestRed    <= '1';
                        -- If 1 minute has passed
                        if Counter = CounterVal(Minutes => 1) then
                            Counter <= 0;
                            State   <= StopNorth;
                        end if;
 
                    -- Red and yellow light in north/south direction
                    when StopNorth =>
                        NorthYellow <= '1';
                        WestRed     <= '1';
                        -- If 5 seconds have passed
                        if Counter = CounterVal(Seconds => 5) then
                            Counter <= 0;
                            State   <= WestNext;
                        end if;
 
                    -- Red light in all directions
                    when WestNext =>
                        NorthRed <= '1';
                        WestRed  <= '1';
                        -- If 5 seconds have passedf
                        if Counter = CounterVal(Seconds => 5) then
                            Counter <= 0;
                            State   <= StartWest;
                        end if;
 
                    -- Yellow light in west/east direction
                    when StartWest =>
                        NorthRed   <= '1';
                        WestRed    <= '1';
                        WestYellow <= '1';
                        -- If 5 seconds have passed
                        if Counter = CounterVal(Seconds => 5) then
                            Counter <= 0;
                            State   <= West;
                        end if;
 
                    -- Green light in west/east direction
                    when West =>
                        NorthRed  <= '1';
                        WestGreen <= '1';
                        -- If 1 minute has passed
                        if Counter = CounterVal(Minutes => 1) then
                            Counter <= 0;
                            State   <= StopWest;
                        end if;
 
                    -- Red and yellow light in west/east direction
                    when StopWest =>
                        NorthRed   <= '1';
                        WestYellow <= '1';
                        -- If 5 seconds have passed
                        if Counter = CounterVal(Seconds => 5) then
                            Counter <= 0;
                            State   <= NorthNext;
                        end if;
 
                end case;
            end if;
        end if;
 
    end process;
 
end architecture;
