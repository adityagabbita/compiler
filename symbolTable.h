//Batch No						38
//Krishna Aditya Gabbita	2009B3A7608P
//Varun Sah						2010A7PS078P

#include<stdio.h>
#include<stdlib.h>

#include "parserDef.h"
#include "symbolTableDef.h"

//Symbol Table
extern symt init_st(abstractSyntaxTree A);
extern fn init_fn(abstractSyntaxTree A,int scope,symt s);
extern fn init_main(abstractSyntaxTree A,int scope,symt s);
extern id init_id(abstractSyntaxTree A,int scope);
extern id init_fid(abstractSyntaxTree A,int scope,char *rec);
extern void print_st(symt s);

//variable checker
extern int id_present(id head,char *iden);
extern int rectype_present(id head,char *iden);
extern int field_present(id head,char *recid,char *fid);

//offset updates
extern void offset_init(symt s);
extern int size_rec(symt s,char *rname);
extern char *type_rec(symt s,char *rname);
