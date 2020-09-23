#include <iostream>
#include <ios>
#include <cstdlib>
#include <string>
#include "scan.h"

//using this instead of namespace std to avoid allocating for those unused
using std::string;
using std::cin;
using std::cout;
using std::endl;

const char* names[] = {"read", "write", "if", "while", "end", "id", "literal", "gets",
                       "add", "sub", "mul", "div", "lparen", "rparen", 
                       "t_equal", "t_notequal", "t_smaller", "t_greater", "t_smallerequal", "t_greaterequal", "eof"};

static token input_token;

void error() {
    //TODO
}

string match(token expected) {
    //TODO
}

string program();
string stmt_list();
string stmt();
string condition();
string expr();
string term_tail();
string term();
string factor_tail();
string factor();
string relation_op();
string add_op();
string mul_op();


int main(){
    cout << "main \n" << endl;
    input_token = scan();
    cout << program()
    return 0;
}