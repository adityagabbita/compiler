//Batch No						38
//Krishna Aditya Gabbita	2009B3A7608P
//Varun Sah						2010A7PS078P

#include "symbolTable.h"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

symt init_st(abstractSyntaxTree A)
{
	s=(symt)malloc(sizeof(struct _st));
	
	s->fno=1;
	
	abstractSyntaxTree temp;
	temp=A->child[0];
	
	while(temp->e!=53)
	{
		(s->fno)++;
		temp=temp->child[1];
	}
	
	s->glist=NULL;
	s->rec=NULL;
	s->flist=(fn *)malloc((s->fno)*sizeof(fn));
	
	int i;
	temp=A->child[0];
	
	//printf("No. of functions: %d\n",s->fno);
	
	for(i=0;i<((s->fno)-1);i++)
	{
		//printf("i: %d\n",i);
		s->flist[i]=init_fn(temp->child[0],(i+1),s);
		temp=temp->child[1];
	}
	
	s->flist[((s->fno)-1)]=init_main(A->child[1],(s->fno),s);
	
	offset_init(s);
	
	return s;
}

fn init_fn(abstractSyntaxTree A,int scope,symt s)
{
	fn f;
	f=(fn)malloc(sizeof(struct _fn));
	
	f->scope=scope;
	f->fname=(char *)malloc(101*sizeof(char));
	strcpy(f->fname,A->lex);
	f->in_list=NULL;
	f->out_list=NULL;
	f->idlist=NULL;
	
	abstractSyntaxTree ti;
	ti=A->child[0]->child[0];
	
	//printf("A\n");
	
	while(ti->e!=53)
	{
		if(f->in_list==NULL)
		{
			//sprintf("A\n");
			f->in_list=init_id(ti,scope);
			//printf("A\n");
		}
		else
		{
			id temp;
			temp=f->in_list;
			
			while(temp->next!=NULL)
			{
				temp=temp->next;
			}
			
			temp->next=init_id(ti,scope);
		}
		
		//printf("A\n");
		
		ti=ti->child[1];
	}
	
	//printf("A\n");
	
	abstractSyntaxTree to;
	to=A->child[1];
	
	if(to->e!=53)
	{
		to=to->child[0];
		
		while(to->e!=53)
		{
			if(f->out_list==NULL)
			{
				f->out_list=init_id(to,scope);
			}
			else
			{
				id temp;
				temp=f->out_list;
				
				while(temp->next!=NULL)
				{
					temp=temp->next;
				}
				
				temp->next=init_id(to,scope);
			}
		
			to=to->child[1];
		}
	}
	
	abstractSyntaxTree tid;
	tid=A->child[2]->child[0]; //<typeDefinitions>
	
	while(tid->e!=53)
	{
		if(s->rec==NULL)
		{
			s->rec=init_id(tid,scope);
			
			abstractSyntaxTree temp1;
			temp1=tid->child[0];
			
			while(temp1->e!=53)
			{
				id temp;
				temp=s->rec;
			
				while(temp->next!=NULL)
				{
					temp=temp->next;
				}
			
				temp->next=init_fid(temp1,scope,tid->lex);
				
				temp1=temp1->child[1];
			}
		}
		else
		{
			id temp;
			temp=s->rec;
			
			while(temp->next!=NULL)
			{
				temp=temp->next;
			}
			
			temp->next=init_id(tid,scope);
			
			abstractSyntaxTree temp1;
			temp1=tid->child[0];
			
			while(temp1->e!=53)
			{
				temp=s->rec;
			
				while(temp->next!=NULL)
				{
					temp=temp->next;
				}
			
				temp->next=init_fid(temp1,scope,tid->lex);
				
				temp1=temp1->child[1];
			}
		}
		
		tid=tid->child[1];
	}

	tid=A->child[2]->child[1]; //<declarations>
	
	while(tid->e!=53)
	{
		if(tid->child[1]->e==53) //non-global
		{
			if(f->idlist==NULL)
			{
				f->idlist=init_id(tid,scope);
			}
			else
			{
				id temp;
				temp=f->idlist;
			
				while(temp->next!=NULL)
				{
					temp=temp->next;
				}
			
				temp->next=init_id(tid,scope);
			}
		}
		
		else
		{
			if(s->glist==NULL)
			{
				s->glist=init_id(tid,0);
			}
			else
			{
				id temp;
				temp=s->glist;
			
				while(temp->next!=NULL)
				{
					temp=temp->next;
				}
			
				temp->next=init_id(tid,0);
			}
		}
		
		tid=tid->child[2];
	}
	
	return f;
}

fn init_main(abstractSyntaxTree A,int scope,symt s)
{
	fn f;
	f=(fn)malloc(sizeof(struct _fn));
	
	f->scope=scope;
	f->fname=(char *)malloc(101*sizeof(char));
	strcpy(f->fname,A->lex);
	f->in_list=NULL;
	f->out_list=NULL;
	f->idlist=NULL;
	
	abstractSyntaxTree tid;
	tid=A->child[0]->child[0]; //<typeDefinitions>
	
	while(tid->e!=53)
	{
		if(s->rec==NULL)
		{
			s->rec=init_id(tid,scope);
			
			abstractSyntaxTree temp1;
			temp1=tid->child[0];
			
			while(temp1->e!=53)
			{
				id temp;
				temp=s->rec;
			
				while(temp->next!=NULL)
				{
					temp=temp->next;
				}
			
				temp->next=init_fid(temp1,scope,tid->lex);
				
				temp1=temp1->child[1];
			}
		}
		else
		{
			id temp;
			temp=s->rec;
			
			while(temp->next!=NULL)
			{
				temp=temp->next;
			}
			
			temp->next=init_id(tid,scope);
			
			abstractSyntaxTree temp1;
			temp1=tid->child[0];
			
			while(temp1->e!=53)
			{
				temp=s->rec;
			
				while(temp->next!=NULL)
				{
					temp=temp->next;
				}
			
				temp->next=init_fid(temp1,scope,tid->lex);
				
				temp1=temp1->child[1];
			}
		}
		
		tid=tid->child[1];
	}

	tid=A->child[0]->child[1]; //<declarations>
	
	while(tid->e!=53)
	{
		if(tid->child[1]->e==53) //non-global
		{
			if(f->idlist==NULL)
			{
				f->idlist=init_id(tid,scope);
			}
			else
			{
				id temp;
				temp=f->idlist;
			
				while(temp->next!=NULL)
				{
					temp=temp->next;
				}
			
				temp->next=init_id(tid,scope);
			}
		}
		
		else
		{
			if(s->glist==NULL)
			{
				s->glist=init_id(tid,0);
			}
			else
			{
				id temp;
				temp=s->glist;
			
				while(temp->next!=NULL)
				{
					temp=temp->next;
				}
			
				temp->next=init_id(tid,0);
			}
		}
		
		tid=tid->child[2];
	}
	
	return f;
}

id init_id(abstractSyntaxTree A,int scope)
{
	//printf("A\n");
	id iden;
	iden=(id)malloc(sizeof(struct _id));
	
	iden->scope=scope;
	iden->next=NULL;
	
	if((A->e)==18 && A->childnum==2) //function i/p & o/p
	{
		if(A->child[0]->e==39)
		{
			iden->type=0;
			iden->rec=0;
			iden->rname=NULL;
		}
		else if(A->child[0]->e==45)
		{
			iden->type=1;
			iden->rec=0;
			iden->rname=NULL;
		}
		else
		{
			iden->type=2;
			iden->rec=0;
			iden->rname=(char *)malloc(101*sizeof(char));
			//printf("A\n");
			strcpy(iden->rname,A->child[0]->lex);
			//printf("A\n");
		}
		
		iden->lno=A->linenum;
		iden->id_name=(char *)malloc(21*sizeof(char));
		strcpy(iden->id_name,A->lex);
		//printf("%s\n",iden->id_name);
	}
	
	//printf("A\n");
	
	if((A->e)==18 && A->childnum==3) //<declarations>
	{
		if(A->child[0]->e==39)
		{
			iden->type=0;
			iden->rec=0;
			iden->rname=NULL;
		}
		else if(A->child[0]->e==45)
		{
			iden->type=1;
			iden->rec=0;
			iden->rname=NULL;
		}
		else
		{
			iden->type=2;
			iden->rec=0;
			iden->rname=(char *)malloc(101*sizeof(char));
			strcpy(iden->rname,A->child[0]->lex);
		}
		
		iden->lno=A->linenum;
		iden->id_name=(char *)malloc(21*sizeof(char));
		strcpy(iden->id_name,A->lex);
		//printf("%s\n",iden->id_name);
	}
	
	if((A->e)==2)
	{
		iden->type=2;
		iden->rec=0;
		iden->lno=A->linenum;
		iden->rname=(char *)malloc(101*sizeof(char));
		strcpy(iden->rname,A->lex);
		//printf("%s\n",iden->rname);
		iden->id_name=(char *)malloc(21*sizeof(char));
		strcpy(iden->id_name,"N/A");
	}
	
	return iden;
}

id init_fid(abstractSyntaxTree A,int scope,char *rec)
{
	id fid;
	fid=(id)malloc(sizeof(struct _id));
	
	if(A->child[0]->e==39)
	{
		fid->type=0;
	}
	else
	{
		fid->type=1;
	}
	
	fid->rec=1;
	fid->scope=scope;
	fid->lno=A->linenum;
	fid->id_name=(char *)malloc(21*sizeof(char));
	strcpy(fid->id_name,A->lex);
	//printf("%s\n",fid->id_name);
	fid->rname=(char *)malloc(101*sizeof(char));
	strcpy(fid->rname,rec);
	//printf("%s\n",fid->rname);
	fid->next=NULL;
		
	return fid;
}

void print_st(symt s)
{
	//printf("\nGlobal Identifiers:\n");
	printf("%30s\t%30s\t%30s\t%10s\n","Identifier Name","Scope","Type","Offset");
	
	id temp;
	temp=s->glist;
	
	while(temp!=NULL)
	{
		char type[101];
		if(temp->type==0)
		{
			strcpy(type,"int");
		}
		else if(temp->type==1)
		{
			strcpy(type,"real");
		}
		else
		{
			strcpy(type,type_rec(s,temp->rname));
		}
		
		printf("%30s\t%30s\t%30s\t%10d\n",temp->id_name,"global",type,temp->offset);
		//printf("Type: %20s\tID: %20s\tSize: %3d\tOffset: %d\n",type,temp->id_name,temp->size,temp->offset);
		
		temp=temp->next;
	}
	
	/*
	printf("\nType Definitions:\n");
	
	temp=s->rec;
	
	while(temp!=NULL)
	{
		char type[101];
		if(temp->type==0)
		{
			strcpy(type,"integer");
		}
		else if(temp->type==1)
		{
			strcpy(type,"real");
		}
		else
		{
			strcpy(type,temp->rname);
		}
	
		if(temp->rec==0)
		{
			printf("Type: %20s\tID: %20s\tSize: %3d\tOffset: %d\n",type,temp->id_name,temp->size,temp->offset);
		}
		else
		{
			printf("Record: %20s\tType: %20s\tID: %20s\tSize: %3d\tOffset: %d\n",temp->rname,type,temp->id_name,temp->size,temp->offset);
		}
	
		temp=temp->next;
	}
	*/
	
	//printf("\nNo of functions (including main): %d\n",s->fno);
	
	int i=0;
	
	while(i<(s->fno))
	{
		//printf("\nFunction %s:\n",s->flist[i]->fname);
		
		//printf("Input Parameters:\n");
		temp=s->flist[i]->in_list;
		
		//printf("AAA\n");
		
		while(temp!=NULL)
		{
			char type[101];
			if(temp->type==0)
			{
				strcpy(type,"int");
			}
			else if(temp->type==1)
			{
				strcpy(type,"real");
			}
			else
			{
				strcpy(type,type_rec(s,temp->rname));
			}
			
			//printf("AAA\n");
		
			printf("%30s\t%30s\t%30s\t%10d\n",temp->id_name,s->flist[i]->fname,type,temp->offset);
			//printf("Type: %20s\tID: %20s\tSize: %3d\tOffset: %d\n",type,temp->id_name,temp->size,temp->offset);
		
			temp=temp->next;
		}
		
		//printf("Output Parameters:\n");
		temp=s->flist[i]->out_list;
		
		while(temp!=NULL)
		{
			char type[101];
			if(temp->type==0)
			{
				strcpy(type,"int");
			}
			else if(temp->type==1)
			{
				strcpy(type,"real");
			}
			else
			{
				strcpy(type,type_rec(s,temp->rname));
			}
		
			printf("%30s\t%30s\t%30s\t%10d\n",temp->id_name,s->flist[i]->fname,type,temp->offset);
			//printf("Type: %20s\tID: %20s\tSize: %3d\tOffset: %d\n",type,temp->id_name,temp->size,temp->offset);
		
			temp=temp->next;
		}
		
		//printf("Explicit Declarations:\n");
		temp=s->flist[i]->idlist;
		
		while(temp!=NULL)
		{
			char type[101];
			if(temp->type==0)
			{
				strcpy(type,"int");
			}
			else if(temp->type==1)
			{
				strcpy(type,"real");
			}
			else
			{
				strcpy(type,type_rec(s,temp->rname));
			}
		
			if(temp->rec==0)
			{
				printf("%30s\t%30s\t%30s\t%10d\n",temp->id_name,s->flist[i]->fname,type,temp->offset);
				//printf("Type: %20s\tID: %20s\tSize: %3d\tOffset: %d\n",type,temp->id_name,temp->size,temp->offset);
			}
			else
			{
				//printf("Record: %20s\tType: %20s\tID: %20s\tSize: %3d\tOffset: %d\n",temp->rname,type,temp->id_name,temp->size,temp->offset);
			}
		
			temp=temp->next;
		}
		
		//printf("d5cb34567 present: %d\n",id_present(s->flist[i]->idlist,"d5cb34567"));
		//printf("b5c6 present: %d\n",id_present(s->flist[i]->idlist,"b5c6"));
		//printf("maths in #marks present: %d\n",field_present(s->rec,"#marks","maths"));
		//printf("#marks present: %d\n",rectype_present(s->flist[i]->idlist,"#marks"));
		
		i++;
	}
}

int id_present(id head,char *iden)
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
				res++;
			}
		}
		
		temp=temp->next;
	}
	
	return res;
}

int rectype_present(id head,char *iden)
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
				res++;
			}
		}
		
		temp=temp->next;
	}
	
	return res;
}

int field_present(id head,char *recid,char *fid)
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
				res++;
			}
		}
		
		temp=temp->next;
	}
	
	return res;
}

void offset_init(symt s)
{
	//Size initialization of type definitions
	id temp;
	temp=s->rec;
	
	while(temp!=NULL)
	{
		temp->size=0;
		temp->offset=-1;
		
		id temp1;
		temp1=temp->next; //first field of the record
		
		while(temp1!=NULL && temp1->rec==1)
		{
			temp1->size=((temp1->type)+1)*2;
			temp1->offset=-1;
			
			temp->size=(temp->size)+(temp1->size);
			
			temp1=temp1->next;
		}
		
		temp=temp1;
	}
	
	temp=s->glist; //global variables
	
	while(temp!=NULL)
	{
		if(temp->type==2) //record type
		{
			temp->size=size_rec(s,temp->rname);
		}
		else //int or real
		{
			temp->size=((temp->type)+1)*2;
		}
		temp->offset=-1;
		
		temp=temp->next;
	}
	
	int i=0;
	while(i<(s->fno)) //All functions
	{
		int offset=0;
		
		temp=s->flist[i]->in_list; //input parameters
	
		while(temp!=NULL)
		{
			if(temp->type==2) //record type
			{
				temp->size=size_rec(s,temp->rname);
			}
			else //int or real
			{
				temp->size=((temp->type)+1)*2;
			}
			temp->offset=offset;
			offset+=(temp->size);
		
			temp=temp->next;
		}
		
		temp=s->flist[i]->out_list; //output parameters
	
		while(temp!=NULL)
		{
			if(temp->type==2) //record type
			{
				temp->size=size_rec(s,temp->rname);
			}
			else //int or real
			{
				temp->size=((temp->type)+1)*2;
			}
			temp->offset=offset;
			offset+=(temp->size);
		
			temp=temp->next;
		}
		
		temp=s->flist[i]->idlist; //declarations
	
		while(temp!=NULL)
		{
			if(temp->type==2) //record type
			{
				temp->size=size_rec(s,temp->rname);
			}
			else //int or real
			{
				temp->size=((temp->type)+1)*2;
			}
			temp->offset=offset;
			offset+=(temp->size);
		
			temp=temp->next;
		}
		
		i++;
	}
}

int size_rec(symt s,char *rname)
{
	int res=0,flag=0;
	
	id temp;
	temp=s->rec;
	
	while(temp!=NULL && flag==0)
	{
		if(strcmp(temp->rname,rname)==0 && temp->rec==0)
		{
			res=temp->size;
			flag++;
		}
		temp=temp->next;
	}
	
	return res;
}

char *type_rec(symt s,char *rname)
{
	char *res;
	res=(char *)malloc(100*sizeof(char));
	
	res[0]='\0';
	
	id temp;
	temp=s->rec;
	int flag=0;
	
	while(flag==0 && temp!=NULL)
	{
		if(strcmp(temp->rname,rname)==0)
		{
			flag++;
			id temp1;
			temp1=temp->next;
			
			while(temp1!=NULL && temp1->rec!=0)
			{
				if(temp1->type==0)
				{
					strcat(res,"int");
					strcat(res,"x");
				}
				else
				{
					strcat(res,"real");
					strcat(res,"x");
				}
				temp1=temp1->next;
			}
			
			int x;
			x=strlen(res);
			res[x-1]='\0';
			
			temp=temp1;
		}
		if(temp!=NULL)
		{
			temp=temp->next;
		}
	}
	
	return res;
}
