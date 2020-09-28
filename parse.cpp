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

const std::vector<token> FIRST_STMT = {t_id, t_read, t_write, t_if, t_while};
const std::vector<token> FIRST_STMTLIST = {t_id, t_read, t_write, t_if, t_while};
const std::vector<token> FIRST_COND = {t_lparen, t_literal, t_id};
const std::vector<token> FIRST_EXPR = {t_lparen, t_literal, t_id};

const std::vector<token> FOLLOW_STMT = {t_id, t_read, t_write, t_if, t_while};
const std::vector<token> FOLLOW_STMTLIST = {t_end};
const std::vector<token> FOLLOW_COND = {t_id, t_read, t_write, t_if, t_while};
const std::vector<token> FOLLOW_EXPR = {t_rparen, t_equal, t_notequal, t_smaller, t_greater, 
                                        t_smallerequal, t_greaterequal, t_id, t_read, t_write, t_if, t_while};

static token input_token;
static string image = "";
static bool hasError = false;


void error() {
    cout << "Syntax Error" << endl;
    exit (1);
}

bool contains(std::vector<token> set, token t){
    return std::find(set.begin(), set.end(), t) != set.end();
}


bool match (token expected) {
    cout << "   in match " << expected << " " << input_token << endl;
    if (input_token == expected){
        cout << "   input_token == expected" << endl;
        if (input_token == t_id || input_token == t_literal){
            image = getImage();
            cout << "   match" << image << endl;
        }
        input_token = scan();
        cout << "   scan next and get " << input_token << endl;
        return true;
    } 
    else{
        cout << "Umatch Syntax with " << input_token << ". " << "Expect: " << expected << endl;
        return false;
    }
}


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
    std::vector<AST_node> SL;
    switch (input_token) {
        case t_id:
        case t_read:
        case t_write:
        case t_if:
        case t_while:
        case t_eof:
            printf ("predict program --> stmt_list eof\n");
            SL = stmt_list();
            if (match(t_eof)) {
                P.children = SL;
                P.printMode = 3;
                return P;
            }
        default:
            cout << "Try to recover in Program " << endl;
            while (true) {
                if (contains(FIRST_STMTLIST, input_token)){
                    SL = stmt_list();
                    P.children = SL;
                    P.printMode = 3;
                    return P;
                }
                else if (contains(FOLLOW_STMTLIST, input_token) || input_token == t_eof)
                    return AST_node("ERROR");
                else
                    input_token = scan();
            }
    }
}

std::vector<AST_node> stmt_list()
{
    std::vector<AST_node> SL;
    std::vector<AST_node> tail;
    switch (input_token) {
        case t_id:
        case t_read:
        case t_if:
        case t_while:
        case t_write: 
            printf ("predict stmt_list --> stmt stmt_list\n");

            SL.push_back(stmt());
            tail = stmt_list();
            SL.insert(SL.end(), //TODO: possibly empty tail here
                      tail.begin(), tail.end());

            return SL;
        case t_eof: {
            printf("predict stmt_list --> epsilon\n");
            return {}; //return empty vec
        }
        case t_end: {
            printf ("predict stmt_list --> epsilon (end of if/while stmt_list)\n");
            return {};
        }
        default:
            cout << "Try to recover in Stmt_list " << endl;
            while (true) {
                if (contains(FIRST_STMTLIST, input_token))
                    return stmt_list();
                else if (contains(FOLLOW_STMTLIST, input_token) || input_token == t_eof)
                    return {AST_node("ERROR")};
                else
                    input_token = scan();
            }
    }
}

AST_node stmt() 
{
    AST_node S = AST_node("");
    switch (input_token) {
        case t_id: {
            printf ("predict stmt --> id_gets expr\n");
            if (match(t_id) && match(t_gets)){
                S.value = ":=";
                S.children = {AST_node("id:" + image), expr()};
                S.printMode = 1;
                return S;
            }
        }
        case t_read:{
            printf ("predict stmt --> read id\n");
            if (match(t_read) && match(t_id)) {
                match(t_id);
                S.value = "read";
                S.children = {AST_node("id:" + image)};
                S.printMode = 1;
            }
            return S;
        }
        case t_write:{
            printf ("predict stmt --> write expr\n");
            if (match(t_write)) {
                S.value = "write";
                S.children = {expr()};
                S.printMode = 1;
                return S;
            }
        }
        case t_if:{
            printf ("predict stmt --> if condition stmt_list\n");
            match(t_if);
            S.value = "if";
            S.children = {condition()};
            std::vector<AST_node> ifSubS = stmt_list();
            S.printMode = 2;
            if (match(t_end)){
                S.children.insert(S.children.end(), 
                                     ifSubS.begin(), ifSubS.end());
                return S;
            }
    
        }
        case t_while:{
            printf ("predict stmt --> while condition stmt_list\n");
            match(t_while);
            S.value = "while";
            S.children = {condition()};
            std::vector<AST_node> whileSubS = stmt_list();
            S.printMode = 2;
            if (match(t_end)){
                S.children.insert(S.children.end(), 
                                     whileSubS.begin(), whileSubS.end());
                return S;
            }
    
        }
        default:
            cout << "Try to recover in Stmt " << endl;
            while (true) {
                if (contains(FIRST_STMT, input_token))
                    return stmt();
                else if (contains(FOLLOW_STMT, input_token) || input_token == t_eof)
                    return AST_node("ERROR");
                else
                    input_token = scan();
            }
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
            cout << "Try to recover in Condition " << endl;
            while (true) {
                if (contains(FIRST_COND, input_token))
                    return condition();
                else if (contains(FOLLOW_COND, input_token) || input_token == t_eof)
                    return AST_node("ERROR");
                else
                    input_token = scan();
            }
            exit(1);
    }
    AST_node rexpr = expr();
    C.children = {lexpr, rexpr};
    C.printMode = 0;
    return C;
}

AST_node expr() 
{
    AST_node T = AST_node("");
    bool isParan = false;
    switch (input_token) {
        case t_lparen:
            isParan = true;
        case t_id:
        case t_literal:
            printf ("predict expr --> term term_tail\n");
            T = term();
            T.printMode = 0;
            cout << "return from E" << endl;
            return term_tail(T);
        default: 
            cout << "Try to recover in Expr " << endl;
            while (true) {
                if (contains(FIRST_STMT, input_token))
                    return expr();
                else if (contains(FOLLOW_STMT, input_token) || input_token == t_eof)
                    return {AST_node("ERROR")};
                else
                    input_token = scan();
            }
            
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
            cout << "In term " << endl;
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
        case t_eof:
            printf ("predict term_tail --> epsilon\n");
            return T; /* epsilon production */
        default:
            cout << "In term_tail, with term value " << T.value << endl;
            error ();
    }
}

AST_node factor() 
{
    switch (input_token) {
        case t_id :{
            printf ("predict factor --> id\n");
            match (t_id);
            return AST_node("id:" + image);;
        }
        case t_literal:{
            printf ("predict factor --> literal\n");
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
            cout << "In factor ";
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
            cout << "In factor_tail, with factor value: " << F.value << endl;
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
            cout << "In add_op ";
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
            cout << "In mul_op ";
            error();
    }
}

int main() {
    cout << "hello world" << endl;
    input_token = scan();
    cout << input_token << " in main" << endl;
    AST_node P = program();

    cout << endl;
    if (!hasError)
        cout << "Tree" << endl;
    else
        cout << "Error Tree" << endl;

    P.printAST(0);
    return 0;
}