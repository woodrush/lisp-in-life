opstr = "eval closure atom quote macro define while progn lambda > < + - * / t mod print cons cdr car eq if "
offset = 10
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
