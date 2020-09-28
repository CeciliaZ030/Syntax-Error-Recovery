CSC 254 Assignment 2 
Syntax Error Recovery

Yixin Zhang
netID: yzh223

Ziyi Wang
netID: zwang104





This submission includes three C++ files including scan.h, scan.cpp, and parse.cpp. 
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





Please note that there should be space between each input symbol; for instance, "a+b" should be write as a + b for input in the program. The input file we included contains the example we used to test the program. 

Please use the following command to run our program.

$ g++ -std=c++11 parse.cpp scan.cpp -o parse

$ ./parse < ./input.txt
 
