opstr = "define if quote car cdr cons atom print progn while lambda macro eval eq + - * / mod < > t "
offset = 11
poslist = [0+offset]
for i_c, c in enumerate(opstr):
    if c == " ":
        op = opstr.split(" ")[len(poslist)-1]
        print("#define {}_str {}".format(op, poslist[len(poslist)-1]))
        # print("#define {op}_str \"{op}\"".format(op=op))
        poslist.append(i_c+1+offset)
print(poslist)