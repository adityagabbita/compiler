//Batch No						38
//Krishna Aditya Gabbita		2009B3A7608P
//Varun Sah						2010A7PS078P

#include "typeExtractor.h"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void type_check(symt s,abstractSyntaxTree A)
{
	int i=0;
	
	abstractSyntaxTree temp;
	temp=A->child[0];
	
	while(temp->e!=53)
	{
		//printf("\nIn Function %s:\n",s->flist[i]->fname);
		
		abstractSyntaxTree temp1;
		temp1=temp->child[0];
		
		travel(s,i,temp1);
		
		temp=temp->child[1];
		
		i++;
	}
	
	temp=A->child[1];
	
	//printf("\nIn Function %s:\n",s->flist[i]->fname);
	travel(s,i,temp);
}

void travel(symt s,int scope,abstractSyntaxTree A)
{
	//Arithmetic checkers
	if(A->e==132)
	{
		char type[20];
		int a=arith_check(s,scope,A);
		
		if(a==0)
		{
			strcpy(type,"int");
		}
		else if(a==1)
		{
			strcpy(type,"real");
		}
		else if(a==2)
		{
			strcpy(type,"record");
		}
		else
		{
			strcpy(type,"mismatch");
		}
		
		if(a==-1)
		{
			printf("TC_ERROR1@Line %d: Arithmetic expression datatype mismatch\n",A->linenum);
			ts_err++;
		}
		//printf("Type of Arithmetic expression on line %d: \n",A->linenum);
	}
	
	//Boolean checkers
	else if((A->e==4)||(A->e==13)||(A->e==22)||(A->e==24)||(A->e==26)||(A->e==27)||(A->e==28)||(A->e==29)||(A->e==1))
	{
		char type[20];
		int a=bool_check(s,scope,A);
		
		if(a==0)
		{
			strcpy(type,"integer. Mismatch"); 	//should never occur since it would cause a parsing error 
		}
		else if(a==1)
		{
			strcpy(type,"real. Mismatch");		//should never occur since it would cause a parsing error 
		}
		else if(a==2)
		{
			strcpy(type,"record. Mismatch");	//should never occur since it would cause a parsing error 
		}
		else if(a==4)
		{
			strcpy(type,"boolean");
		}
		else
		{
			strcpy(type,"mismatch");
		}
		
		if(a==-1)
		{
			printf("TC_ERROR2@Line %d: Boolean expression datatype mismatch\n",A->linenum);
			ts_err++;
		}
		//printf("Type of Arithmetic expression on line %d: \n",A->linenum);
	}
	else
	{
		int i=0;
		
		while(i<(A->childnum))
		{
			travel(s,scope,A->child[i]);
			i++;
		}
	}
}

int arith_check(symt s,int scope,abstractSyntaxTree A)
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

int bool_check(symt s,int scope,abstractSyntaxTree A)
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
