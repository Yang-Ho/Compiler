Contains the following files:
scanner.h and main.cpp

To compile:
g++ main.cpp -o scanner

To run: (assuming you use -o scanner)
./scanner filename.c

 - It will generate a filename_gen.c file in the same directory as filename.c.

Error:
- If there is an error with a token, then it will display the following message:
Error in filename.c: badToekn is an invalid token!
- Other error messages:
Error: no file provided!

- If there is an error, it does not produce a _gen.c file. (It creates one but it is removed).

Scanner Implementation:
1. I defined a series of constant strings that contain all valid characters and categorized them based on type. (ex. Digits, letters (as defined by token language), etc.)
 - I also created an array of reserved words.
2. I then implemented a DFA using explicit state transitions (switch statement). 
3. In terms of the DFA, I did not differntiate between ID and Reserved words. This is because all reserved words would be accepted by the id regex. To differntiate, I simply search to see if the token matches a reserved word.
4. To determine if there are any tokens left, I just check for eof() and I ignore whitespace in cases where it is appropriate