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
static int spaceVal = 0;
static int hasError = 0;
static string image = "";

void error () {
    cout << "Syntax Error" << endl;
    exit (1);
}

string match (token expected) {
    if (input_token == expected) {
        image = getImage();
        input_token = scan ();
    } else {
        return "match";
  }
  return "";
}

string frontSpace(string str, int tab){
  for(int i = 0; i <= tab; i++){
    str = " " + str;
  }
  return str;
}

string backSpace(string str, int tab){
  for(int i = 0; i <= tab; i++){
    str += " ";
  }
  return str;
}

string prefix(string str, string tail){
  if(tail == "")
      return str;
  for(int i = 0; i < tail.length(); ++i){
    if(tail[i] == ' '){
      return tail.substr(0,i) + " " + str + " "
        + tail.substr(i+1, tail.length() - i);
    }
  }
  return "prefix error";
}

int contains(token t, token set[]){
  int i = 0;
  while(set[i]){
    if (t == set[i++]) {
      return 1;
    }
  }
  return 0;
}



string program ();
string stmt_list ();
string stmt ();
string condition();
string expr ();
string expr_tail();
string term_tail ();
string term ();
string factor_tail ();
string factor ();
string relation_op();
string add_op ();
string mul_op ();


string program() {
    spaceVal++;
    try{
        switch (input_token) {
            case t_id:
            case t_read:
            case t_write:
            case t_eof:{
                
                string line1 = "(program \n" ;
                line1 = backSpace(line1, spaceVal);
                line1 += "[";
                line1 += stmt_list ();
                match(t_eof);
                line1 = backSpace(line1, spaceVal);
                line1 += "]\n";
                
                return line1 + ")\n";
            }
            default:
            return "";
        }
    } catch (string e){
      return "";
    }
}

string stmt_list() {
  switch (input_token) {
      case t_id:
      case t_write:{
          
          string line1 = "";
          line1 = backSpace(line1, spaceVal);
          line1 += "(" + stmt();
          line1 += stmt_list();
          line1 = backSpace(line1, spaceVal);
          line1 += ")\n";
          spaceVal--;
          return line1;
      }
      case t_read:
      case t_if:
      case t_do:{
        string line1 = "";
        line1 = backSpace(line1, spaceVal);
        line1 += "(" + stmt();
        line1 += stmt_list();
        line1 = backSpace(line1, spaceVal);
        line1 += ")\n";
        spaceVal--;
        return line1;
      }
      case t_eof:{
          
          return "\n";
      }
        
    default:
      spaceVal--;
      return "\n";
    }
}

string stmt() {
  spaceVal++;
  try{
    switch (input_token) {
        case t_id:{
            match(t_id);
            match(t_gets);
            string line1 = "( := (id " + image + ")" + condition();
            line1 = backSpace(line1, spaceVal);
            line1 += ")";
            spaceVal--;
            return line1;
        }
        case t_read:
            match(t_read);
            match(t_id);
            spaceVal--;
            return "read (id" + image +" )\n";
            
        case t_write:{
            match(t_write);
            string line1 = condition();
            line1 = backSpace(line1, spaceVal);
            spaceVal--;
            return "(write " + line1 + ")\n";
        }
        case t_if:{
              match(t_if);
              string line1 = "(if \n";
              line1 = backSpace(line1, spaceVal);
              line1 += condition();
              line1 = backSpace(line1, spaceVal);
              string line2 = stmt_list();
              line2 = backSpace(line2, spaceVal);
              spaceVal--;
            
              return line1 +"[\n"+ line2 + "])\n";
        }
        case t_while:{
              match(t_while);
              string line1 = "(while \n";
              line1 = backSpace(line1, spaceVal);
              line1 += condition();
              line1 = backSpace(line1, spaceVal);
              string line2 = stmt_list();
              line2 = backSpace(line2, spaceVal);
              spaceVal--;
            
              return line1 +"[\n"+ line2 + "])\n";
        }


        default:
            error();
            spaceVal--;
            return "";
      }
    } catch(string e) {
        return "";
    }
}

string condition() {
    try{
      spaceVal++;
      string line2 = expr();
      string line1 = expr();
      spaceVal--;
      return "(" + prefix(line2, line1) + ")\n";
    }catch(string e){
        
        return "";
    }
}

string expr() {
  spaceVal++;
  try{
        string line1 = term();
        string line2 = term_tail();
        spaceVal--;
        return prefix(line1, line2);
    } catch(string e){
        return "";
    }
    error();
    spaceVal--;
  return "";

}


string expr_tail() {
  spaceVal++;
  switch (input_token) {
    case t_equal:
    case t_notequal:
    case t_smaller:
    case t_greater:
    case t_smallerequal:
    case t_greaterequal:{
        string line1 = relation_op();
        string line2 = expr();
        spaceVal--;
        return line1 + " " + line2;
    }
    case t_id:
    case t_read:
    case t_write:
    case t_eof:
        spaceVal--;
        return "";
    default:
        spaceVal--;
        return "";
  }
}

string term_tail() {
  spaceVal++;
  switch (input_token) {
    case t_add:
    case t_sub:{
        string line1 = add_op ();
        line1 += " ";
        line1 += term();
        string line2 = term_tail();
        spaceVal--;
        return prefix(line1, line2);
    }
    case t_rparen:
    case t_id:
    case t_read:
    case t_write:
    case t_eof:
      spaceVal--;
      return "";
    default:
      return "";
  }
}

string term() {
  try{
    spaceVal++;
    string line1 = factor();
    string line2 = factor_tail();
    spaceVal--;
    return prefix(line1, line2);
    } catch (string e){
      spaceVal--;
    }
    return "";

}

string factor_tail() {
  spaceVal++;
  switch (input_token) {
    case t_mul:
    case t_div:{
        string line1 = mul_op();
        string line2 = factor();
        line1 += line2;
        line1 += factor_tail();
        spaceVal--;
        return line1 + "";
    }
    case t_add:
    case t_sub:
    case t_rparen:
    case t_id:
    case t_read:
    case t_write:
    case t_eof:
      spaceVal--;
      return "";
    default:
      return "";
    }
}

string factor() {
  spaceVal++;
  switch (input_token) {
    case t_id :{
      match (t_id);
      spaceVal--;
      string line1 = "(id" + image + ")";
      return line1;
    }
    case t_literal:{
      match (t_literal);
      spaceVal--;
      string line1 = "(lit" + image + ")";
      return line1;
    }
    case t_lparen:{
        match (t_lparen);
        string line1 = condition();
        match (t_rparen);
        spaceVal--;
        return "(" + line1 + ")";
    }
    default:
        spaceVal--;
        return "";
  }
}


string relation_op(){
  spaceVal++;
  switch(input_token){
    case t_equal:
        match(t_equal);
        spaceVal--;
        return "= ";
    case t_notequal:
        match(t_notequal);
        spaceVal--;
        return "<> ";
    case t_smaller:
        match(t_smaller);
        spaceVal--;
        return "< ";
    case t_greater:
        match(t_greater);
        spaceVal--;
        return "> ";
    case t_smallerequal:
        match(t_smallerequal);
        spaceVal--;
        return "<= ";
    case t_greaterequal:
        match(t_greaterequal);
        spaceVal--;
        return ">= ";
    default:
        spaceVal--;
        return "";
  }
}

string add_op() {
  spaceVal++;
  switch (input_token) {
    case t_add:
        match(t_add);
        spaceVal--;
        return "+ ";
    case t_sub:
        match(t_sub);
        spaceVal--;
        return "- ";
    default:
        spaceVal--;
        return "";
  }
}

string mul_op() {
  spaceVal++;
  switch (input_token) {
    case t_mul:
        match(t_mul);
        spaceVal--;
        return "* ";
    case t_div:
        match(t_div);
        spaceVal--;
        return "/ ";
    default:
        spaceVal--;
        return "";
  }
}

int main() {
    input_token = scan();
    cout << program();
    return 0;
}
