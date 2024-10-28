--infer 4-bit wide latch
LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
USE ieee.std_logic_vector_arith.ALL;

ENTITY storage IS
    PORT (
        data_in : IN STD_LOGIC_VECTOR(3 DOWNTO 0);
        data_out : OUT STD_LOGIC_VECTOR(3 DOWNTO 0);
        enable : IN STD_LOGIC);
END storage;

ARCHITECTURE wed OF storage IS
    BEGIN
    infer_latch:
    PROCESS (enable, data_in)
    BEGIN
        IF enable = ‘1’ THEN  -- Noncompliant, no corresponding else -- UndCC_Violation
            data_out <= data_in;
        END IF;
        
        IF enable = ‘1’ THEN  -- Compliant
            data_out <= data_in;
        ELSE
          report "Else Found";
        END IF;
    END PROCESS infer_latch;
END ARCHITECTURE wed;
