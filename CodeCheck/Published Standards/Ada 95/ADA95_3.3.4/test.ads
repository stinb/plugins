------------------------------------------------------------------------
-- AUTOLAYOUT
-- Purpose:
-- This package computes positional information for nodes and arcs
-- of a directed graph. It encapsulates a layout algorithm which is
-- designed to minimize the number of crossing arcs and to emphasize
-- the primary direction of arc flow through the graph.
-- Effects:
-- - The expected usage is:
-- 1. Call Define for each node and arc to define the graph.
-- 2. Call Layout to assign positions to all nodes and arcs.
-- 3. Call Position_Of for each node and arc to determine the
-- assigned coordinate positions.
-- - Layout can be called multiple times, and recomputes the
-- positions of all currently defined nodes and arcs each time.
-- - Once a node or arc has been defined, it remains defined until
-- Clear is called to delete all nodes and arcs.
-- Performance:
-- This package has been optimized for time, in preference to space.
-- Layout times are on the order of N*log(N) where N is the number
-- of nodes, but memory space is used inefficiently.
------------------------------------------------------------------------

package Test is
   ---------------------------------------------------------------------
   -- Define
   -- Purpose:
   -- This procedure defines one node of the current graph.
   -- Exceptions:
   -- Node_Already_Defined
   ---------------------------------------------------------------------
   procedure Define (New_Node : in Node);
   ---------------------------------------------------------------------
   -- Layout
   -- Purpose:
   -- This procedure assigns coordinate positions to all defined
   -- nodes and arcs.
   -- Exceptions:
   -- None.
   ---------------------------------------------------------------------
   procedure Layout;
   ---------------------------------------------------------------------
   -- Position_Of
   -- Purpose:
   -- This function returns the coordinate position of the
   -- specified node. The default position (0,0) is returned if no
   -- position has been assigned yet.
   -- Exceptions:
   -- Node_Not_Defined
   ---------------------------------------------------------------------
   function Position_Of (Current : in Node) return Position;
end Test;
