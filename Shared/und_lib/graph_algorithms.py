def sccList(successorFunc, roots, calcCondensedGraph = False, isAborted=None):
  '''
  Find the strongly connected components reachable from roots using Tarjan's
  algorithm.
  https://en.wikipedia.org/wiki/Tarjan%27s_strongly_connected_components_algorithm

  successorFunc(node) returns an iterable of the nodes that node points to. It
  is called once per node (twice if calcCondensedGraph is True), so it can also
  record per-node information such as self loops. Nodes must be hashable.

  Returns (components, condensedGraph). Components are sets, listed in reverse
  topological order (a component comes before any component that points to it).
  condensedGraph is a dict from src component index to sets of destination
  component indices, and is only built if calcCondensedGraph is True. Calculating
  it requires a second pass over the graph.
  '''
  # Iterative, since Python's recursion limit is hit by DFS paths over ~1000 nodes
  index = 0
  indices = dict()
  lowlink = dict()
  stack = list()
  onStack = set()
  components = list()

  for root in roots:
    if root in indices:
      continue

    indices[root] = lowlink[root] = index
    index += 1
    stack.append(root)
    onStack.add(root)
    work = [(root, iter(successorFunc(root)))]

    while work:
      if isAborted is not None and isAborted():
        return list(), dict()
      node, successors = work[-1]
      descended = False
      for toNode in successors:
        if toNode not in indices:
          indices[toNode] = lowlink[toNode] = index
          index += 1
          stack.append(toNode)
          onStack.add(toNode)
          work.append((toNode, iter(successorFunc(toNode))))
          descended = True
          break
        elif toNode in onStack:
          lowlink[node] = min(lowlink[node], indices[toNode])
      if descended:
        continue

      work.pop()
      if work:
        parent = work[-1][0]
        lowlink[parent] = min(lowlink[parent], lowlink[node])

      if lowlink[node] == indices[node]:
        component = set()
        while True:
          n = stack.pop()
          onStack.remove(n)
          component.add(n)
          if n == node:
            break
        components.append(component)

  condensed = dict()
  if calcCondensedGraph:
    condensed = condense(successorFunc, components, isAborted)
    if isAborted is not None and isAborted():
      return list(), dict()

  return components, condensed

def condense(successorFunc, groups, isAborted=None):
  '''
  Build the graph between groups of nodes, where group a points to group b if
  any node in a points to any node in b.

  successorFunc(node) returns an iterable of the nodes that node points to. It
  is called once per node. groups is a list of disjoint iterables of nodes.
  Edges to nodes outside of every group, and edges within a group, are left
  out. Nodes must be hashable.

  Returns a dict from each group index to the set of group indices it points
  to, or an empty dict if aborted.
  '''
  groupOf = dict()
  for idx, group in enumerate(groups):
    for node in group:
      groupOf[node] = idx

  condensed = {idx: set() for idx in range(len(groups))}
  for node, src in groupOf.items():
    if isAborted is not None and isAborted():
      return dict()
    for to in successorFunc(node):
      dest = groupOf.get(to)
      if dest is not None and dest != src:
        condensed[src].add(dest)
  return condensed

def incoming(successorFunc, nodes, isAborted=None):
  '''
  Count, for each node in nodes, how many other nodes in nodes point to it.

  successorFunc(node) returns an iterable of the nodes that node points to. It
  is called once per node, and edges to nodes outside of nodes are left out.
  Nodes must be hashable.

  Returns a dict from each node to its count, or an empty dict if aborted.
  '''
  counts = {node: 0 for node in nodes}
  for node in counts:
    if isAborted is not None and isAborted():
      return dict()
    for to in set(successorFunc(node)):
      if to != node and to in counts:
        counts[to] += 1
  return counts

def disjointParts(successorFunc, roots, isAborted=None):
  '''
  Find the weakly connected components reachable from roots: groups of nodes
  linked by edges in either direction. Uses union-find, so edges are only
  followed forwards and no reverse adjacency is built.
  https://en.wikipedia.org/wiki/Disjoint-set_data_structure

  successorFunc(node) returns an iterable of the nodes that node points to. It
  is called once per node. Nodes must be hashable.

  Returns a list of sets, in no particular order.
  '''
  parent = dict()

  def find(x):
    while parent[x] != x:
      parent[x] = parent[parent[x]]
      x = parent[x]
    return x

  pending = list()
  for root in roots:
    if root not in parent:
      parent[root] = root
      pending.append(root)

  while pending:
    if isAborted is not None and isAborted():
      return list()
    node = pending.pop()
    for toNode in successorFunc(node):
      if toNode not in parent:
        parent[toNode] = toNode
        pending.append(toNode)
      a, b = find(node), find(toNode)
      if a != b:
        parent[a] = b

  parts = dict()
  for node in parent:
    parts.setdefault(find(node), set()).add(node)
  return list(parts.values())

def layers(dagSuccessorFunc, topologicalOrder, isAborted=None):
  '''
  Return a list of sets of nodes forming layers in the graph. Index 0 is the
  lowest layer, and nodes with no incoming dependencies are in the last layer.
  Dependencies always go from a higher layer to a lower one, possibly skipping
  layers.

  The graph must be a directed acyclic graph (dag), and dagSuccessorFunc(node)
  should return an iterable of the nodes that node points to.
  topologicalOrder must list every node, with sources first. For the condensed
  graph from sccList, use range(len(components) - 1, -1, -1).
  '''
  depth = dict()
  for node in topologicalOrder:
    if isAborted is not None and isAborted():
      return []
    d = depth.setdefault(node, 0)
    for toNode in dagSuccessorFunc(node):
      depth[toNode] = max(d + 1, depth.get(toNode, 0))

  if not depth:
    return []

  maxDepth = max(depth.values())
  layerList = [set() for _ in range(maxDepth + 1)]
  for node, d in depth.items():
    layerList[maxDepth - d].add(node)
  return layerList

def reachability(dagSuccessorFunc, topologicalOrder, isAborted = None):
  '''
  Return a dictionary from a node to the set of all nodes that can reach it,
  transitively.

  The graph must be a directed acyclic graph (dag), and dagSuccessorFunc(node)
  should return an iterable of the nodes that node points to.
  topologicalOrder must list every node, with sources first. For the condensed
  graph from sccList, use range(len(components) - 1, -1, -1).
  '''
  reachInto = {}
  for node in topologicalOrder:
    if isAborted is not None and isAborted():
      return dict()
    reachedBy = reachInto.setdefault(node, set())
    for toNode in dagSuccessorFunc(node):
      reachInto.setdefault(toNode, set()).update(reachedBy, (node,))
  return reachInto
