//Batch No						38
//Krishna Aditya Gabbita	2009B3A7608P
//Varun Sah						2010A7PS078P

#include<stdio.h>
#include<stdlib.h>

#include "lexerDef.h"

extern FILE *getStream(FILE *fp,char *B,int k);
extern tokenInfo *getNextToken(char *B,int k);

//hardcoded dfa initialization
void popst(); //for populating the DFA table
void ascii(); //for initializing the table to map ASCII values of character to column in DFA
void tokens(); //for initializing token names of lexemes
void print_tables(); //to print the DFA tables and ASCII mapping for debugging purposes

//token list helper functions
extern tlist add(tlist t,tokenInfo ti); //to add tokens to a list
extern tlist initList ();	//initialize the token list
extern void print_tlist(tlist t); //print the token list

//token class mapping
extern int tk_class(int a); //to map accepting state numbers in DFA to Parse Table column numbers

//comment preprocessing
extern FILE *process(FILE *fp1,FILE *fp);
