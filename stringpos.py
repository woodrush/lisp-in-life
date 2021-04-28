from graphviz import Digraph

opstr = "eval atom quote macro lambda* define while progn lambda > < + - * / t mod print cons cdr car list eq if"
ops = opstr.split(" ")
print(sorted(ops))

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

dg = Digraph(format="png")

def setgraph(tree, dg):
    dg.node(tree["mid"])
    if tree["lesser"] is not None:
        dg.edge(tree["mid"], tree["lesser"]["mid"])
        setgraph(tree["lesser"], dg)
    if tree["greater"] is not None:
        dg.edge(tree["mid"], tree["greater"]["mid"])
        setgraph(tree["greater"], dg)

setgraph(tree, dg)

dg.render("opgraph")

exit()

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
