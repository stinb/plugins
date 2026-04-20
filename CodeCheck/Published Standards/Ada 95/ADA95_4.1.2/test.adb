-- Violations: configuration pragmas before a package body
pragma Normalize_Scalars;                    -- UndCC_Violation
pragma Restrictions (No_Dynamic_Allocation); -- UndCC_Violation
pragma Suppress (Range_Check);               -- UndCC_Violation

package body Test_4_1_2 is
end Test_4_1_2;
