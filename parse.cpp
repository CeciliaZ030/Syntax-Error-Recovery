#include <iostream>
#include <ios>
#include "scan.h"
#include <cstdlib>
#include <string>


using std::string;
using std::cin;
using std::cout;
using std::endl;


const char* names[] = {"read", "write", "id", "literal", "gets", "if",
"do", "equal", "notequal", "smaller",
"greater", "smallerequal","greaterequal",
"add", "sub", "mul", "div", "lparen", "rparen", "eof"};

static token input_token;
static int spaceCount = 0;
static string image = "";
static int hasError = 0;



void error () {
    cout << "syntax error" << endl;
    exit (1);
}


string match (token expected) {
    if (input_token == expected) {
        image = getImage();
        input_token = scan();
        
    } else {
      image = getImage();
      throw string("match");
  }
  return "";
}

string spaceBefore(string str, int space){
  for(int i = 0; i <= space; i++){
    str = " " + str;
  }
  return str;
}

string spaceAfter(string str, int space){
  for(int i = 0; i <= space; i++){
    str += " ";
  }
  return str;
}

string program();
string stmt_list();
string stmt();
string c();
string expr();
string expr_tail();
string term_tail();
string term();
string factor_tail();
string factor();
string relation_op();
string add_op();
string mul_op();
string relation();

string program() {
    try{
        switch (input_token) {
            case t_id:
            case t_read:
            case t_write:
            case t_if:
            case t_eof:{
                spaceCount++;
                string line1 = "(program \n" ;
                line1 = spaceAfter(line1, spaceCount);
                line1 += "[";
                line1 += stmt_list ();
                match (t_eof);
                line1 = spaceAfter(line1, spaceCount);
                line1 += "]\n";
                if(hasError) return "";
                return line1+")\n";
            }
            default:
            throw string("program");
            return "";
        }
    } catch (string e){
        cout << " Not expecting token in Program"<< endl;
        return "";
    }
}

string stmt_list() {
  switch (input_token) {
      case t_id:
      case t_write:
      case t_read:
      case t_if:
      case t_do:{
        string line1 = "";
        line1 = spaceAfter(line1, spaceCount);
        line1 += "("+stmt();
        line1 += stmt_list();
        line1 = spaceAfter(line1, spaceCount);
        line1 += ")\n";
        spaceCount--;
        return line1;
      }
      case t_eof:
        spaceCount--;
        return "\n";
    default:
      spaceCount--;
      return "\n";
    }
}

// S    →     id := E  |  read id  |  write E  |  if C SL end  |  while C SL end
string stmt() {
  spaceCount++;
  try{
    switch (input_token) {
        case t_id:{
            match (t_id);
            match (t_gets);
            string line1 = "( := (id "+ image+")" + expr();
            line1 = spaceAfter(line1, spaceCount);
            line1 += ")";
            spaceCount--;
          return line1;
        }
            
        case t_read:{
            match (t_read);
            match (t_id);
            spaceCount--;
            return "read (id"+image+" )\n";
        }
           
       case t_write:{
            match (t_write);
            string line1 = expr();
            line1 = spaceAfter(line1, spaceCount);
            spaceCount--;
            return "(write " + line1 + ")\n";
        }
        
        
        case t_if:{
          //if C SL end
          match(t_if);
          string line1 = "(if \n";
          line1 = spaceAfter(line1, spaceCount);
          line1 += c();
          line1 = spaceAfter(line1, spaceCount);
          string line2  = stmt_list();
          line2 = spaceAfter(line2, spaceCount);
          spaceCount--;
          return line1 +"[\n"+ line2 + "])\n";
        }
            
        case t_while:{
          // while C SL end
          match(t_if);
          string line1 = "(if \n";
          line1 = spaceAfter(line1, spaceCount);
          line1 += c();
          line1 = spaceAfter(line1, spaceCount);
          string line2  = stmt_list();
          line2 = spaceAfter(line2, spaceCount);
          spaceCount--;
          return line1 +"[\n"+ line2 + "])\n";
        }

        default: 
        error();
        spaceCount--;
        return "";
      }
    }catch(string e){
        return " ";
    }

}

string c() {
  return "";

}


string expr() {
  return "";

}


string expr_tail(){
    return " ";
}
 
string term_tail() {
    return " ";
}


string term() {
    return " ";
}


string factor_tail() {

    return " ";
}

string factor() {
    return " ";
}


string relation_op(){
    return " ";
}

string add_op() {
    return " ";
}
 
string mul_op() {
    return " ";
}

string relation(){
      return " ";
}

int main() {
    input_token = scan ();
    cout << program ();
    return 0;
}
