//Batch No						38
//Krishna Aditya Gabbita	2009B3A7608P
//Varun Sah						2010A7PS078P

#include<stdio.h>
#include<stdlib.h>

#include "parserDef.h"
#include "symbolTableDef.h"

extern void asm_code(symt s,abstractSyntaxTree A,FILE *fp);
extern void travel_scope(symt s,abstractSyntaxTree A,int scope,FILE *fp);
extern void arith_eval(symt s,abstractSyntaxTree A,int scope,FILE *fp);
extern void cond_eval(symt s,abstractSyntaxTree A,int scope,FILE *fp);
extern void bool_eval(symt s,abstractSyntaxTree A,int scope,FILE *fp);
extern void loop_eval(symt s,abstractSyntaxTree A,int scope,FILE *fp);

//offset of variables
extern int var_offset(symt s,int scope,char *iden);
extern int field_offset(symt s,char *rname,char *fdname);
extern char *rec_type(symt s,char *idname,int scope);
extern int var_size(symt s,char *idname,int scope);
