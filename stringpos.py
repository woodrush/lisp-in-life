from graphviz import Digraph

opstr = "eval atom quote macro lambda* define while progn lambda > < + - * / t mod print cons cdr car list eq if"


def addtree(op, tree=None):
    print(op, tree)
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

tree = None
for op in opstr.split(" "):
    tree = addtree(op, tree)

def setgraph(tree, dg):
    dg.node(tree["mid"])
    if tree["lesser"] is not None:
        dg.edge(tree["mid"], tree["lesser"]["mid"])
        setgraph(tree["lesser"], dg)
    if tree["greater"] is not None:
        dg.edge(tree["mid"], tree["greater"]["mid"])
        setgraph(tree["greater"], dg)

        
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


def breadthfirst_print(tree):
    queue = []
    queue.append(tree)
    while len(queue) > 0:
        nextqueue = []
        for item in queue:
            print(item["mid"], end=" ")
            if item["greater"] is not None:
                nextqueue.append(item["greater"])
            if item["lesser"] is not None:
                nextqueue.append(item["lesser"])
        queue = nextqueue
breadthfirst_print(tree)

dg = Digraph(format="png")
setgraph(tree, dg)
dg.render("opgraph")


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
print(opstr.replace(" ", "\\0"))
# print(poslist)
