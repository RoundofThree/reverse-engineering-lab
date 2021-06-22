; nasm -f elf64 test.asm
section .text
global foo
foo:
	mov rax, 7
	ret