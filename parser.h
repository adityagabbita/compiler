//Batch No						38
//Krishna Aditya Gabbita	2009B3A7608P
//Varun Sah						2010A7PS078P

#include<stdio.h>
#include<stdlib.h>

#include "parserDef.h"

extern void createParseTable(grammar G,table T);
extern parseTree parseInputSourceCode(FILE *fp, table T,grammar G);
extern void printParseTree(parseTree pt,char *outfile,int *totalAllocatedMemory);
extern abstractSyntaxTree createAbstractSyntaxtree(parseTree T,abstractSyntaxTree A);
extern void printAST(abstractSyntaxTree A,char *outFile,int *totalAllocatedMemory);

//FIRST and FOLLOW initialization and population functions
extern grammar init_ff(grammar G, FILE *fp1, FILE * fp2); // initialize each node of grammar with first and follow info
extern grammar rulefirst(grammar G); //to automate the FIRST sets of the ruels using FIRST of non-terminals
extern llist insertfirst(llist first,int e);
extern llist insertfirstset(llist first,int e,int *flag,grammar G);
extern void print_firstrules(grammar G);
extern void init_td(); //initialize token names and non-terminal names
extern void print_parsetable(table T); //print the parsetable for debugging purposes

//stack functions
extern stack push(stack s,int e);
extern int top(stack s);
extern stack pop(stack s);
extern int isEmpty (stack s);
extern stack initStack();

//grammar initialization and population functions
extern grammar init_grammar(FILE *fp); //initialize the grammar data structure from file fp
//extern rulenode add_rule(grammar G,int *r,int det,int rn,int len); //add the rule (interger array) to the corresponding determinant
extern void print_grammar(grammar G);

//Predictive parsing and dfs
extern void parse(FILE *fp,table T,grammar G);
extern void dfs(parseTree pt);
extern void print_parsetable_file(FILE *fp,table T);

//Abstract Syntax Tree construction
extern abstractSyntaxTree ast(abstractSyntaxTree *temp,int rno,int childnum);
