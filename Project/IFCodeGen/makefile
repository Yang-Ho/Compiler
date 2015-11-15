CC = g++

all: clean main 

main:
	$(CC) main.cpp parser.cpp scanner.cpp grammar.cpp token.cpp symbol.cpp ASTnode.cpp address.cpp label.cpp -o compiler.exe

clean:
	rm compiler.exe
