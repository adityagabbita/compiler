//Batch No						38
//Krishna Aditya Gabbita	2009B3A7608P
//Varun Sah						2010A7PS078P

#include<stdio.h>
#include<stdlib.h>

#include "parserDef.h"
#include "symbolTableDef.h"

//Type extractor and checker
extern void type_check(symt s,abstractSyntaxTree A);
extern void travel(symt s,int scope,abstractSyntaxTree A);
extern int arith_check(symt s,int scope,abstractSyntaxTree A);
extern int bool_check(symt s,int scope,abstractSyntaxTree A);
