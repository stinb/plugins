def sccList(successorFunc, roots):
  '''
  Find the strongly connected components reachable from roots using Tarjan's
  algorithm.
  https://en.wikipedia.org/wiki/Tarjan%27s_strongly_connected_components_algorithm

  successorFunc(node) returns an iterable of the nodes that node points to. It
  is called once per node, so it can also record per-node information such as
  self loops. Nodes must be hashable.

  Returns (components, nodeToComponent). Components are sets, listed in reverse
  topological order (a component comes before any component that points to it).
  nodeToComponent maps each node to its index in components.
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

  return components, nodeToComponent
