
typedef enum {
	t_read, t_write, t_if, t_while, t_end,
	t_literal, t_id, t_gets,
    t_add, t_sub, t_mul, t_div, t_lparen, t_rparen, 
    t_equal, t_notequal, t_smaller, t_greater, t_smallerequal, t_greaterequal,
    t_eof
} token;


extern char token_image[128];

extern token scan();

