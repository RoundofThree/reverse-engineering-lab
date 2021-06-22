all: ex3 

ex3: ex3.o
	gcc -Wall -no-pie -m64 ex3.o test_ex3.c -o test_ex3

ex3.o:
	nasm -f elf64 ex3.asm 

clean:
	rm -rf *.o test_ex3