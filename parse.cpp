#include <iostream>
#include <ios>
#include <cstdlib>
#include <string>
#include "scan.h"

using std::string;
using std::cin;
using std::cout;
using std::endl;

const char* names[] = {"read", "write", "if", "while", "end", "id", "literal", "gets", "add", "sub", "mul", "div", "lparen", "rparen", "t_equal", "t_notequal", "t_smaller", "t_greater", "t_smallerequal", "t_greaterequal", "eof"};

static token input_token;
static int spaceNum = 0;
static string image = "";

void error() {
    //TODO
}

string match(token expected) {
   
    return "";
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


string program(){
    
    try{
       
        switch (input_token){
            case t_id:
            case t_read:
            case t_write:
            case t_eof:{
                string line1 = "(program \n";
                // postIndent
                line1 += "[";
                line1 += stmt_list();
                match(t_eof);
                // postIndent
                line1 += "]\n";
                
                match(t_eof);
                return line1 + ")\n";
            }
            default:
            return "program";
                
        }
    } catch(string exception){
        cout << "unexpected input in the program" << endl;
        return "";
    }
}



string stmt_list(){
    
    cout << "predict stmt_list --> stmt stmt_list\n" << endl;
    try{
        switch (input_token){
                case t_id:
                case t_read:
                case t_write:
                case t_if:
                case t_eof:{
                
                    //
                    string line1 = "";
                    // line1 += postIndent
                    line1 += "(" + stmt ();
                    line1 += stmt_list();
                    // line 1 += PostIndent
                    line1 += ")\n";
                    spaceNum--;
                    
                    return line1;
                    break;
                    }
                    spaceNum--;
                    return "\n";
                default:
                    spaceNum--;
                    return "\n";
                
                
        }
    } catch(string exception){
        cout << "unexpected input in the program" << endl;
        return "";
    }


}


// S    →     id := E  |  read id  |  write E  |  if C SL end  |  while C SL end
string stmt(){
    
    spaceNum++;
    try{
       
        cout << "predict stmt --> id gets expr\n" << endl;
        switch (input_token){
            case t_id:{

                // cout << "predict stmt --> id gets expr\n" << endl;
                match(t_id);
                match(t_gets);
                string line1 = "( :=(id " + image + ")" + expr ();
                // postIndent
                line1 += ")";
                spaceNum--;
                    
                return line1;
                break;
            }
            case t_read:{
                
                // cout << "predict stmt --> read id\n" << endl;
                match (t_read);
                match (t_id);
                spaceNum--;
                
                return "read (id " + image + ")\n";
                break;
            }
                
            case t_write:{
                
                // cout << "predict stmt --> write expr\n" << endl;
                match (t_write);
                string line1 = expr ();
                // postIndent
                spaceNum--;
                return "write " + line1 + ")\n";
                break;
            }
            
            case t_if:{
                return "";
            }
                
            case t_while:{
                return "";
            }
                
            default:
                error();
                spaceNum--;
                return "";
        }
    } catch(string exception){
        cout << "unexpected input in the program" << endl;
        return "";
    }
}

string expr(){
    return "";
}


int main(){
    cout << "main \n" << endl;
    input_token = scan();
    cout << program();
    return 0;
}
