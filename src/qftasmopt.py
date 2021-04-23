from pyparsing import *
import sys
import re

class Parser(object):
    def __init__(self):
        addressing_mode = Optional(Char("ABC")).setParseAction(
            lambda t: {"A": 1, "B": 2, "C": 3}[t[0]] if len(t) > 0 else 0
        )
        integer = (Optional("-") + Word(nums)).setParseAction(
            lambda t: int("".join(t))
        )
        ppliteral = (Literal("{") + CharsNotIn("}") + Literal("}")).setParseAction(
            lambda t: [[0, "".join(t)]]
        )
        operand = Group(addressing_mode + integer) | ppliteral
        MNZ = Literal("MNZ")
        MLZ = Literal("MLZ")
        ADD = Literal("ADD")
        SUB = Literal("SUB")
        AND = Literal("AND")
        OR = Literal("OR")
        XOR = Literal("XOR")
        ANT = Literal("ANT")
        SL = Literal("SL")
        SRL = Literal("SRL")
        SRA = Literal("SRA")
        opcode = MNZ | MLZ | ADD | SUB | AND | OR | XOR | ANT | SL | SRL | SRA
        lineno = (integer + Literal(".")).setParseAction(
            lambda t: [int(t[0])]
        )
        comment = (Literal(";") + restOfLine).setParseAction(
            lambda t: ["".join(t)]
        )
        inst = (lineno + opcode + operand + operand + operand + Optional(comment)).setParseAction(
            lambda t: [t]
        )
        self.program = ZeroOrMore(inst)

    def parse_string(self, string):
        return self.program.parseString(string)


filepath = sys.argv[1]

parser = Parser()

def dstr(mode, d):
    if mode == 0:
        return "const"
    else:
        return (mode, d)

with open(filepath, "rt") as f:
    rom_lines = f.readlines()
rom = [parser.parse_string(inst)[0] for inst in rom_lines]

n_registers = 11

# reg_updated = {}
# reg_referenced = {}
reg_fresh = {}


unused_lines = []

def readinst (inst):
    if len(inst) == 5:
        return tuple(inst) + ("",)
    return inst

# Constant folding
for i_inst, inst in enumerate(rom):
    lineno, opcode, (mode_1, d1), (mode_2, d2), (mode_3, d3), comment = readinst(inst)

    if mode_3 == 0 and d3 == 0 and not (mode_1 == 0 and d1 == 0 and mode_2 == 0 and d2 == 0 and opcode == "MNZ"):
        reg_fresh = {}
        # print("Detected jump")
        # print()


    if mode_1 > 0:
        if d1 in reg_fresh.keys() and reg_fresh[d1][1] == "const":
            # print("Constant folding is possible at line {}, with a value from {}".format(lineno, reg_fresh[d1]))
            rom[i_inst] = lineno, opcode, (mode_1 - 1, reg_fresh[d1][2]), (mode_2, d2), (mode_3, d3), comment
        
        reg_fresh[d1] = False, None

    if mode_2 > 0:
        if d2 in reg_fresh.keys() and reg_fresh[d2][1] == "const":
            # print("Constant folding is possible at line {}, with a value from {}".format(lineno, reg_fresh[d2]))
            rom[i_inst] = lineno, opcode, (mode_1, d1), (mode_2 - 1, reg_fresh[d2][2]), (mode_3, d3), comment
        reg_fresh[d2] = False, None

    if mode_3 > 0:
        if d3 in reg_fresh.keys() and reg_fresh[d3][1] == "const":
            # print("Constant folding is possible at line {}, with a value from {}".format(lineno, reg_fresh[d3]))
            rom[i_inst] = lineno, opcode, (mode_1, d1), (mode_2, d2), (mode_3 - 1, reg_fresh[d3][2]), comment
        reg_fresh[d3] = False, None


    if mode_3 == 0:
        if (opcode == "MNZ" or opcode == "MLZ") and mode_2 == 0 and not (mode_1 == 0 and d1 == 0):
            reg_fresh[d3] = lineno, "const", d2
        elif opcode == "ADD" and mode_1 == 0 and mode_2 == 0:
            reg_fresh[d3] = lineno, "const", (d1 + d2)
        elif opcode == "SUB" and mode_1 == 0 and mode_2 == 0:
            reg_fresh[d3] = lineno, "const", (d1 - d2)
        else:
            reg_fresh[d3] = lineno, "reg", None

    # print(lineno, reg_fresh)


# # Remove dead lines
# for inst, line in zip(rom, rom_lines):
#     lineno, opcode, (mode_1, d1), (mode_2, d2), (mode_3, d3), comment = readinst(inst)

#     cur_updated = []
#     cur_referenced = []

#     def printinst(*args):
#         pass
#         # print(lineno, *args)

#     if opcode == "MNZ":
#         # MOV
#         if mode_1 == 0 and d1 == 32768 and mode_3 == 0 and d3 == 0:
#             # opcode = "JMP"
#             printinst("{} goto {}".format(opcode, dstr(mode_2, d2)))
            
#         elif mode_1 == 0 and d1 == 32768:
#             # opcode = "MOV"
#             printinst("{} {}->{}".format(opcode, dstr(mode_2, d2), (mode_3, d3)))

#         elif mode_3 == 0 and d3 == 0 and mode_2 == 0 and d2 == 0 and mode_1 == 0 and d1 == 0:
#             pass

#         elif mode_3 == 0 and d3 == 0:
#             # opcode = "JMP"
#             printinst("{} if {} goto {}".format(opcode, dstr(mode_1, d1), dstr(mode_2, d2)))
        
#         else:
#             printinst("{} if {} then {}->{}".format(opcode, dstr(mode_1, d1), dstr(mode_2, d2), (mode_3, d3)))

#     elif opcode == "MLZ":
#         if mode_3 == 0 and d3 == 0:
#             # opcode = "MLZJMP"
#             printinst("{} if {} goto {}".format(opcode, dstr(mode_1, d1), dstr(mode_2, d2)))

#         else:
#             printinst("{} if {} then {}->{}".format(opcode, dstr(mode_1, d1), dstr(mode_2, d2), (mode_3, d3)))

#     elif opcode == "ADD" or opcode == "SUB":
#         if mode_3 == 0 and d3 == 0:
#             # opcode = "JMP"
#             printinst("{} goto {} . {}".format(opcode, dstr(mode_1, d1), dstr(mode_2, d2)))
#         else:
#             printinst(opcode, dstr(mode_1, d1), dstr(mode_2, d2), "->", (mode_3, d3))

#     else:
#         printinst("Unknown instruction!")
#         # printinst(opcode, dstr(mode_1, d1), dstr(mode_2, d2), (mode_3, d3))

#     if mode_3 == 0 and d3 == 0 and not (mode_1 == 0 and d1 == 0 and mode_2 == 0 and d2 == 0 and opcode == "MNZ"):
#         reg_fresh = {}
#         # print("Detected jump")
#         # print()


#     if mode_1 > 0:
#         if d1 in reg_fresh.keys() and reg_fresh[d1][1] == "const":
#             print("Constant folding is possible at line {}, with a value from {}".format(lineno, reg_fresh[d1]))

#         reg_fresh[d1] = False, None

#     if mode_2 > 0:
#         if d2 in reg_fresh.keys() and reg_fresh[d2][1] == "const":
#             print("Constant folding is possible at line {}, with a value from {}".format(lineno, reg_fresh[d2]))
#         reg_fresh[d2] = False, None

#     if mode_3 > 0:
#         if d3 in reg_fresh.keys() and reg_fresh[d3][1] == "const":
#             print("Constant folding is possible at line {}, with a value from {}".format(lineno, reg_fresh[d3]))
#         reg_fresh[d3] = False, None
#     else:
#         if (
#             not (opcode == "MNZ" and (mode_1 == 0 and d1 == 0) or (opcode == "MLZ"))
#             and d3 in reg_fresh.keys()
#             and reg_fresh[d3] != False
#            ):  # `!= False`, since reg_fresh[d3] may be 0
#             # print("Unused line: {} (overwriten at {})".format(reg_fresh[d3], lineno))
#             unused_lines.append(reg_fresh[d3][0])

#         if "pc ==" not in line:
#             if (opcode == "MNZ" or opcode == "MLZ") and mode_2 == 0:
#                 reg_fresh[d3] = lineno, "const"
#             else:
#                 reg_fresh[d3] = lineno, "reg"

modedict = {
    0 : "",
    1 : "A",
    2 : "B",
    3 : "C",
}

for inst in rom:
    lineno, opcode, (mode_1, d1), (mode_2, d2), (mode_3, d3), comment = readinst(inst)
    print("{}. {} {}{} {}{} {}{}".format(
        lineno, opcode,
        modedict[mode_1], d1,
        modedict[mode_2], d2,
        modedict[mode_3], d3,
    ))

# i_effective_line = 0
# for line, inst in zip(rom_lines, rom):
#     lineno, opcode, (mode_1, d1), (mode_2, d2), (mode_3, d3) = inst
#     if lineno in unused_lines:
#         continue

#     print(re.sub(r"^\d+\.", str(i_effective_line) + ".", line), end="")
#     i_effective_line += 1
