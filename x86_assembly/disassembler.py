# Disassemble a binary at its entry point, just like ndisasm. 
# Use capstone instead of BeaEngine. 
from capstone import *
import sys

# CODE = b"\x55\x48\x8b\x05\xb8\x13\x00\x00"

def disassemble(file):
    with open(file, 'rb') as f:
        CODE = f.read() 
    mode = Cs(CS_ARCH_X86, CS_MODE_64) 
    for (address, size, mnemonic, op_str) in mode.disasm_lite(CODE, 0x1000):
	    print("0x%x: \t%s\t%s" %(address, mnemonic, op_str))

if __name__ == '__main__': 
    if len(sys.argv) < 2: 
        print("Usage: disassembler.py <binary>")
        print("For example, \n disassembler.py test.bin")
        exit(0)
    print(f"Disassembly output for {sys.argv[1]}")
    disassemble(sys.argv[1])