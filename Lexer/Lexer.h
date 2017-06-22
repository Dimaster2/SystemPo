/* Lab2 Lexer */

enum {
	DO_SYM, ELSE_SYM, IF_SYM, WHILE_SYM, LBRA, RBRA, LPAR, RPAR,
	PLUS, MINUS, LESS, SEMI, EQUAL, INT, ID, EOI
};

char *words[] = { "do", "else", "if", "while", NULL };

int ch = ' ';
int sym;
int int_val;
char id_name[100];

void syntax_error() { fprintf(stderr, "syntax error\n"); exit(1); }

void next_ch() { ch = getchar(); }

void next_sym()
{
again: switch (ch)
{
case ' ': case '\n': next_ch(); goto again;
case EOF: sym = EOI; break;
case '{': next_ch(); sym = LBRA; break;
case '}': next_ch(); sym = RBRA; break;
case '(': next_ch(); sym = LPAR; break;
case ')': next_ch(); sym = RPAR; break;
case '+': next_ch(); sym = PLUS; break;
case '-': next_ch(); sym = MINUS; break;
case '<': next_ch(); sym = LESS; break;
case ';': next_ch(); sym = SEMI; break;
case '=': next_ch(); sym = EQUAL; break;
default:
	if (ch >= '0' && ch <= '9')
	{
		int_val = 0; 
		while (ch >= '0' && ch <= '9')
		{
			int_val = int_val * 10 + (ch - '0'); next_ch();
		}
		sym = INT;
	}
	else if (ch >= 'a' && ch <= 'z')
	{
		int i = 0; 
		while ((ch >= 'a' && ch <= 'z') || ch == '_')
		{
			id_name[i++] = ch; next_ch();
		}
		id_name[i] = '\0';
		sym = 0;
		while (words[sym] != NULL && strcmp(words[sym], id_name) != 0)
			sym++;
		if (words[sym] == NULL)
		if (id_name[1] == '\0') sym = ID; else syntax_error();
	}
	else
		syntax_error();
}
}