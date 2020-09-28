CSC 254 Assignment 2 
Syntax Error Recovery

Yixin Zhang
netID: yzh223

Ziyi Wang
netID: zwang104

This submission includes three C++ files including scan.h, scan.cpp, and parse.cpp. 

Please use the following command to run our program.
$ g++ -std=c++11 parse.cpp scan.cpp -o parse
$ ./parse < ./input.txt

Important: Please note that there should be space between each symbol in the input file; for instance, "a+b" as a part of the input should be write as "a + b" in the input file for the program. 

Our program, scan.cpp relied on spaces to identify tokens, and variables that were not separated by space cannot be processed by the programmer and would not lead to expected outputs.

We tested our program on cycle1 csug machine and the input file contained the example we used while testing. We also used the sample given on the assignment page, which is the case that calculates the first n primes, and the program was able to output the abstract syntax tree as expected. In order to demonstrate the structure of the abstract syntax tree, we implemented the indentation which mirrors the internal structure of the abstract syntax tree. 

Our program reads a calculator program from standard input and the program is expected to output a correct syntax tree or syntax error messages. While translating the program from the given source code written in C to C++, we replaced the calls to c libraries with equivalents statement in C++. More specifically, we replaced "printf" statement with standard C++ output. 

We extended the language with if and while statements and implemented the program with the given grammar.

P	→ 	SL $$
SL	→ 	S SL  |  ε
S	→ 	id := E  |  read id  |  write E  |  if C SL end  |  while C SL end
C	→ 	E ro E
E	→ 	T TT
T	→ 	F FT
F	→ 	( E )  |  id  |  lit
TT	→ 	ao T TT  |  ε
FT	→ 	mo F FT  |  ε
ro	→ 	=  |  <>  |  <  |  >  |  <=  |  >=
ao	→ 	+  |  -
mo	→ 	*  |  /


The syntax error recovery we implemented in the program functions by detecting syntax flaws in the first and follow set of the non-terminals. We completed first and follow set scrutinization for grammars of "statement", "statement list", "condition", and "expression". Meanwhile, if the program detects an error in "term tail" and "factor tail" part of the grammar, the program would return the error message and throw the error to "expression" grammar. 


