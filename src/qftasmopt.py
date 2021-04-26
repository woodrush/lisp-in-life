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
            lambda t: ["".join(t)]
        )
        operand = Group(addressing_mode + integer) | Group(addressing_mode + ppliteral)
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
        SRU = Literal("SRU")
        SRE = Literal("SRE")
        opcode = MNZ | MLZ | ADD | SUB | AND | OR | XOR | ANT | SL | SRL | SRA | SRU | SRE
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
header = [inst for inst in rom_lines if inst.startswith(";;")]

rom = [parser.parse_string(inst)[0] for inst in rom_lines if not inst.startswith(";;")]

# for inst in rom_lines:
#     if not inst.startswith(";;"):
#         print(inst)
#         print(parser.parse_string(inst))
#         print(parser.parse_string(inst)[0])

n_registers = 11

# reg_updated = {}
# reg_referenced = {}


def readinst (inst):
    if len(inst) == 5:
        return tuple(inst) + ("",)
    return inst    


# Fold addition of/from 0, subtraction of 0, xor with 0, and constant-condition conditional moves to MOV
for i_inst, inst in enumerate(rom):
    lineno, opcode, (mode_1, d1), (mode_2, d2), (mode_3, d3), comment = readinst(inst)

    if opcode == "ADD" or opcode == "XOR":
        if mode_1 == 0 and d1 == 0:
            rom[i_inst] = lineno, "MNZ", (0, 32768), (mode_2, d2), (mode_3, d3), comment
        elif mode_2 == 0 and d2 == 0:
            rom[i_inst] = lineno, "MNZ", (0, 32768), (mode_1, d1), (mode_3, d3), comment

    elif opcode == "SUB":
        if mode_2 == 0 and d2 == 0:
            rom[i_inst] = lineno, "MNZ", (0, 32768), (mode_1, d1), (mode_3, d3), comment

    elif opcode == "MNZ":
        if mode_1 == 0 and d1 != 0:
            rom[i_inst] = lineno, "MNZ", (0, 32768), (mode_2, d2), (mode_3, d3), comment

    elif opcode == "MLZ":
        if mode_1 == 0 and ((d1 < 0) or (d1 >= (1 << 15))):
            rom[i_inst] = lineno, "MNZ", (0, 32768), (mode_2, d2), (mode_3, d3), comment


# Constant folding and Mov folding (no dereferences)
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
            if mode_1 - 1 + mode_table <= 3:
                rom[i_inst] = lineno, opcode, (mode_1 - 1 + mode_table, dtable), (mode_2, d2), (mode_3, d3), comment

    if mode_2 > 0:
        if d2 in reg_value.keys():
            # print("Mov folding is possible at line {}, with a value from {}".format(lineno, reg_value[d2]))
            _, (mode_table, dtable) = reg_value[d2]
            if mode_2 - 1 + mode_table <= 3:
                rom[i_inst] = lineno, opcode, (mode_1, d1), (mode_2 - 1 + mode_table, dtable), (mode_3, d3), comment

    if mode_3 > 0:
        if d3 in reg_value.keys():
            # print("Mov folding is possible at line {}, with a value from {}".format(lineno, reg_value[d3]))
            _, (mode_table, dtable) = reg_value[d3]
            if mode_3 - 1 + mode_table <= 3:
                rom[i_inst] = lineno, opcode, (mode_1, d1), (mode_2, d2), (mode_3 - 1 + mode_table, dtable), comment


    if mode_3 == 0 and d3 != 0:
        if opcode == "MNZ" and mode_1 == 0 and not (d1 == 0):
            reg_value[d3] = lineno, (mode_2, d2)
            if mode_2 >= 1:
                add_dst_to_src(d3, d2)
        elif opcode == "MLZ" and mode_1 == 0 and not (type(d2) == int and (0 <= d2 < 32768)):
            reg_value[d3] = lineno, (mode_2, d2)
            if mode_2 >= 1:
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






# Mov folding with dereferences
reg_value = {}
# reg_value_dstedge = {}

for i_inst, inst in enumerate(rom):
    lineno, opcode, (mode_1, d1), (mode_2, d2), (mode_3, d3), comment = readinst(inst)

    if mode_3 == 0 and d3 == 0 and not (mode_1 == 0 and d1 == 0 and opcode == "MNZ"):
        reg_value = {}
        # reg_value_dstedge = {}
        # print("Detected jump")
        # print()

    if mode_1 > 0:
        for mode in reversed(range(4)):
            if (mode, d1) in reg_value.keys():
                # print("Mov folding is possible at line {}, with a value from {}".format(lineno, reg_value[d1]))
                _, (mode_table, dtable) = reg_value[(mode, d1)]
                if mode_1 - 1 + mode_table <= 3:
                    rom[i_inst] = lineno, opcode, (mode_1 - 1 + mode_table, dtable), (mode_2, d2), (mode_3, d3), comment
            break

    if mode_2 > 0:
        for mode in reversed(range(4)):
            if (mode, d2) in reg_value.keys():
                # print("Mov folding is possible at line {}, with a value from {}".format(lineno, reg_value[d2]))
                _, (mode_table, dtable) = reg_value[(mode, d2)]
                if mode_2 - 1 + mode_table <= 3:
                    rom[i_inst] = lineno, opcode, (mode_1, d1), (mode_2 - 1 + mode_table, dtable), (mode_3, d3), comment
            break

    if mode_3 > 0:
        for mode in reversed(range(4)):
            if (mode, d3) in reg_value.keys():
                # print("Mov folding with dereference is possible at line {}, with a value from {}".format(lineno, reg_value[d3]))
                _, (mode_table, dtable) = reg_value[(mode, d3)]
                if mode_3 - 1 + mode_table <= 3:
                    rom[i_inst] = lineno, opcode, (mode_1, d1), (mode_2, d2), (mode_3 - 1 + mode_table, dtable), comment
            break

    # Initialize on every line
    reg_value = {}

    if mode_3 == 0 and d3 != 0:
        if opcode == "MNZ" and mode_1 == 0 and not (d1 == 0):
            reg_value[(mode_3, d3)] = lineno, (mode_2, d2)
            # if mode_2 > 0:
            #     add_dst_to_src((mode_3, d3), d2)
        elif opcode == "MLZ" and mode_1 == 0 and not (type(d2) == int and (0 <= d2 < 32768)):
            reg_value[(mode_3, d3)] = lineno, (mode_2, d2)
            # if mode_2 == 1:
            #     add_dst_to_src(d3, d2)
        elif opcode == "ADD" and mode_1 == 0 and mode_2 == 0:
            reg_value[(mode_3, d3)] = lineno, (mode_2, d1 + d2)
        elif opcode == "SUB" and mode_1 == 0 and mode_2 == 0:
            reg_value[(mode_3, d3)] = lineno, (mode_2, d1 - d2)
        # else:
        #     if d3 in reg_value.keys():
        #         del reg_value[d3]
        # remove_dependent_reference(d3)

    if "pc ==" in comment:
        reg_value = {}
        # reg_value_dstedge = {}


def compare_inst(inst, l):
    lineno, opcode, (mode_1, d1), (mode_2, d2), (mode_3, d3), comment = readinst(inst)
    return (opcode, (mode_1, d1), (mode_2, d2), (mode_3, d3)) == l


unused_lines = []
# Dead code elimination - eq-jeq with 0 to jne
for i_inst, inst in enumerate(rom):
    lineno, opcode, (mode_1, d1), (mode_2, d2), (mode_3, d3), comment = readinst(inst)
    if (
        opcode == "XOR" and i_inst < len(rom) - 1 - 5
        and (mode_3, d3) == (0, 3)
        and compare_inst(rom[i_inst+1], ("MNZ", (1,3), (0,1), (0,3)))
        and compare_inst(rom[i_inst+2], ("XOR", (0,1), (1,3), (0,3)))
        and compare_inst(rom[i_inst+3], ("MNZ", (0,32768), (0,1), (0,9)))
        and compare_inst(rom[i_inst+4], ("MNZ", (1,3), (0,0), (0,9)))
        and tuple(readinst(rom[i_inst+5])[2]) == (1,9) and tuple(readinst(rom[i_inst+5])[4]) == (0,0)
    ):
        rom[i_inst]   = lineno, "XOR", (mode_1, d1), (mode_2, d2), (0,3), "; EQ-JEQ with 0 -> JNE"
        readinst(rom[i_inst+5])[2] = (1,3)
        # rom[i_inst+1] = rom[i_inst+1][0], "MNZ", (1,3), readinst(rom[i_inst+5])[3], (0,0), ";"
        unused_lines.append(i_inst+1)
        unused_lines.append(i_inst+2)
        unused_lines.append(i_inst+3)
        unused_lines.append(i_inst+4)
        # unused_lines.append(i_inst+5)

# Dead code elimination - self-assignment mov elimination
for i_inst, inst in enumerate(rom):
    lineno, opcode, (mode_1, d1), (mode_2, d2), (mode_3, d3), comment = readinst(inst)
    # Line is a no-op
    if opcode == "MNZ" and mode_1 == 0 and d1 != 0 and mode_2 == 1 and mode_3 == 0 and d2 == d3:
        if i_inst > 0:
            prev_inst = readinst(rom[i_inst-1])
            def maybe_jump(inst):
                lineno, opcode, (mode_1, d1), (mode_2, d2), (mode_3, d3), comment = readinst(inst)
                return (mode_3 == 0 and d3 == 0) or (mode_3 > 0)
            if maybe_jump(prev_inst):
                continue
        unused_lines.append(i_inst)

# Dead code elimination - unused mov elimination
reg_fresh = {}


for inst in rom:
    lineno, opcode, (mode_1, d1), (mode_2, d2), (mode_3, d3), comment = readinst(inst)

    # if len(unused_lines) > 18:
        # break

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
            not (opcode == "MNZ" and mode_1 > 0)
            and not (opcode == "MLZ")
            and d3 in reg_fresh.keys()
            and reg_fresh[d3] != False
           ):  # `!= False`, since reg_fresh[d3] may be 0
            # print("Unused line: {} (overwriten at {})".format(reg_fresh[d3], lineno))

            unused_lines.append(reg_fresh[d3])
            # if "pc ==" in comment:
            #     print("Error: Unused line with a pc tag!", lineno)
            #     exit(1)

        reg_fresh[d3] = lineno


pc_rewrite_list = []

# Fold program counter regions that immediately jump to another program counter region,
# and are only reachable by jumping
for i_inst, inst in enumerate(rom):
    if i_inst < 2:
        continue
    inst_2 = readinst(rom[i_inst-2])
    inst_1 = readinst(rom[i_inst-1])
    lineno, opcode, (mode_1, d1), (mode_2, d2), (mode_3, d3), comment = readinst(inst)

    if (
        # The prev-prev inst is mov
        (
            (inst_2[1] == "MNZ" and inst_2[2][0] == 0 and inst_2[2][1] != 0)
            or
            (inst_2[1] == "MLZ" and inst_2[2][0] == 0 and (inst_2[2][1] < 0 or inst_2[2][1] >= (1 << 15)))
        )
        # The prev-prev inst is a mov to the program counter
        and (inst_2[4][0] == 0 and inst_2[4][1] == 0)
        # The previous inst is the beginning of a program counter
        and "pc == " in inst_1[-1]
        # # The previous inst is a no-op
        # and inst_1[1:-1] == ("MNZ", (0,0), (0,0), (0,0))
        # The current inst is mov
        and (
            (inst[1] == "MNZ" and inst[2][0] == 0 and inst[2][1] != 0)
            or
            (inst[1] == "MLZ" and inst[2][0] == 0 and (inst[2][1] < 0 or inst[2][1] >= (1 << 15)))
        )
        # The current inst is a mov to the program counter
        and (inst[4][0] == 0 and inst[4][1] == 0)
        ):
        current_pc = int(re.findall(r'pc == ([0-9]+):', inst_1[-1])[0])
        current_jmp_dest = d2
        pc_rewrite_list.append((current_pc, current_jmp_dest))

        unused_lines.append(i_inst)
        unused_lines.append(i_inst-1)


modedict = {
    0 : "",
    1 : "A",
    2 : "B",
    3 : "C",
}

i_effective_line = 0

for pc, dest in pc_rewrite_list:
    header = header + [";;;; pc{} : {}\n".format(pc, dest)]

for inst in header:
    print(inst, end="")

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
