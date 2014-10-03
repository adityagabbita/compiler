//Batch No						38
//Krishna Aditya Gabbita	2009B3A7608P
//Varun Sah						2010A7PS078P

#include<stdio.h>
#include<stdlib.h>

#include "parserDef.h"
#include "symbolTableDef.h"

extern void sem_checker(symt s,abstractSyntaxTree A);

//variable checker
extern int id_p(id head,char *iden);
extern int rectype_p(id head,char *iden);
extern int field_p(id head,char *recid,char *fid);

//multiple declaration in single scope
extern void mult_dec(symt s);
extern void mult_dec_scope(id idlist);
extern void mult_dec_record(id idlist);
extern void mult_dec_field(id idlist);

//declaration before use
extern void udec_use(symt s,abstractSyntaxTree A);
extern void undec_travel(symt s,abstractSyntaxTree A,int scope);
extern void undec_travel_func(symt s,abstractSyntaxTree A,int scope);

//global redeclaration
extern void g_rdec(symt s);

//if statement boolean type
extern int b_type(symt s,int scope,abstractSyntaxTree A);
extern void if_check(symt s,abstractSyntaxTree A);
extern void if_check_scope(symt s,abstractSyntaxTree A,int scope);

//function overloading
extern void f_overload(symt s);

//recursive call
extern void rec_call(symt s,abstractSyntaxTree A);
extern void rec_scope(symt s,abstractSyntaxTree A,char *fname);

//Assignment match
extern void assign_match(symt s,abstractSyntaxTree A);
extern void assign_match_scope(symt s,abstractSyntaxTree A,int scope);
extern int arith_type(symt s,int scope,abstractSyntaxTree A);
extern int var_type(symt s,char *idname,int scope);
extern int field_type(symt s,char *idname,char *fdname,int scope);
extern char *var_type_print(int type);

//Function call before definition
extern void undef_call(symt s,abstractSyntaxTree A);
extern void undef_call_scope(symt s,abstractSyntaxTree A,int scope);

//Function parameters check
extern void func_para(symt s,abstractSyntaxTree A);
extern void func_para_scope(symt s,abstractSyntaxTree A,int scope);
