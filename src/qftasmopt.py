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


def readinst (inst):
    if len(inst) == 5:
        return tuple(inst) + ("",)
    return inst    


# Constant folding and Mov folding
reg_value = {}
reg_value_dstedge = {}

def add_dst_to_src(reg_dst, reg_src):
    if reg_src not in reg_value_dstedge.keys():
        reg_value_dstedge[reg_src] = []
    reg_value_dstedge[reg_src].append(reg_dst)

def remove_dependent_reference(reg):
    if reg in reg_value_dstedge.keys():
        for dst in reg_value_dstedge[reg]:
            if dst in reg_value.keys():
                del reg_value[dst]

for i_inst, inst in enumerate(rom):
    lineno, opcode, (mode_1, d1), (mode_2, d2), (mode_3, d3), comment = readinst(inst)

    if mode_3 == 0 and d3 == 0 and not (mode_1 == 0 and d1 == 0 and opcode == "MNZ"):
        reg_value = {}
        reg_value_dstedge = {}
        # print("Detected jump")
        # print()

    if mode_1 > 0:
        if d1 in reg_value.keys():
            # print("Mov folding is possible at line {}, with a value from {}".format(lineno, reg_value[d1]))
            _, (mode_table, dtable) = reg_value[d1]
            rom[i_inst] = lineno, opcode, (mode_1 - 1 + mode_table, dtable), (mode_2, d2), (mode_3, d3), comment

    if mode_2 > 0:
        if d2 in reg_value.keys():
            # print("Mov folding is possible at line {}, with a value from {}".format(lineno, reg_value[d2]))
            _, (mode_table, dtable) = reg_value[d2]
            rom[i_inst] = lineno, opcode, (mode_1, d1), (mode_2 - 1 + mode_table, dtable), (mode_3, d3), comment

    if mode_3 > 0:
        if d3 in reg_value.keys():
            # print("Mov folding is possible at line {}, with a value from {}".format(lineno, reg_value[d3]))
            _, (mode_table, dtable) = reg_value[d3]
            rom[i_inst] = lineno, opcode, (mode_1, d1), (mode_2, d2), (mode_3 - 1 + mode_table, dtable), comment


    if mode_3 == 0 and d3 != 0:
        if opcode == "MNZ" and mode_1 == 0 and mode_2 <= 1 and not (d1 == 0):
            reg_value[d3] = lineno, (mode_2, d2)
            if mode_2 == 1:
                add_dst_to_src(d3, d2)
        elif opcode == "MLZ" and mode_1 == 0 and mode_2 == 0 and not (type(d2) == int and (0 <= d2 < 32768)):
            reg_value[d3] = lineno, (mode_2, d2)
            if mode_2 == 1:
                add_dst_to_src(d3, d2)
        elif opcode == "ADD" and mode_1 == 0 and mode_2 == 0:
            reg_value[d3] = lineno, (mode_2, d1 + d2)
        elif opcode == "SUB" and mode_1 == 0 and mode_2 == 0:
            reg_value[d3] = lineno, (mode_2, d1 - d2)
        else:
            if d3 in reg_value.keys():
                del reg_value[d3]
        remove_dependent_reference(d3)

    if "pc ==" in comment:
        reg_value = {}
        reg_value_dstedge = {}




unused_lines = []

# Dead code elimination
reg_fresh = {}

for inst in rom:
    lineno, opcode, (mode_1, d1), (mode_2, d2), (mode_3, d3), comment = readinst(inst)

    if mode_3 == 0 and d3 == 0 and not (mode_1 == 0 and d1 == 0 and mode_2 == 0 and d2 == 0 and opcode == "MNZ"):
        reg_fresh = {}
        # print("Detected jump")
        # print()

    if mode_1 > 0:
        reg_fresh[d1] = False

    if mode_2 > 0:
        reg_fresh[d2] = False

    if mode_3 > 0:
        reg_fresh[d3] = False
    elif not (opcode == "MNZ" and mode_1 == 0 and d1 == 0):
        if (
            not (opcode == "MLZ")
            and d3 in reg_fresh.keys()
            and reg_fresh[d3] != False
           ):  # `!= False`, since reg_fresh[d3] may be 0
            # print("Unused line: {} (overwriten at {})".format(reg_fresh[d3], lineno))

            unused_lines.append(reg_fresh[d3])
            # if "pc ==" in comment:
            #     print("Error: Unused line with a pc tag!", lineno)
            #     exit(1)

        reg_fresh[d3] = lineno

modedict = {
    0 : "",
    1 : "A",
    2 : "B",
    3 : "C",
}

i_effective_line = 0

for i_inst, inst in enumerate(rom):
    lineno, opcode, (mode_1, d1), (mode_2, d2), (mode_3, d3), comment = readinst(inst)
    if lineno in unused_lines:
        if "pc ==" in comment:
            print(comment, end="")
        continue

    if 0 < i_inst:
        print()

    print("{}. {} {}{} {}{} {}{}{}".format(
        i_effective_line, opcode,
        modedict[mode_1], d1,
        modedict[mode_2], d2,
        modedict[mode_3], d3,
        comment
    ), end="")

    i_effective_line += 1