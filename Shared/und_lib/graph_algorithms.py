def sccList(successorFunc, roots, calcCondensedGraph = False):
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
  nodeToComponent = dict()

  for root in roots:
    if root in indices:
      continue

    indices[root] = lowlink[root] = index
    index += 1
    stack.append(root)
    onStack.add(root)
    work = [(root, iter(successorFunc(root)))]

    while work:
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
        componentIdx = len(components)
        component = set()
        while True:
          n = stack.pop()
          onStack.remove(n)
          nodeToComponent[n] = componentIdx
          component.add(n)
          if n == node:
            break
        components.append(component)

  condensed = dict()
  if calcCondensedGraph:
    for i in range(len(components)):
      condensed[i] = set()

    for node, src in nodeToComponent.items():
      for to in successorFunc(node):
        dest = nodeToComponent[to]
        if src != dest:
          condensed[src].add(dest)

  return components, condensed

def layers(dagSuccessorFunc, topologicalOrder):
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

def reachability(dagSuccessorFunc, topologicalOrder):
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
    reachedBy = reachInto.setdefault(node, set())
    for toNode in dagSuccessorFunc(node):
      reachInto.setdefault(toNode, set()).update(reachedBy, (node,))
  return reachInto
