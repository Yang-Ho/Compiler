Use makefile provided.

Run: 
    make main

Will produce program called compiler.exe

If makefile is corrupted or something run the following:

g++ main.cpp parser.cpp scanner.cpp grammar.cpp token.cpp symbol.cpp ASTnode.cpp label.cpp -o compiler.exe

To run:
    ./compiler.exe <input>

will produce output code <input>_gen.c

Output:
    if there are no errors:
        Done generating code
        Output file: "output file name"
    otherwise:
        Approripate error message for scanner or parsing.

I used my modified scanner from project 2.

I modified my own parser to use the same structure, organization, and design of the sample parser but the implementation of the parser is still largely my own. For the code generation I modified the parser to generate an AST.

For this project my AST had two kinds of nodes:
1) Expression node - anything that requires an address (local[whatever]) and does not end with a ; or {}
2) Statement node - anything thing that ends with ; or {}

There are several types of expression nodes and statement nodes (see ASTnode.h for details).

I used abstract classes ExprNode and StmtNode and created each node as a separate class. I then modified the parser to create nodes and combined certain productions to reduce the number of different types of AST nodes. Each node had a virtual function called genCode.

For ExprNodes, genCode returned a string that represents the resulting address. For StmtNodes, genCode did not return anything. Both had one parameter: a SymbolTable. 

Each node's genCode function would generate the approriate code. 

To keep track of labels, symbols, and addresses I created three new classes: Label, Symbol, and SymbolTable.

Label class keeps track of the next label to be created. 
Symbol represented any variable that needed to be stored in a more permanent address (like variables, parameters, and func id's).
SymbolTable kept track of what addresses belonged to which symbols using maps (hash functions) and a vector (the stack of hash tables). SymbolTable also kept track of the various scopes (function scopes and while scopes) and was tasked with assigning new temporary addresses (local[some number]). 
