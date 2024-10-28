entity T08_IfTb is
end entity;
 
architecture sim of T08_IfTb is
 
    signal NumOne     : integer := 0;
    signal NumTwo     : integer := 0;
    signal NumThree   : integer := 0;
    signal NumFour    : integer := 0;
    signal NumFive    : integer := 0;
    signal NumSix     : integer := 0;
    signal NumSeven   : integer := 0;
 
begin
 
    process is
    begin
 
        NumOne    <= NumOne + 1;
        NumTwo    <= NumTwo + 1;
        NumThree  <= NumThree + 1;
        NumFour   <= NumFour + 1;
        NumFive   <= NumFive + 1;
        NumSix    <= NumSix + 1;
        NumSeven  <= NumSeven + 1;
        wait for 10 ns;
 
    end process;
 
    process is
    begin
 
        if NumOne > 100 then
            if NumTwo > 100 then
                if NumThree > 100 then
                    if NumFour > 100 then
                        if NumFive > 100 then
                            if NumSix > 100 then
                                if NumSeven > 100 then -- UndCC_Violation
                                    report "All numbers are larger than 100";
                                end if;
                            end if;
                        end if;
                    end if;
                end if;
            end if;
        end if;
        
        wait on NumOne, NumTwo, NumThree, NumFour, NumFive, NumSix, NumSeven;
 
    end process;
 
end architecture;
