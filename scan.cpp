#include<iostream>
#include<ios>
#include <cstdlib> //Only for exit(1)
#include <string>
#include <cctype>
#include "scan.h"

using std::string;
using std::cin;
using std::cout;
using std::endl;
using std::noskipws;


char token_image[128];
string image;

std::string getImage(){
  return " \""+image+"\"";
}

void indent(int level){
    for(int i = 0; i < level; i++)
    cout << " ";
}

token scan() {
    static char c = ' ';
        /* next available char; extra (int) width accommodates EOF */
         /* index into token_image */
    int i = 0; 
	do{
    	if(!cin.get(c)){
            return t_eof;
        }
	} while(isspace(c));

    bool isAlldigit = true;

    if (isalpha(c) || isdigit(c)) {
        do {
            token_image[i++] = c;
            if (!isdigit(c)) {
                isAlldigit = false;
            }
            if(!cin.get(c)){
                break;
            }
        } while (!isspace(c) && (isalpha(c) || isdigit(c) || c == '_'));

        token_image[i] = '\0';

        if(token_image == string("read")){
            return t_read;
        }
        else if (token_image == string("write")){
            return t_write;
        }
        else if (token_image == string("if")){
            return t_if;
        }
        else if (token_image == string("while")){
            return t_while;
        }
        else if (token_image == string("end")){
            cout << "scanner t_end" << endl;
            return t_end;
        }
        else{
            image = token_image;
            if (isAlldigit){
                cout << "scanner t_literal" << endl;
                return t_literal;
            }
            else{
                cout << "scanner t_id" << endl;
                return t_id;
            }
        }   
    }
    else {
        //cin.get(c); ?
        switch (c) {
            case '+': 
                cout << "scan t_rparen" << endl;
                return t_add;
            case '-': 
                return t_sub;
            case '*': 
                return t_mul;
            case '/': 
                return t_div;
            case '(': 
                return t_lparen;
            case ')': 
                cout << "scan t_rparen" << endl;
                return t_rparen;
            case ':':
                cin.get(c);
                if (c == '='){
                    return t_gets;
                } 
                cout << "scanning error 1\n";

            //Relation operators
                
            case '<':
                cin.get(c);
                cout << "here " << c << endl;
                if (c == '>'){
                    cout << "scanner t_notequal" << endl;
                    return t_notequal;
                }else if(c == '='){
                    cout << "scanner t_smallerequal" << endl;
                    return t_smallerequal;
                }else{
                    cout << "scanner t_smaller" << endl;
                    return t_smaller;
                }
            case '>':
                cin.get(c);
                if(c == '='){
                    cout << "scanner t_greaterequal" << endl;
                    return t_greaterequal;
                }else{
                    cout << "scanner t_greater" << endl;
                    return t_greater;
                }
            case '=':
                cin.get(c);
                if(c == '='){
                    cout << "scanner t_equal" << endl;
                    return t_equal;
                }//remove double equal

            // end of file

            case '$':
                cin.get(c);
                if(c == '$'){
                    cout << "scanner t_eof" << endl;
                    return t_eof;
                }
            default:
                cout << "scanning error 2\n";
                exit(1);
        }
    }
}
