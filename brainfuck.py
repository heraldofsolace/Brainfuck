#A simple (probably) Brainfuck interpreter.
#If does not work, blame Jason

import sys

memcells = [0]*30000
memptr = 0

def char_lt():
    """
    performs a <
    """
    global memptr
    if memptr <= 0:
        raise (ValueError, "Trying to point before starting of memory")
    memptr -= 1

def char_gt():
    """
    performs a >
    """
    global memptr
    if memptr >= 3*10**4 - 1:
        raise (ValueError, "Trying to point outside memory")
    memptr += 1

def char_plus():
    """
    performs a +
    """
    global memptr
    memcells[memptr] = (memcells[memptr] + 1) % 256

def char_minus():
    """
    perfoms a -
    """
    global memptr
    memcells[memptr] = (memcells[memptr] - 1) % 256

def char_dot():
    """
    performs a .
    """
    global memptr
    sys.stdout.write(chr(memcells[memptr]))

def char_comma():
    """
    performs a ,
    """
    global memptr
    c = ord(sys.stdin.read(1))
    if c != 26:
        memcells[memptr] = c

def loopmap(code):
    """
    maps the "["s to the corresponding "]"s
    """
    opening_brackets_stack = []
    map_loops = {}
    for index,char in enumerate(code):
        if char == "[":
            opening_brackets_stack.append(index)
        elif char == "]":
            try:
                begin = opening_brackets_stack.pop()
                map_loops[begin] = index
            except IndexError:
                raise (ValueError, "Number of ] is more than [")
    if opening_brackets_stack != []:
        raise (ValueError, "Number of [ is more than ]")
    else:
        return map_loops

def evaluate(code):
    """
    evaluates brainfuck code
    """
    global memptr
    map_loops = loopmap(code)
    pc = 0
    stack = []
    while pc < len(code):
        instruction = code[pc]
        if instruction == ">":
            char_gt()
        elif instruction == "<":
            char_lt()
        elif instruction == "+":
            char_plus()
        elif instruction == "-":
            char_minus()
        elif instruction == ".":
            char_dot()
        elif instruction == ",":
            char_comma()
        elif instruction == "[":
            if memcells[memptr] > 0:
                stack.append(pc)
            else:
                pc = map_loops[pc]
        elif instruction == "]":
            pc = stack.pop() - 1
        pc += 1

def filter_code(code):
    """
    Filters the code and ignores anything that's not an instruction
    """
    return list(filter(lambda x: x in ['.', ',', '[', ']', '<', '>', '+', '-'], code))

file = open(sys.argv[1],'r')
cleaned_code = filter_code(file.read())
evaluate(cleaned_code)
file.close()
