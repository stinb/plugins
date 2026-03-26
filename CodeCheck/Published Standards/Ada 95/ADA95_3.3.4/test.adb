------------------------------------------------------------------------
-- Autolayout
-- Implementation Notes:
-- - This package uses a heuristic algorithm to minimize the number
-- of arc crossings. It does not always achieve the true minimum
-- number which could theoretically be reached. However it does a
-- nearly perfect job in relatively little time. For details about
-- the algorithm, see ...
-- Portability Issues:
-- - The native math package Math_Lib is used for computations of
-- coordinate positions.
-- - 32-bit integers are required.
-- - No operating system specific routines are called.
-- Anticipated Changes:
-- - Coordinate_Type below could be changed from integer to float
-- with little effort. Care has been taken to not depend on the
-- specific characteristics of integer arithmetic.
------------------------------------------------------------------------

package body Test is

   ---------------------------------------------------------------------
   -- Define
   -- Implementation Notes:
   -- - This routine stores a node in the general purpose Graph data
   -- structure, not the Fast_Graph structure because ...
   ---------------------------------------------------------------------
   procedure Define (New_Node : in Node) is
   begin

   end Define;

   procedure Layout is -- UndCC_Violation
   begin

   end Layout;
   ---------------------------------------------------------------------
   -- Position_Of
   ---------------------------------------------------------------------
   function Position_Of (Current : in Node) return Position is
   begin

   end Position_Of;

end Test;
