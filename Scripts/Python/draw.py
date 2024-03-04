
import argparse
import understand

def callDraw(object, args):
  try:
    if args.variant and args.options:
      object.draw(args.graph, args.out, options=args.options, variant=args.variant)
    elif args.variant:
      object.draw(args.graph, args.out, variant=args.variant)
    elif args.options:
      object.draw(args.graph, args.out, options=args.options)
    else:
      object.draw(args.graph, args.out)
  except Exception as e:
    print ("Error:", e)

def draw(args):
  entArchCnt = 0
  if args.entunique:
    entArchCnt += 1
  if args.entid:
    entArchCnt += 1
  if args.entsearch:
    entArchCnt += 1
  if args.arch:
    entArchCnt += 1

  if entArchCnt != 1:
    print("Error: must have exactly one of the following flags --entunique, --entid, --entsearch, --arch")
    return

  db = understand.open(args.db)
  if args.arch:
    a = db.lookup_arch(args.arch)
    if not a:
      print ("Error: unable to find architecture")
      return
    callDraw(a, args)
    return

  ent = None
  if args.entunique:
    ent = db.lookup_uniquename(args.entunique)
  elif args.entid:
    ent = db.ent_from_id(args.entid)
  elif args.entsearch:
    ents = db.lookup(args.entsearch[0], args.entsearch[1])
    if ents:
      ent = ents[0]

  if not ent:
    print ("Error: unable to find entity")
    return
  callDraw(ent,args)

parser = argparse.ArgumentParser(description="Draw a graph. Use --entunique, --entid, --entsearch, or --arch to specify an entity or arch to draw the graph for.")
parser.add_argument("--db", required=True, help="The understand database")
parser.add_argument("--graph", required=True, help="The name of the graph to generate")
parser.add_argument("-o","--out", required=True, help="The filename to save the graph to. The file format is determined by the suffix of the filename")

parser.add_argument("--variant", help="The graph variant to use, such as classic, simplified, or cluster. The default is determined user settings if not provided")
parser.add_argument("--options", help="A single string to pass to the options argument of the draw command. See the documentation for \"draw\"")

parser.add_argument("--entunique", help="The unique name of an entity to draw the graph for.")
parser.add_argument("--entid", help="The entity id to draw the graph for. Note that ids can change with every parse")
parser.add_argument("--entsearch", nargs=2, metavar=("NAME","KINDSTRING"), help="An entity name and kindstring to pass to lookup. The first matching entity will be used to draw the graph")
parser.add_argument("--arch", help="The long name of the architecture to draw the graph for")

args = parser.parse_args()

draw(args)
