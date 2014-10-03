//Batch No						38
//Krishna Aditya Gabbita	2009B3A7608P
//Varun Sah						2010A7PS078P

#ifndef DATA
#define DATA

typedef struct _determinant determinant;
typedef determinant *grammar;
typedef struct _rule rule;
typedef rule *rulenode;
typedef struct _llist *llist;
typedef int table[50][55];
typedef struct _stack *stack;
typedef struct _parseTree *parseTree;
typedef struct _abstractSyntaxTree *abstractSyntaxTree;

#define ndet 50

int parse_err;
char td[150][30];

struct _determinant
{
	char detname[30];
	int num_rules; //no. of rules for determinant
	rulenode head;
	int *first;
	int *follow;
	int firstlen;
	int followlen;
};

struct _rule
{
	int rno; //universal rule no.
	int len; //length of rule string
	int *head;
	llist first;
	int eflag; //eflag set to 1 if rule has epsilon in FIRST
	rulenode next;
};

struct _llist
{
	int e;
	llist next;	
};

struct _stack
{
	int e;
	parseTree pt;
	stack next;
};

struct _parseTree
{
	int e;
	int rno;
	int linenum;
	char lex[101];
	parseTree parent;
	parseTree *child;
	int childnum;
};

struct _abstractSyntaxTree
{
	int e;
	int rno;
	int linenum;
	int childnum;
	char lex[101];
	abstractSyntaxTree *child;
	abstractSyntaxTree parent;
};

#endif
