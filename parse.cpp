#include <vector>
#include <iostream>
#include <ios>
#include <cstdlib>
#include <string>

#include "scan.h"

using std::string;
using std::cin;
using std::cout;
using std::endl;

const char* names[] = {"read", "write", "id", "literal", "gets", "if",
"do", "equal", "notequal", "smaller",
"greater", "smallerequal","greaterequal",
"add", "sub", "mul", "div", "lparen", "rparen", "eof"};


static token input_token;
static string image = "";

// static int spaceVal = 0;
// static int hasError = 0;


void error () {
    cout << "Syntax Error" << endl;
    exit (1);
}

void match (token expected) {
    cout << "   in match " << expected << " " << input_token << endl;
    if (input_token == expected){
        cout << "   input_token == expected" << endl;
        if (input_token == t_id || input_token == t_literal){
            image = getImage();
            cout << "   match" << image << endl;
        }
        input_token = scan();
        cout << "   scan next and get " << input_token << endl;
    } 
    else{
        cout << "match";
        error();
    }
}

// string frontSpace(string str, int tab){
//   for(int i = 0; i <= tab; i++){
//     str = " " + str;
//   }
//   return str;
// }

// string backSpace(string str, int tab){
//   for(int i = 0; i <= tab; i++){
//     str += " ";
//   }
//   return str;
// }

// string prefix(string str, string tail){
//   if(tail == "")
//       return str;
//   for(int i = 0; i < tail.length(); ++i){
//     if(tail[i] == ' '){
//       return tail.substr(0,i) + " " + str + " "
//         + tail.substr(i+1, tail.length() - i);
//     }
//   }
//   return "prefix error";
// }

// int contains(token t, token set[]){
//   int i = 0;
//   while(set[i]){
//     if (t == set[i++]) {
//       return 1;
//     }
//   }
//   return 0;
// }



AST_node program ();
std::vector<AST_node> stmt_list ();
AST_node stmt ();
AST_node condition();
AST_node expr ();
AST_node expr_tail();
AST_node term_tail (AST_node T);
AST_node term ();
AST_node factor_tail (AST_node F);
AST_node factor ();
AST_node relation_op();
AST_node add_op ();
AST_node mul_op ();


AST_node program()
{
    AST_node P = AST_node("program");
    switch (input_token) {
        case t_id:
        case t_read:
        case t_write:
        case t_eof:{
            printf ("predict program --> stmt_list eof\n");
            std::vector<AST_node> SL = stmt_list();
            match(t_eof);
            P.children = SL;
            P.printMode = 3;
            cout << "YEAH!!!" << endl;
            return P;
        }
        default:
            cout << "program" << endl;
            error();
    }
}

std::vector<AST_node> stmt_list()
{
    std::vector<AST_node> SL;
    switch (input_token) {
        case t_id:
        case t_read:
        case t_if:
        case t_while:
        case t_write: {
            printf ("predict stmt_list --> stmt stmt_list\n");

            SL.push_back(stmt());
            std::vector<AST_node> tail = stmt_list();
            SL.insert(SL.end(), //TODO: possibly empty tail here
                      tail.begin(), tail.end());

            return SL;
        }
        case t_eof: {
            printf("predict stmt_list --> epsilon\n");
            return {}; //return empty vec
        }
        case t_end: {
            printf ("predict stmt_list --> epsilon (end of if/while stmt_list)\n");
            return {};
        }
        default:
            cout << "stmt_list" << endl;
            error();
    }
}

AST_node stmt() 
{
    AST_node S = AST_node("");
    switch (input_token) {
        case t_id:{
            printf ("predict stmt --> id_gets expr\n");
            match(t_id);
            match(t_gets);
            S.value = ":=";
            S.children = {AST_node("id:" + image), expr()};
            S.printMode = 1;
            return S;
        }
        case t_read:{
            printf ("predict stmt --> read id\n");
            match(t_read);
            cout << "match(t_id);" << endl;
            match(t_id);
            S.value = "read";
            S.children = {AST_node("id:" + image)};
            S.printMode = 1;
            return S;
        }
        case t_write:{
            printf ("predict stmt --> write expr\n");
            match(t_write);
            S.value = "write";
            S.children = {expr()};
            S.printMode = 1;
            cout << "here";
            return S;
        }
        case t_if:{
            printf ("predict stmt --> if condition stmt_list\n");
            match(t_if);
            S.value = "if";
            S.children = {condition()};
            std::vector<AST_node> ifSubS = stmt_list();
            S.printMode = 2;
            cout << "t_end in t_if" << endl;
            match(t_end);
            S.children.insert(S.children.end(), 
                                 ifSubS.begin(), ifSubS.end());
            return S;
        }
        case t_while:{
            printf ("predict stmt --> while condition stmt_list\n");
            match(t_while);
            S.value = "while";
            S.children = {condition()};
            std::vector<AST_node> whileSubS = stmt_list();
            match(t_end);
            S.printMode = 2;
            cout << "t_end in t_while" << endl;
            S.children.insert(S.children.end(), 
                                 whileSubS.begin(), whileSubS.end());
            return S;
        }
        default:
            cout << "stmt" << endl;
            error();
    }

}

AST_node condition() 
{
    AST_node C = AST_node("");
    AST_node lexpr = expr();
    printf ("predict condition --> expr r_op expr\n");
    // relation op
    switch(input_token){
        case t_greater:
            match(t_greater);
            C.value = ">";
            break;
        case t_smaller:
            match(t_smaller);
            C.value = "<";
            break;
        case t_greaterequal:
            match(t_greaterequal);
            C.value = ">=";
            break;
        case t_smallerequal:
            match(t_smallerequal);
            C.value = "<=";
            break;
        case t_equal:
            match(t_equal);
            C.value = "==";
            break;
        case t_notequal:
            match(t_notequal);
            C.value = "!=";
            break;
        default:
            cout<< "condition";
            error();
    }

    AST_node rexpr = expr();
    C.children = {lexpr, rexpr};
    C.printMode = 0;
    return C;
}

AST_node expr() 
{
    AST_node E = AST_node("");
    bool isParan = false;
    switch (input_token) {
        case t_lparen:
            isParan = true;
        case t_id:
        case t_literal:
            printf ("predict expr --> term term_tail\n");

            if (isParan)
                match(t_lparen);

            E = term();
            E.printMode = 0;

            if (isParan)
                match(t_rparen);

            cout << "return from E" << endl;
            return term_tail(E);
        default: 
            cout << "expr";
            error();
    }
}

AST_node term() {
    AST_node F = AST_node("");
    switch (input_token) {
        case t_id:
        case t_literal:
        case t_lparen:
            printf ("predict term --> factor factor_tail\n");
            F = factor();
            F.printMode = 0;
            return factor_tail(F);
        default: 
            cout << "term" << endl;
            error();
    }
}

AST_node term_tail(AST_node T) 
{
    AST_node TT = AST_node("");
    switch (input_token) {
        case t_add:
        case t_sub:
            printf ("predict term_tail --> add_op term term_tail\n");
            TT = add_op();
            TT.printMode = 0;
            TT.children = {T, term()};
            return term_tail(TT);
        case t_rparen:
        case t_id:
        case t_read:
        case t_write:
        case t_while:
        case t_if:
        case t_end:
        case t_equal: 
        case t_notequal: 
        case t_smaller: 
        case t_greater: 
        case t_smallerequal:  
        case t_greaterequal:
        case t_eof:{
            printf ("predict term_tail --> epsilon\n");
            return T; /* epsilon production */
        }
        default:{
            cout << "term_tail" << endl;
            error ();
        }
    }
}

AST_node factor() 
{
    switch (input_token) {
        case t_id :{
            printf ("predict factor --> literal\n");
            match (t_id);
            return AST_node("id:" + image);;
        }
        case t_literal:{
            printf ("predict factor --> id\n");
            match (t_literal);
            return AST_node("num:" + image);;
        }
        case t_lparen:{
            printf ("predict factor --> lparen expr rparen\n");
            match (t_lparen);
            AST_node E = expr();
            E.printMode = 0;
            match (t_rparen);
            return E;
        }
        default:
            cout << "factor" << endl;
            error();
    }
}
AST_node factor_tail(AST_node F) 
{
    AST_node FT = AST_node("");

    switch (input_token) {
        case t_mul:
        case t_div:
            printf ("predict factor_tail --> mul_op factor factor_tail\n");
            FT = mul_op();
            FT.children = {F, factor()};
            FT.printMode = 0;
            return factor_tail(FT);
        case t_add:
        case t_sub:
        case t_rparen:
        case t_id:
        case t_read:
        case t_write:
        case t_while:
        case t_if:
        case t_end:
        case t_equal: 
        case t_notequal: 
        case t_smaller: 
        case t_greater: 
        case t_smallerequal:  
        case t_greaterequal:
        case t_eof:
            printf ("predict factor_tail --> epsilon\n");
            return F;          /* epsilon production */
        default: 
            cout << "factor_tail ** " << F.value << endl;
            error();
    }
}


AST_node add_op() {
    switch (input_token) {
        case t_add:
            printf ("predict add_op --> add\n");
            match (t_add);
            return AST_node("+");
        case t_sub:
            printf ("predict add_op --> sub\n");
            match (t_sub);
            return AST_node("-");
        default: 
            cout << "add_op";
            error();
    }
}

AST_node mul_op() {
    switch (input_token) {
        case t_mul:
            printf ("predict mul_op --> mul\n");
            match (t_mul);
            return AST_node("*");
        case t_div:
            printf ("predict mul_op --> div\n");
            match (t_div);
            return AST_node("/");
        default: 
            cout << "mul_op";
            error();
    }
}

int main() {
    cout << "hello world" << endl;
    input_token = scan();
    cout << input_token << " in main" << endl;
    AST_node P = program();
    P.printAST(0);
    return 0;
}