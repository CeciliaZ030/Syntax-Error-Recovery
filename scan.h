#include <vector>

using std::cout;
using std::endl;

typedef enum {
	t_read, t_write, t_if, t_while, t_end,
	t_literal, t_id, t_gets, t_do,
    t_add, t_sub, t_mul, t_div, t_lparen, t_rparen, 
    t_equal, t_notequal, t_smaller, t_greater, t_smallerequal, t_greaterequal,
    t_eof
} token;


extern char token_image[128];
extern std::string getImage();
extern token scan();
extern void indent(int level);
extern bool contains(std::vector<token> set, token t);

class AST_node
{
    public: std::vector<AST_node> children; 
    		std::string value;
    		int printMode;

        AST_node(std::string val)
        {
                std::vector<AST_node> init;
                children = init;
                value = val;
                printMode = 0;
        }

        void printAST(int level)
        {
        	switch (printMode){

		        case 3:
		        	/* Program
						(Program
							[
								stmt
								stmt
								.....
							]
						)
		        	*/
		        	indent(level);
		            cout << "(" << value << endl;
		            level++;
		            indent(level);
		            cout << "[" << endl;
		            level++;
		            for (std::vector<AST_node>::iterator it = children.begin(); it != children.end(); ++it)
		                it -> printAST(level);
		            indent(level--);
		            cout << "]" << endl;
		            level--;
		            indent(level);
		            cout << ")" << endl;
		            return;

        		case 0:
        			/* Append to a stmt
						... 1 + 1 ...
        			*/
        			cout << value << " ";
                    for (std::vector<AST_node>::iterator it = children.begin(); it != children.end(); ++it)
		                it -> printAST(level);
        			return;

        		case 1:
        			/* New stmt, new line
						read ...
        			*/
        			indent(level);
		            cout << "(" << value << " ";
		            for (std::vector<AST_node>::iterator it = children.begin(); it != children.end(); ++it)
		                it -> printAST(level);
		            cout << ")" << endl;
		            return;

		        case 2:
		        	/* If stmt, indent children
						(if
							(condition)
							[
								stmt
								stmt
								.....
							]
						)
		        	*/
        			indent(level); 
		            cout << "(" << value << endl;

		            level++;
		            indent(level);

		            cout << "(";
		            children[0].printAST(level);
		            cout << ")" << endl;

		            indent(level);
		            cout << "[" << endl;
		            level++;
		            for(int i = 1; i < children.size(); i++){
		            	children[i].printAST(level);
		            }

		            level--;
		            indent(level);
		            cout << "]" << endl;
		            level--;
		           	indent(level);
		            cout << ")" << endl;
		            return;

		        default:
		        	cout << "exit in default " << value << endl;
		        	exit(1);

        	}
        	exit(1);

        } 
};
