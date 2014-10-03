//Batch No						38
//Krishna Aditya Gabbita	2009B3A7608P
//Varun Sah						2010A7PS078P

typedef struct token tokenInfo;
typedef struct tnode *tlist;

int st[170][85];
int asc[128];
char tk[170][20];

char lx[101]; //lexeme
int *state,*j,*lnum;

struct token
{
	char lex[101];
	char tok[20];
	int lnum;
	int tkclass;
};

struct tnode
{
	tokenInfo ti;
	tlist next;
};
