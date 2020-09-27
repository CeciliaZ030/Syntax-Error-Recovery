#include <vector>

typedef enum {
	t_read, t_write, t_if, t_while, t_end,
	t_literal, t_id, t_gets, t_do,
    t_add, t_sub, t_mul, t_div, t_lparen, t_rparen, 
    t_equal, t_notequal, t_smaller, t_greater, t_smallerequal, t_greaterequal,
    t_eof
} token;

class AST_node
{
        public: std::vector<AST_node> children; 
        		std::string value;

        AST_node(std::string val)
        {
                std::vector<AST_node> init;
                children = init;
                value = val;
        }
};

extern char token_image[128];
extern std::string getImage();
extern token scan();


