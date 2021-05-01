from graphviz import Digraph

opdata = [
    ("*",2),
    ("+",2),
    ("-",2),
    ("/",0),
    ("<",1),
    (">",0),
    ("atom",0),
    ("car",2),
    ("cdr",2),
    ("cons",3),
    ("define",19),
    ("eq",6),
    ("eval",0),
    ("if",8),
    ("lambda",14),
    ("lambda*",2),
    ("list",4),
    ("macro",2),
    ("mod",1),
    ("print",10),
    ("progn",4),
    ("quote",8),
    ("t",0),
    ("while",2),
]

opdata_all = [
    ("*",2),
    ("+",2),
    ("-",2),
    (".",10),
    ("/",0),
    ("<",1),
    (">",0),
    ("append",3),
    ("atom",0),
    ("body",2),
    ("car",2),
    ("cdr",2),
    ("cons",3),
    ("counter",3),
    ("counter1",6),
    ("counter2",5),
    ("dec",2),
    ("define",19),
    ("defun",3),
    ("eq",6),
    ("eval",0),
    ("f",3),
    ("fact",2),
    ("fname",2),
    ("get",1),
    ("if",8),
    ("inc",7),
    ("isprime",2),
    ("item",3),
    ("l",4),
    ("lambda",14),
    ("lambda*",2),
    ("list",4),
    ("m",2),
    ("macro",2),
    ("methodname",7),
    ("mod",1),
    ("n",19),
    ("new",3),
    ("nmax",2),
    ("object",2),
    ("p",3),
    ("primelist",11),
    ("print",10),
    ("progn",4),
    ("quote",8),
    ("ret",3),
    ("set",3),
    ("t",0),
    ("v",4),
    ("varlist",2),
    ("while",2),
    ("x",8),
]

ops, _ = zip(*opdata_all)

d_hash = {}
for op in ops:
    h = 0
    # for c in op:
    #     h = (h ^ ord(c))
    # h = h >> 2
    for c in op:
        h += ord(c)
    h &= 0b1111
    # h = h >> 2

    print("{}:{}".format(op, h))
    if h not in d_hash.keys():
        d_hash[h] = []
    d_hash[h].append(op)

for item in sorted(d_hash.items()):
    print(item)







strpointers = [
    ("createlambda",11), #("lambda",11),
    ("print",18), #("print",18),
    ("define",24), #("define",24),
    ("quote",31), #("quote",31),
    ("list",37), #("list",37),
    ("if",42), #("if",42),
    ("car",45), #("car",45),
    ("while",49), #("while",49),
    ("progn",55), #("progn",55),
    ("createlambda",61), #("macro",61),
    ("createlambda",67), #("lambdaast",67),
    ("eq",75), #("eq",75),
    ("cons",78), #("cons",78),
    ("arith",83), #("plus",83),
    (0,85), #("t",85),
    ("arith",87), #("mod",87),
    ("eval",91), #("eval",91),
    ("cdr",96), #("cdr",96),
    ("arith",100), #("minus",100),
    ("arith",102), #("ast",102),
    ("cmp",104), #("lt",104),
    ("cmp",106), #("gt",106),
    ("arith",108), #("slash",108),
    ("atom",110), #("atom",110),
]

# plist = [0 for _ in range((110-11)//2+1)]
plist = [0 for _ in range(110-11+1)]

# string, plist = zip(*strpointers)
curhead = 11
for op, p in strpointers:
    # plist[(p-11)//2] = "&&eval_{}".format(op);
    plist[(p-11)] = "&&eval_{}".format(op);
for item in plist:
    if item != 0:
        print()
    print("{}, ".format(item), end="")
print()
print()


# plist = [0 for _ in range((110-11)//2+1)]
plist = [0 for _ in range((110-11)//2 + 1)]

# string, plist = zip(*strpointers)
curhead = 11
for op, p in strpointers:
    print((p-11)//2)
    plist[(p-11)//2] = "eval_{}".format(op);
for item in plist:
    print("    .long {}".format(item))
print()


exit()


def getcost_and_opttree(opdata, showFreq=True):
    ops, freq = zip(*opdata)
    if showFreq:
        ops = ["{}({})".format(op,f) for op,f in opdata]

    cost = {}

    def getcost_and_opttree(i,j):
        if (i,j) in cost.keys():
            return cost[(i,j)]
        # if i < 0 or len(ops) <= j:
        #     return 0, None
        # if i == j:
        #     return 1, {
        #         "mid": ops[i],
        #         "lesser": None,
        #         "greater": None,
        #     }
        
        mincost = None
        mintree = None
        for k in range(i,j+1):
            if k == i:
                lcost, ltree = 0, None
            else:
                lcost, ltree = getcost_and_opttree(i,k-1)
            
            if k == j:
                rcost, rtree = 0, None
            else:
                rcost, rtree = getcost_and_opttree(k+1,j)
            curcost = sum(freq[i:j+1]) + lcost + rcost
            if mincost is None or curcost < mincost:
                curtree = {
                    "mid": ops[k],
                    "lesser": ltree,
                    "greater": rtree,
                }
                mincost, mintree = curcost, curtree
        cost[(i,j)] = mincost, mintree
        return mincost, mintree

    optcost, opttree = getcost_and_opttree(0, len(ops)-1)
    return optcost, opttree

def setgraph(tree, dg):
    dg.node(tree["mid"])
    if tree["lesser"] is not None:
        dg.edge(tree["mid"], tree["lesser"]["mid"])
        setgraph(tree["lesser"], dg)
    if tree["greater"] is not None:
        dg.edge(tree["mid"], tree["greater"]["mid"])
        setgraph(tree["greater"], dg)

optcost, opttree = getcost_and_opttree(opdata)
dg = Digraph(format="png")
setgraph(opttree, dg)
dg.render("optgraph")

optcost_all, opttree_all = getcost_and_opttree(opdata_all)
dg = Digraph(format="png")
setgraph(opttree_all, dg)
dg.render("optgraph_all")

# exit()

def addtree(op, tree=None):
    if tree is None:
        return {
            "mid": op,
            "greater": None,
            "lesser": None
        }
    elif op < tree["mid"]:
        return {
            "mid": tree["mid"],
            "greater": tree["greater"],
            "lesser": (
                addtree(op, tree["lesser"])
            ),
        }
    else:
        return {
            "mid": tree["mid"],
            "greater": (
                addtree(op, tree["greater"])
            ),
            "lesser": tree["lesser"],
        }

opstr = "eval atom quote macro lambda* define while progn lambda > < + - * / t mod print cons cdr car list eq if"
# opstr = "lambda * print + eq - if / > < progn mod macro while lambda* define list quote cons cdr car atom eval t"

print(" ".join(sorted(opstr.split(" "))))
# exit()

def buildtree_from_opstr(opstr_):
    tree = None
    for op in opstr_.split(" "):
        tree = addtree(op, tree)
    return tree

tree = buildtree_from_opstr(opstr)
dg = Digraph(format="png")
setgraph(tree, dg)
dg.render("opgraph_v1")


# opstr = "- cons while list t > macro if car print / + lambda define atom eq cdr mod * < progn lambda* quote eval"
ops = opstr.split(" ")
ops = sorted(ops)
# print(sorted(ops))


def divide(l):
    if len(l) == 0:
        return None
    # if len(l) == 1:
    #     return {
    #         "mid": l[0],
    #         "lesser": None,
    #         "greater": None
    #     }
    mid = len(l) // 2
    return {
        "mid": l[mid],
        "lesser": divide(l[:mid]),
        "greater": divide(l[mid+1:])
    }
tree = divide(ops)


def breadthfirst_str(tree):
    queue = []
    queue.append(tree)
    ret = ""
    while len(queue) > 0:
        nextqueue = []
        for item in queue:
            ret = ret + item["mid"] + " "
            if item["greater"] is not None:
                nextqueue.append(item["greater"])
            if item["lesser"] is not None:
                nextqueue.append(item["lesser"])
        queue = nextqueue
    return ret
print(breadthfirst_str(tree))

dg = Digraph(format="png")
setgraph(tree, dg)
dg.render("opgraph")


def printdefine(opstr):
    offset = 11
    poslist = [0+offset]
    for i_c, c in enumerate(opstr):
        if c == " ":
            op = opstr.split(" ")[len(poslist)-1]
            s = "#define {}_str {}".format(op, poslist[len(poslist)-1])
            s = s.replace("<", "lt")
            s = s.replace(">", "gt")
            s = s.replace("+", "plus")
            s = s.replace("-", "minus")
            s = s.replace("*", "ast")
            s = s.replace("/", "slash")
            print(s)
            # print("#define {op}_str \"{op}\"".format(op=op))
            poslist.append(i_c+1+offset)
# print(poslist)


reconst_str = breadthfirst_str(opttree)
reconst_tree = buildtree_from_opstr(reconst_str[:-1])
dg = Digraph(format="png")
setgraph(reconst_tree, dg)
dg.render("optgraph_reconst")


optcost, opttree = getcost_and_opttree(opdata, showFreq=False)
result = breadthfirst_str(opttree)
printdefine(result)
print(result[:-1].replace(" ", "\\x00"))