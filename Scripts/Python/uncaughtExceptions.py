#Associate all exceptions derived from a base type to the top level function in the call cone from all throw points.
#Guruprasad Rao

def get_callers(func):
    """
    Get all callers of a function. Handle virtual calls.
    Returns a list of ids.
    """
    ents = [x.id() for x in func.ents("callby", "function")]
    if not ents:
        overrs = func.refs("overrides", "", True)
        refs = [i.ent().refs("callby virtual", "function,method", True) for i in overrs]
        ents = [x.ent().id() for y in refs for x in y]
    return ents
 
def find_top_of_call_chains(db, called_function, path=[], cache={}, depth=0):
    """
    Find the top functions in the call chains from an entity.
    Returns a list of ids
    """
    tops = set()
   
    if called_function.id() in cache:
        return cache[called_function.id()]
   
    ids = get_callers(called_function)
    for cf_id in ids:
        cf = db.ent_from_id(cf_id)
        if 'Function' not in cf.kindname():
            continue
        if cf_id in path:
            continue
        for i in find_top_of_call_chains(db, cf, path+[cf_id], cache, depth+1):
            tops.add(i)
           
    if len(ids) == 0:
        tops.add(called_function.id())
    cache[called_function.id()] = tops
    return tops
   
def find_derived_classes(base, graph=set()):
    """
    Find all derived classes of base.
    The graph is recorded in paramater 'graph'.
    Elements in the set are pairs with (from, to) edges.
    """
    for ent in base.ents('derive'):
        if ent.kindname() == 'Class':
            graph.add((base.id(), ent.id()))
            find_derived_classes(ent, graph)
 
def find_exception_associations(db, base, outfname=None):
    """
    Given a base class, find all its derived classes.
    For each class in the derivation graph, find 'Throw' references.
    For each reference, find the top of the call chain.
    Record in a file.
    """
    if outfname:
        outf = open(outfname, 'w')
    else:
        outf = sys.stdout
    graph = set()
    find_derived_classes(base, graph)
    nodes = set()
    for (fr, to) in graph:
        nodes.add(fr)
        nodes.add(to)
 
    cache = {}
    Tops = {}
    for node in nodes:
        ent = db.ent_from_id(node)
        throw_locs = [r for r in ent.refs() if 'Throw' in r.kindname()]
        for t in throw_locs:
            Tops[node] = find_top_of_call_chains(db, t.ent(), [t.ent().id()], cache)
            outf.write("* %s (%s:%d)\n" % (ent.longname(), t.file().longname(), t.line()))
            for n in Tops[node]:
                lent = db.ent_from_id(n)
                outf.write("** %s(%s)\n" % (lent.longname(), lent.parameters()))
            outf.write('\n\n')
           