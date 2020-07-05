#compiler creates MockShell from main.c and builtins.c
all: builtins.c main.c
	gcc main.c builtins.c -o MockShell

clean:
	$(RM) MockShell
