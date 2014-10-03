//Batch No						38
//Krishna Aditya Gabbita		2009B3A7608P
//Varun Sah						2010A7PS078P

#include "semantics.h"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int id_p(id head,char *iden)
{
	int res=0;
	
	id temp;
	temp=head;
	
	while(temp!=NULL && res==0)
	{
		if(temp->rec==0)
		{
			if(strcmp(temp->id_name,iden)==0)
			{
				res=temp->lno;
			}
		}
		
		temp=temp->next;
	}
	
	return res;
}

int rectype_p(id head,char *iden)
{
	int res=0;
	
	id temp;
	temp=head;
	
	while(temp!=NULL && res==0)
	{
		if(temp->rec==0 && temp->type==2)
		{
			if(strcmp(temp->rname,iden)==0)
			{
				res=temp->lno;
			}
		}
		
		temp=temp->next;
	}
	
	return res;
}

int field_p(id head,char *recid,char *fid)
{
	int res=0;
	
	id temp;
	temp=head;
	
	while(temp!=NULL && res==0)
	{
		if(temp->rec==1)
		{
			if(strcmp(temp->id_name,fid)==0 && strcmp(temp->rname,recid)==0)
			{
				res=temp->lno;
			}
		}
		
		temp=temp->next;
	}
	
	return res;
}

void sem_checker(symt s,abstractSyntaxTree A)
{
	//printf("\nSemantic Errors:\n");
	
	//Type 12 is taken care of in the symbol table implementation
	mult_dec(s); //Type 1
	udec_use(s,A); //Type 2 & 11
	g_rdec(s); //Type 3
	func_para(s,A); //Type 4, 6 & 7
	if_check(s,A); //Type 8
	f_overload(s); //Type 9
	rec_call(s,A); //Type 10
	assign_match(s,A); //Type 13
	undef_call(s,A); //Type 14
}

void mult_dec(symt s)
{
	mult_dec_scope(s->glist); //global variable list
	mult_dec_record(s->rec); //record type def list
	mult_dec_field(s->rec); //field type def list
	
	int i;
	
	for(i=0;i<(s->fno);i++)
	{
		mult_dec_scope(s->flist[i]->in_list); //input variables
		mult_dec_scope(s->flist[i]->out_list); //output variables
		mult_dec_scope(s->flist[i]->idlist); //explicit declarations
	}
}

void mult_dec_scope(id idlist)
{
	int count=0;
	
	id temp;
	temp=idlist;
	
	while(temp!=NULL) //no. of identifiers in idlist
	{
		count++;
		temp=temp->next;
	}
	
	int i,j;
	
	for(i=0;i<count;i++)
	{
		temp=idlist;
		
		for(j=0;j<i;j++)
		{
			temp=temp->next;
		}
		
		char id_name[100];
		int lno;
		strcpy(id_name,temp->id_name);
		lno=temp->lno;
		
		for(j=(i+1);j<count;j++)
		{
			temp=temp->next;
			if(strcmp(id_name,temp->id_name)==0)
			{
				printf("Sem_E1@Line %d: Identifier %s redeclared in same scope, previous declaration in line %d\n",temp->lno,id_name,lno);
				ts_err++;
			}
		}
	}
}

void mult_dec_record(id idlist)
{
	int count=0;
	
	id temp;
	temp=idlist;
	
	while(temp!=NULL) //no. of identifiers in idlist
	{
		count++;
		temp=temp->next;
	}
	
	int i,j;
	
	for(i=0;i<count;i++)
	{
		temp=idlist;
		
		for(j=0;j<i;j++)
		{
			temp=temp->next;
		}
		
		if(temp->rec==0)
		{
			char rec_name[100];
			int lno;
			strcpy(rec_name,temp->rname);
			lno=temp->lno;
		
			for(j=(i+1);j<count;j++)
			{
				temp=temp->next;
				if(strcmp(rec_name,temp->rname)==0 && temp->rec==0)
				{
					printf("Sem_E1@Line %d: Type Definition %s redefined, previous definition in line %d\n",temp->lno,rec_name,lno);
					ts_err++;
				}
			}
		}
	}
}

void mult_dec_field(id idlist)
{
	int count=0;
	
	id temp;
	temp=idlist;
	
	while(temp!=NULL) //no. of identifiers in idlist
	{
		count++;
		temp=temp->next;
	}
	
	int i,j;
	
	for(i=0;i<count;i++)
	{
		temp=idlist;
		
		for(j=0;j<i;j++)
		{
			temp=temp->next;
		}
		
		if(temp->rec==1)
		{
			char field_name[100];
			char rec_name[100];
			int lno;
			strcpy(field_name,temp->id_name);
			strcpy(rec_name,temp->rname);
			lno=temp->lno;
		
			for(j=(i+1);j<count;j++)
			{
				temp=temp->next;
				if(strcmp(field_name,temp->id_name)==0 && strcmp(rec_name,temp->rname)==0 && temp->rec==1)
				{
					printf("Sem_E1@Line %d: Field Definition %s of Type Definition %s redefined, previous definition in line %d\n",temp->lno,field_name,rec_name,lno);
					ts_err++;
				}
			}
		}
	}
}

void udec_use(symt s,abstractSyntaxTree A)
{
	int scope=0;
	
	abstractSyntaxTree temp;
	temp=A->child[0];
	
	while(temp->e!=53)
	{
		undec_travel(s,temp->child[0]->child[2],scope);
		temp=temp->child[1];
		scope++;
	}
	
	undec_travel(s,A->child[1]->child[0],scope);
}

void undec_travel(symt s,abstractSyntaxTree A,int scope)
{
	undec_travel_func(s,A->child[1],scope); //<declarations> primarily for typedef
	undec_travel_func(s,A->child[2],scope); //<otherStmts>
	undec_travel_func(s,A->child[3],scope); //<returnStmt>
}

void undec_travel_func(symt s,abstractSyntaxTree A,int scope)
{
	int i=0;
	
	if(A->e==2)
	{
		if(rectype_p(s->rec,A->lex)==0)
		{
			printf("SEM_E2@Line %d: Record Type %s undefined before use\n",A->linenum,A->lex);
			ts_err++;
		}
	}
	
	if(A->e==18)
	{
		if(id_p(s->flist[scope]->in_list,A->lex)==0 && id_p(s->flist[scope]->out_list,A->lex)==0 && id_p(s->flist[scope]->idlist,A->lex)==0
		   && id_p(s->glist,A->lex)==0)
		{
			printf("SEM_E2@Line %d: Identifier %s undeclared before use\n",A->linenum,A->lex);
			ts_err++;
		}
	}
	
	while(i<(A->childnum))
	{
		undec_travel_func(s,A->child[i],scope);
		i++;
	}
}

void g_rdec(symt s)
{
	id temp;
	temp=s->glist;
	
	while(temp!=NULL)
	{
		int i;
		for(i=0;i<(s->fno);i++)
		{
			int a=id_p(s->flist[i]->in_list,temp->id_name);
			int b=id_p(s->flist[i]->out_list,temp->id_name);
			int c=id_p(s->flist[i]->idlist,temp->id_name);
			
			if(a>0 || b>0 || c>0)
			{
				if(a>0)
				{
					printf("SEM_E3@Line %d: Global Identifier %s has been redeclared, previous declaration in Line %d\n",a,temp->id_name,temp->lno); 
					ts_err++;
				}
				if(b>0)
				{
					printf("SEM_E3@Line %d: Global Identifier %s has been redeclared, previous declaration in Line %d\n",b,temp->id_name,temp->lno);
					ts_err++;
				}
				if(c>0)
				{
					printf("SEM_E3@Line %d: Global Identifier %s has been redeclared, previous declaration in Line %d\n",c,temp->id_name,temp->lno);
					ts_err++;
				}
			}
		}
		temp=temp->next;
	}
}

int b_type(symt s,int scope,abstractSyntaxTree A)
{
	int res=-1;
	
	if(A->e==53) // can remove since eps never occurs in boolean expressions
	{
		res=3;
	}
	
	else if(A->e!=53 && A->e!=18 && A->e!=15 && A->e!=16 && A->e!=22) //<any of the logical and relational operators except not>
	{
		int t1,t2;
		t1=bool_check(s,scope,A->child[0]);
		t2=bool_check(s,scope,A->child[1]);
		
		if((t1==t2 || t1==3 || t2==3) && (t1!=-1))
		{
			if(t1==3)
			{
				res=t2;
			}
			else
			{
				//res=t1;
				res=4;
			}
		}
	}
	
	else if(A->e==22 ) //<not operator>
	{
		int t1;
		t1=bool_check(s,scope,A->child[0]);
		
		
		if(t1==4)
		{
			res=4;
		}
		else res=-1;
	}
	
	else if(A->e==18)			//<var> gives tk_id 
	{
		//printf("A\n");
		int flag=0;
		
		id temp;
		temp=s->glist; //global list
		
		while(temp!=NULL && flag==0)
		{
			if(strcmp(A->lex,temp->id_name)==0)
			{
				res=temp->type;
				flag++;
			}
			temp=temp->next;
		}
		
		//printf("A\n");
		
		temp=s->flist[scope]->in_list; //input parameter list
		
		while(temp!=NULL && flag==0)
		{
			if(strcmp(A->lex,temp->id_name)==0)
			{
				res=temp->type;
				flag++;
			}
			temp=temp->next;
		}
		
		temp=s->flist[scope]->out_list; //output parameter list
		
		while(temp!=NULL && flag==0)
		{
			if(strcmp(A->lex,temp->id_name)==0)
			{
				res=temp->type;
				flag++;
			}
			temp=temp->next;
		}
		
		//printf("A\n");
		
		temp=s->flist[scope]->idlist; //declarations list
		
		while(temp!=NULL && flag==0)
		{
			if(strcmp(A->lex,temp->id_name)==0)
			{
				res=temp->type;
				flag++;
			}
			temp=temp->next;
		}
		
		//printf("A\n");
	}
	
	else if(A->e==15)	//<var> gives tk_num
	{
		res=0;
	}
	
	else if(A->e==16)	//<var> gives tk_rnum
	{
		res=1;
	}
	
		
	return res;
}

void if_check(symt s,abstractSyntaxTree A)
{
	int scope=0;
	
	abstractSyntaxTree temp;
	temp=A->child[0];
	
	while(temp->e!=53)
	{
		if_check_scope(s,temp->child[0]->child[2],scope);
		temp=temp->child[1];
		scope++;
	}
	
	if_check_scope(s,A->child[1]->child[0],scope);
}

void if_check_scope(symt s,abstractSyntaxTree A,int scope)
{
	if(A->e==37) //TK_IF
	{
		int a=b_type(s,scope,A->child[0]);
		if(a!=4)
		{
			printf("SEM_E8@Line %d: The conditional expression is not a boolean type\n",A->linenum);
			ts_err++;
		}
	}
	
	int i=0;
	while(i<(A->childnum))
	{
		if_check_scope(s,A->child[i],scope);
		i++;
	}
}

void f_overload(symt s)
{
	int i,j;
	
	for(i=0;i<((s->fno)-1);i++)
	{
		char fname[100];
		strcpy(fname,s->flist[i]->fname);
		
		for(j=(i+1);j<(s->fno);j++)
		{
			if(strcmp(fname,s->flist[j]->fname)==0)
			{
				printf("SEM_E9: Function %s overloaded\n",fname);
				ts_err++;
			}
		}
	}
}

void rec_call(symt s,abstractSyntaxTree A)
{
	int scope=0;
	
	abstractSyntaxTree temp;
	temp=A->child[0];
	
	while(temp->e!=53)
	{
		rec_scope(s,temp->child[0]->child[2],s->flist[scope]->fname);
		temp=temp->child[1];
		scope++;
	}
	
	rec_scope(s,A->child[1]->child[0],"_main");
}

void rec_scope(symt s,abstractSyntaxTree A,char *fname)
{
	if(A->e==20)
	{
		if(strcmp(A->lex,fname)==0)
		{
			printf("SEM_E10@Line %d: Function %s called recursively inside itself\n",A->linenum,fname);
			ts_err++;
		}
	}
	
	int i=0;
	while(i<(A->childnum))
	{
		rec_scope(s,A->child[i],fname);
		i++;
	}
}

void assign_match(symt s,abstractSyntaxTree A)
{
	int scope=0;
	
	abstractSyntaxTree temp;
	temp=A->child[0];
	
	while(temp->e!=53)
	{
		assign_match_scope(s,temp->child[0]->child[2],scope);
		temp=temp->child[1];
		scope++;
	}
	
	assign_match_scope(s,A->child[1]->child[0],scope);
}

void assign_match_scope(symt s,abstractSyntaxTree A,int scope)
{
	if(A->e==25) //TK_ASSIGNOP
	{
		int type;
		
		if(A->child[0]->child[0]->e==53) //identifier
		{
			type=var_type(s,A->child[0]->lex,scope);
		}
		else //field
		{
			type=field_type(s,A->child[0]->lex,A->child[0]->child[0]->child[0]->lex,scope);
		}
		
		if(type!=arith_type(s,scope,A->child[1]))
		{
			printf("SEM_E13@Line %d: Assignment datatype mismatch\n",A->linenum);
			ts_err++;
		}
	}
	
	int i=0;
	while(i<(A->childnum))
	{
		assign_match_scope(s,A->child[i],scope);
		i++;
	}
}

int arith_type(symt s,int scope,abstractSyntaxTree A)
{
	int res=-1;
	
	if(A->e==53)
	{
		res=3;
	}
	
	else if(A->e!=53 && A->e!=18 && A->e!=15 && A->e!=16 && A->e!=2) //<arithmeticExpression>
	{
		int t1,t2;
		t1=arith_check(s,scope,A->child[0]);
		t2=arith_check(s,scope,A->child[1]);
		
		if((t1==t2 || t1==3 || t2==3) && (t1!=-1))
		{
			if(t1==3)
			{
				res=t2;
			}
			else
			{
				res=t1;
			}
		}
	}
	
	else if(A->e==18) //TK_ID
	{
		//printf("A\n");
		int flag=0;
		
		id temp;
		temp=s->glist; //global list
		
		while(temp!=NULL && flag==0)
		{
			if(strcmp(A->lex,temp->id_name)==0)
			{
				res=temp->type;
				flag++;
			}
			temp=temp->next;
		}
		
		//printf("A\n");
		
		temp=s->flist[scope]->in_list; //input parameter list
		
		while(temp!=NULL && flag==0)
		{
			if(strcmp(A->lex,temp->id_name)==0)
			{
				res=temp->type;
				flag++;
			}
			temp=temp->next;
		}
		
		temp=s->flist[scope]->out_list; //output parameter list
		
		while(temp!=NULL && flag==0)
		{
			if(strcmp(A->lex,temp->id_name)==0)
			{
				res=temp->type;
				flag++;
			}
			temp=temp->next;
		}
		
		//printf("A\n");
		
		temp=s->flist[scope]->idlist; //declarations list
		
		while(temp!=NULL && flag==0)
		{
			if(strcmp(A->lex,temp->id_name)==0)
			{
				res=temp->type;
				flag++;
			}
			temp=temp->next;
		}
		
		//printf("A\n");
	}
	
	else if(A->e==15) //TK_NUM
	{
		res=0;
	}
	
	else if(A->e==16) //TK_RNUM
	{
		res=1;
	}
	
	else if(A->e==2) //TK_RECORDID
	{
		if(A->child[0]->e==53)
		{
			res=2;
		}
		else
		{
			A=A->child[0]->child[0];
			
			int flag=0;
			
			id temp;
			temp=s->rec; //field list
		
			while(temp!=NULL && flag==0)
			{
				if(strcmp(A->lex,temp->id_name)==0)
				{
					res=temp->type;
					flag++;
				}
				temp=temp->next;
			}
		}
	}
	
	return res;
}

int var_type(symt s,char *idname,int scope)
{
	int res=-1;
	int flag=0;
		
	id temp;
	temp=s->glist; //global list
		
	while(temp!=NULL && flag==0)
	{
		if(strcmp(idname,temp->id_name)==0)
		{
			res=temp->type;
			flag++;
		}
		temp=temp->next;
	}
	
	temp=s->rec; //type definition
		
	while(temp!=NULL && flag==0)
	{
		if(strcmp(idname,temp->rname)==0)
		{
			if(temp->rec==0)
			{
				res=temp->type;
				flag++;
			}
		}
		temp=temp->next;
	}
		
	temp=s->flist[scope]->in_list; //input parameter list
		
	while(temp!=NULL && flag==0)
	{
		if(strcmp(idname,temp->id_name)==0)
		{
			res=temp->type;
			flag++;
		}
		temp=temp->next;
	}
		
	temp=s->flist[scope]->out_list; //output parameter list
		
	while(temp!=NULL && flag==0)
	{
		if(strcmp(idname,temp->id_name)==0)
		{
			res=temp->type;
			flag++;
		}
		temp=temp->next;
	}
		
	temp=s->flist[scope]->idlist; //declarations list
		
	while(temp!=NULL && flag==0)
	{
		if(strcmp(idname,temp->id_name)==0)
		{
			res=temp->type;
			flag++;
		}
		temp=temp->next;
	}
	
	return res;
}

int field_type(symt s,char *idname,char *fdname,int scope)
{
	int res=-1;
	int flag=0;
	
	char rname[100];
	strcpy(rname,"---");
			
	id temp;
	temp=s->glist; //global list
		
	while(temp!=NULL && flag==0)
	{
		if(strcmp(idname,temp->id_name)==0)
		{
			strcpy(rname,temp->rname);
			flag++;
		}
		temp=temp->next;
	}
	
	temp=s->flist[scope]->in_list; //input parameter list
		
	while(temp!=NULL && flag==0)
	{
		if(strcmp(idname,temp->id_name)==0)
		{
			strcpy(rname,temp->rname);
			flag++;
		}
		temp=temp->next;
	}
		
	temp=s->flist[scope]->out_list; //output parameter list
		
	while(temp!=NULL && flag==0)
	{
		if(strcmp(idname,temp->id_name)==0)
		{
			strcpy(rname,temp->rname);
			flag++;
		}
		temp=temp->next;
	}
		
	temp=s->flist[scope]->idlist; //declarations list
		
	while(temp!=NULL && flag==0)
	{
		if(strcmp(idname,temp->id_name)==0)
		{
			strcpy(rname,temp->rname);
			flag++;
		}
		temp=temp->next;
	}
	
	flag=0;
	temp=s->rec; //field list
		
	while(temp!=NULL && flag==0)
	{
		if(strcmp(fdname,temp->id_name)==0)
		{
			if(strcmp(rname,temp->rname)==0)
			{
				res=temp->type;
				flag++;
			}
		}
		temp=temp->next;
	}
	
	return res;
}

void undef_call(symt s,abstractSyntaxTree A)
{
	int scope=0;
	
	abstractSyntaxTree temp;
	temp=A->child[0];
	
	while(temp->e!=53)
	{
		undef_call_scope(s,temp->child[0]->child[2],scope);
		temp=temp->child[1];
		scope++;
	}
	
	undef_call_scope(s,A->child[1]->child[0],scope);
}

void undef_call_scope(symt s,abstractSyntaxTree A,int scope)
{
	if(A->e==20)
	{
		int dec=0;
		int x=0;
		
		while(x<=scope)
		{
			if(strcmp(s->flist[x]->fname,A->lex)==0)
			{
				dec++;
			}
			x++;
		}
		
		if(dec==0)
		{
			printf("SEM_E14@Line %d: Function %s undefined before use in Function %s\n",A->linenum,A->lex,s->flist[scope]->fname);
			ts_err++;
		}
	}
	
	int i=0;
	while(i<(A->childnum))
	{
		undef_call_scope(s,A->child[i],scope);
		i++;
	}
}

void func_para(symt s,abstractSyntaxTree A)
{
	int scope=0;
	
	abstractSyntaxTree temp;
	temp=A->child[0];
	
	while(temp->e!=53)
	{
		func_para_scope(s,temp->child[0]->child[2],scope);
		temp=temp->child[1];
		scope++;
	}
	
	func_para_scope(s,A->child[1]->child[0],scope);
}

void func_para_scope(symt s,abstractSyntaxTree A,int scope)
{
	if(A->e==20) //TK_FUNID
	{
		char fname[100];
		strcpy(fname,A->lex);
		
		int i,fscope;
		fscope=0;
		
		for(i=0;i<(s->fno);i++)
		{
			if(strcmp(fname,s->flist[i]->fname)==0)
			{
				fscope=i;
			}
		}
		
		id temp;
		temp=s->flist[fscope]->out_list; //Output parameters
		
		abstractSyntaxTree temp1;
		temp1=A->child[0]; //Output patrameters
		
		int flag=0;
		
		while(temp!=NULL && temp1->e!=53)
		{
			if((temp->type)!=var_type(s,temp1->lex,scope))
			{
				char *a,*b;
				a=var_type_print(var_type(s,temp1->lex,scope));
				b=var_type_print(temp->type);
				printf("SEM_E4@Line %d: Output parameter %s is type %s, expected to be of type %s in function %s\n",temp1->linenum,temp1->lex,a,b,fname);
				ts_err++;
			}
			temp=temp->next;
			temp1=temp1->child[0];
		}
		
		if(temp!=NULL || temp1->e!=53)
		{
			flag++;
		}
		
		if(flag>0)
		{
			printf("SEM_E4@Line %d: Mismatch of number of Output parameters in function %s\n",A->linenum,fname);
			ts_err++;
		}
		
		temp=s->flist[fscope]->in_list; //Input parameters
		
		temp1=A->child[1]; //Input patrameters
		
		flag=0;
		
		while(temp!=NULL && temp1->e!=53)
		{
			if((temp->type)!=var_type(s,temp1->lex,scope))
			{
				char *a,*b;
				a=var_type_print(var_type(s,temp1->lex,scope));
				b=var_type_print(temp->type);
				printf("SEM_E7@Line %d: Input parameter %s is type %s, expected to be of type %s in function %s\n",temp1->linenum,temp1->lex,a,b,fname);
				ts_err++;
			}
			temp=temp->next;
			temp1=temp1->child[0];
		}
		
		if(temp!=NULL || temp1->e!=53)
		{
			flag++;
		}
		
		if(flag>0)
		{
			printf("SEM_E7@Line %d: Mismatch of number of Input parameters in function %s\n",A->linenum,fname);
			ts_err++;
		}
	}
	
	int i=0;
	while(i<(A->childnum))
	{
		func_para_scope(s,A->child[i],scope);
		i++;
	}
}

char *var_type_print(int type)
{
	char *res;
	res=(char *)malloc(20*sizeof(char));
	
	if(type==0)
	{
		strcpy(res,"int");
	}
	else if(type==1)
	{
		strcpy(res,"real");
	}
	else if(type==2)
	{
		strcpy(res,"record");
	}
	else
	{
		strcpy(res,"unknown");
	}
	
	return res;
}
