CC = g++

all: clean parser

parser:
	$(CC) main.cpp parser.cpp scanner.cpp -o parser 

debug:
	$(CC) main.cpp parser.cpp scanner.cpp -g -o parser 

clean:
	rm parser
