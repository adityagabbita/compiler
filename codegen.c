//Batch No						38
//Krishna Aditya Gabbita		2009B3A7608P
//Varun Sah						2010A7PS078P

#include "codegen.h"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void asm_code(symt s,abstractSyntaxTree A,FILE *fp)
{
	int scope=0;
	
	abstractSyntaxTree temp;
	temp=A->child[0];
	
	//fprintf(fp,"\nASM code:\n");
	fprintf(fp,".model small\n");
	fprintf(fp,".stack 1024\n");
	//fprintf(fp,".data\n");
	
	while(temp->e!=53)
	{
		travel_scope(s,temp->child[0]->child[2],scope,fp);
		temp=temp->child[1];
		scope++;
	}
	/*
	id temp1;
	temp1=s->glist;
	while(temp1!=NULL)
	{
		fprintf(fp,"\t%s dw ?\n",temp1->id_name);
		temp1=temp1->next;
	}
	
	temp1=s->flist[scope]->idlist;
	while(temp1!=NULL)
	{
		fprintf(fp,"\t%s dw ?\n",temp1->id_name);
		temp1=temp1->next;
	}
	*/
	fprintf(fp,".code\n");
	fprintf(fp,"START:\n");
	
	//fprintf(fp,"\tmov AX, @data\n");
	//fprintf(fp,"\tmov DS, AX\n\n");
	
	fprintf(fp,"\tmov BX, 512\n\n");
	
	travel_scope(s,A->child[1]->child[0],scope,fp);
	
	fprintf(fp,"\n\tmov AX, 4C00h\n");
	fprintf(fp,"\tint 21h\n");
	
	fprintf(fp,"end START\n");
	fprintf(fp,".end\n");
}

void travel_scope(symt s,abstractSyntaxTree A,int scope,FILE *fp)
{
	if(A->e==25) //TK_ASSIGNOP
	{
		arith_eval(s,A->child[1],scope,fp);
		
		fprintf(fp,"\tsub BX, 2\n");
		fprintf(fp,"\tmov AX, [BX]\n"); 
		fprintf(fp,"\tmov DS:%d, AX\n",var_offset(s,scope,A->child[0]->lex));
		//fprintf(fp,"\tmov %s, [BX]\n\n",A->child[0]->lex);
		
		//fprintf(fp,"mov [BP+%d],[SP]\n",var_offset(s,scope,A->child[0]->lex));
		//fprintf(fp,"pop AX\n\n");
	}
	
	else if(A->e==44) //TK_READ
	{
		if(A->child[0]->e==18 && A->child[0]->child[0]->e==53) //TK_ID
		{
			fprintf(fp,"\n\tmov AH, 1h\n");
			fprintf(fp,"\tint 21h\n");
			fprintf(fp,"\tmov CH, AL\n");
			fprintf(fp,"\tmov AH, 1h\n");
			fprintf(fp,"\tint 21h\n");
			fprintf(fp,"\tmov CL, AL\n");
			fprintf(fp,"\tmov DS:%d, CX\n\n",var_offset(s,scope,A->child[0]->lex));
			//fprintf(fp,"read [BP+%d]\n\n",var_offset(s,scope,A->child[0]->lex));
		}
		if(A->child[0]->e==18 && A->child[0]->child[0]->e==8) //TK_ID.TK_FIELDID
		{
			fprintf(fp,"\n\tmov AH, 1h\n");
			fprintf(fp,"\tint 21h\n");
			fprintf(fp,"\tmov CH, AL\n");
			fprintf(fp,"\tmov AH, 1h\n");
			fprintf(fp,"\tint 21h\n");
			fprintf(fp,"\tmov CL, AL\n");
			int x;
			x=field_offset(s,rec_type(s,A->child[0]->lex,scope),A->child[0]->child[0]->child[0]->lex);
			fprintf(fp,"\tmov DS:%d, CX\n\n",var_offset(s,scope,A->child[0]->lex)+x);
			//fprintf(fp,"read [BP+%d]\n\n",var_offset(s,scope,A->child[0]->lex)+field_offset(s,rec_type(s,A->child[0]->lex,scope),A->child[0]->child[0]->child[0]->lex));
		}
	}
	
	else if(A->e==52) //TK_WRITE
	{
		if(A->child[0]->e==18) //TK_ID
		{
			fprintf(fp,"\n\tmov CX, DS:%d\n",var_offset(s,scope,A->child[0]->lex));
			fprintf(fp,"\tmov DL, CH\n");
			fprintf(fp,"\tmov AH, 2h\n");
			fprintf(fp,"\tint 21h\n");
			fprintf(fp,"\tmov DL, CL\n");
			fprintf(fp,"\tmov AH, 2h\n");
			fprintf(fp,"\tint 21h\n\n");
			//fprintf(fp,"write [BP+%d]\n\n",var_offset(s,scope,A->child[0]->lex));
		}
		if(A->child[0]->e==15 || A->child[0]->e==16) //TK_NUM or TK_RNUM
		{
			fprintf(fp,"\n\tmov CX, %s\n",A->child[0]->lex);
			fprintf(fp,"\tmov DL, CH\n");
			fprintf(fp,"\tmov AH, 2h\n");
			fprintf(fp,"\tint 21h\n");
			fprintf(fp,"\tmov DL, CL\n");
			fprintf(fp,"\tmov AH, 2h\n");
			fprintf(fp,"\tint 21h\n\n");
			//fprintf(fp,"write %s\n\n",A->child[0]->lex);
		}
	}
	
	else if(A->e==37) //TK_IF
	{
		cond_eval(s,A,scope,fp);
	}
	
	else if(A->e==50) //TK_WHILE
	{
		loop_eval(s,A,scope,fp);
	}
	
	else
	{
		int i=0;
		while(i<(A->childnum))
		{
			travel_scope(s,A->child[i],scope,fp);
			i++;
		}
	}
}

void arith_eval(symt s,abstractSyntaxTree A,int scope,FILE *fp)
{
	int i=0;
	while(i<(A->childnum))
	{
		arith_eval(s,A->child[i],scope,fp);
		i++;
	}
	
	if(A->e==18) //TK_ID
	{
		fprintf(fp,"\tmov AX, DS:%d\n",var_offset(s,scope,A->lex));
		fprintf(fp,"\tmov [BX], AX\n");
		//fprintf(fp,"\tmov [BX], %s\n",A->lex);
		fprintf(fp,"\tadd BX, %d\n",var_size(s,A->lex,scope)); 
	}
	
	if(A->e==15 || A->e==16) //TK_NUM or TK_RNUM
	{
		fprintf(fp,"\tmov [BX], %s\n",A->lex);
		if(A->e==15)
		{
			fprintf(fp,"\tadd BX, 2\n");
		}
		if(A->e==16)
		{
			fprintf(fp,"\tadd BX, 4\n");
		}
		//fprintf(fp,"push %s\n",A->lex);
	}
	
	if(A->e==23) //TK_PLUS
	{
		fprintf(fp,"\tsub BX, 2\n");
		fprintf(fp,"\tmov AX, [BX]\n");
		fprintf(fp,"\tadd [BX-2], AX\n");
		//fprintf(fp,"pop AX\n");
		//fprintf(fp,"add [SP], AX\n");
	}
	
	if(A->e==0) //TK_MINUS
	{
		fprintf(fp,"\tsub BX, 2\n");
		fprintf(fp,"\tmov AX, [BX]\n");
		fprintf(fp,"\tsub [BX-2], AX\n");
		//fprintf(fp,"pop AX\n");
		//fprintf(fp,"sub [SP], AX\n");
	}
	
	if(A->e==7) //TK_MUL
	{
		fprintf(fp,"\tsub BX, 2\n");
		fprintf(fp,"\tmov AX, [BX]\n");
		fprintf(fp,"\tmov CX, [BX-2]\n");
		fprintf(fp,"\tmul CX\n");
		//fprintf(fp,"\tmul [BX-2]\n");
		fprintf(fp,"\tmov [BX-2], AX\n");
		//fprintf(fp,"pop AX\n");
		//fprintf(fp,"mul [SP]\n");
		//fprintf(fp,"mov [SP], AX\n");
	}
	
	if(A->e==9) //TK_DIV
	{
		fprintf(fp,"\tsub BX, 2\n");
		fprintf(fp,"\tmov CX, [BX]\n");
		fprintf(fp,"\tsub BX, 2\n");
		fprintf(fp,"\tmov AX, [BX]\n");
		fprintf(fp,"\tdiv CX\n");
		fprintf(fp,"\tmov [BX], AX\n");
		fprintf(fp,"\tadd BX, 2\n");
		//fprintf(fp,"pop BX\n");
		//fprintf(fp,"pop AX\n");
		//fprintf(fp,"div BX\n");
		//fprintf(fp,"push AX\n");
	}
}

int var_offset(symt s,int scope,char *iden)
{
	int res=0,flag=0;
	
	id temp;
	temp=s->flist[scope]->in_list; //input variables
	
	while(temp!=NULL && flag==0)
	{
		if(temp->rec==0)
		{
			if(strcmp(temp->id_name,iden)==0)
			{
				res=temp->offset;
				flag++;
			}
		}
		
		temp=temp->next;
	}
	
	temp=s->flist[scope]->out_list; //output variables
	
	while(temp!=NULL && flag==0)
	{
		if(temp->rec==0)
		{
			if(strcmp(temp->id_name,iden)==0)
			{
				res=temp->offset;
				flag++;
			}
		}
		
		temp=temp->next;
	}
	
	temp=s->flist[scope]->idlist; //declarations
	
	while(temp!=NULL && flag==0)
	{
		if(temp->rec==0)
		{
			if(strcmp(temp->id_name,iden)==0)
			{
				res=temp->offset;
				flag++;
			}
		}
		
		temp=temp->next;
	}
	
	return res;
}

int field_offset(symt s,char *rname,char *fdname)
{
	int res=0,flag=0;
	
	id temp;
	temp=s->rec; //type definitions
	
	while(temp!=NULL && flag==0)
	{
		if(temp->rec==0)
		{
			if(strcmp(temp->rname,rname)==0)
			{
				temp=temp->next;
				
				while(temp!=NULL && flag==0)
				{
					if(strcmp(temp->id_name,fdname)==0 && strcmp(temp->rname,rname)==0)
					{
						flag++;
						return res;
					}
					res+=temp->size;
					temp=temp->next;
				}
			}
		}
		
		if(temp!=NULL)
		{
			temp=temp->next;
		}
	}
	
	return res;
}

char *rec_type(symt s,char *idname,int scope)
{
	char *res;
	res=(char *)malloc(100*sizeof(char));
	strcpy(res,"---");
	
	int flag=0;
	
	id temp;
	temp=s->flist[scope]->in_list; //input variables
	
	while(temp!=NULL && flag==0)
	{
		if(temp->rec==0)
		{
			if(strcmp(temp->id_name,idname)==0)
			{
				strcpy(res,temp->rname);
				flag++;
			}
		}
		
		temp=temp->next;
	}
	
	temp=s->flist[scope]->out_list; //output variables
	
	while(temp!=NULL && flag==0)
	{
		if(temp->rec==0)
		{
			if(strcmp(temp->id_name,idname)==0)
			{
				strcpy(res,temp->rname);
				flag++;
			}
		}
		
		temp=temp->next;
	}
	
	temp=s->flist[scope]->idlist; //declarations
	
	while(temp!=NULL && flag==0)
	{
		if(temp->rec==0)
		{
			if(strcmp(temp->id_name,idname)==0)
			{
				strcpy(res,temp->rname);
				flag++;
			}
		}
		
		temp=temp->next;
	}
	
	return res;
}

void bool_eval(symt s,abstractSyntaxTree A,int scope,FILE *fp)
{
	int i=0;
	while(i<(A->childnum))
	{
		bool_eval(s,A->child[i],scope,fp);
		i++;
	}
	
	if(A->e==18) //TK_ID
	{
		fprintf(fp,"\tmov AX, DS:%d\n",var_offset(s,scope,A->lex));
		fprintf(fp,"\tmov [BX], AX\n");
		//fprintf(fp,"\tmov [BX], %s\n",A->lex);
		fprintf(fp,"\tadd BX, %d\n",var_size(s,A->lex,scope));
		//fprintf(fp,"push [BP+%d]\n",var_offset(s,scope,A->lex));
	}
	if(A->e==15 || A->e==16) //TK_NUM or TK_RNUM
	{
		fprintf(fp,"\tmov [BX], %s\n",A->lex);
		if(A->e==15)
		{
			fprintf(fp,"\tadd BX, 2\n");
		}
		if(A->e==16)
		{
			fprintf(fp,"\tadd BX, 4\n");
		}
		
		//fprintf(fp,"push %s\n",A->lex);
	}
	if(A->e==24) //TK_LT
	{
		fprintf(fp,"\tsub BX, 2\n");
		fprintf(fp,"\tmov AX, [BX]\n");
		fprintf(fp,"\tmov CX, [BX-2]\n");
		fprintf(fp,"\t.if CX < AX\n");
		fprintf(fp,"\tmov [BX-2], 1\n");
		fprintf(fp,"\t.else\n");
		fprintf(fp,"\tmov [BX-2], 0\n");
		fprintf(fp,"\t.endif\n\n"); 
		//fprintf(fp,"pop AX\n");
		//fprintf(fp,"cmp [SP], AX\n");
		//fprintf(fp,"setl [SP]\n");
	}
	if(A->e==26) //TK_LE
	{
		fprintf(fp,"\tsub BX, 2\n");
		fprintf(fp,"\tmov AX, [BX]\n");
		fprintf(fp,"\tmov CX, [BX-2]\n");
		fprintf(fp,"\t.if CX <= AX\n");
		fprintf(fp,"\tmov [BX-2], 1\n");
		fprintf(fp,"\t.else\n");
		fprintf(fp,"\tmov [BX-2], 0\n");
		fprintf(fp,"\t.endif\n\n");
		//fprintf(fp,"pop AX\n");
		//fprintf(fp,"cmp [SP], AX\n");
		//fprintf(fp,"setle [SP]\n");
	}
	if(A->e==27) //TK_EQ
	{
		fprintf(fp,"\tsub BX, 2\n");
		fprintf(fp,"\tmov AX, [BX]\n");
		fprintf(fp,"\tmov CX, [BX-2]\n");
		fprintf(fp,"\t.if CX == AX\n");
		fprintf(fp,"\tmov [BX-2], 1\n");
		fprintf(fp,"\t.else\n");
		fprintf(fp,"\tmov [BX-2], 0\n");
		fprintf(fp,"\t.endif\n\n");
		//fprintf(fp,"pop AX\n");
		//fprintf(fp,"cmp [SP], AX\n");
		//fprintf(fp,"sete [SP]\n");
	}
	if(A->e==28) //TK_GT
	{
		fprintf(fp,"\tsub BX, 2\n");
		fprintf(fp,"\tmov AX, [BX]\n");
		fprintf(fp,"\tmov CX, [BX-2]\n");
		fprintf(fp,"\t.if CX > AX\n");
		fprintf(fp,"\tmov [BX-2], 1\n");
		fprintf(fp,"\t.else\n");
		fprintf(fp,"\tmov [BX-2], 0\n");
		fprintf(fp,"\t.endif\n\n");
		//fprintf(fp,"pop AX\n");
		//fprintf(fp,"cmp [SP], AX\n");
		//fprintf(fp,"setg [SP]\n");
	}
	if(A->e==29) //TK_GE
	{
		fprintf(fp,"\tsub BX, 2\n");
		fprintf(fp,"\tmov AX, [BX]\n");
		fprintf(fp,"\tmov CX, [BX-2]\n");
		fprintf(fp,"\t.if CX >= AX\n");
		fprintf(fp,"\tmov [BX-2], 1\n");
		fprintf(fp,"\t.else\n");
		fprintf(fp,"\tmov [BX-2], 0\n");
		fprintf(fp,"\t.endif\n\n");
		//fprintf(fp,"pop AX\n");
		//fprintf(fp,"cmp [SP], AX\n");
		//fprintf(fp,"setge [SP]\n");
	}
	if(A->e==1) //TK_NE
	{
		fprintf(fp,"\tsub BX, 2\n");
		fprintf(fp,"\tmov AX, [BX]\n");
		fprintf(fp,"\tmov CX, [BX-2]\n");
		fprintf(fp,"\t.if CX != AX\n");
		fprintf(fp,"\tmov [BX-2], 1\n");
		fprintf(fp,"\t.else\n");
		fprintf(fp,"\tmov [BX-2], 0\n");
		fprintf(fp,"\t.endif\n\n");
		//fprintf(fp,"pop AX\n");
		//fprintf(fp,"cmp [SP], AX\n");
		//fprintf(fp,"setne [SP]\n");
	}
	if(A->e==22) //TK_NOT
	{
		fprintf(fp,"\tmov AX, [BX-2]\n");
		fprintf(fp,"\t.if AX == 1\n");
		fprintf(fp,"\tmov [BX-2], 0\n");
		fprintf(fp,"\t.else\n");
		fprintf(fp,"\tmov [BX-2], 1\n");
		fprintf(fp,"\t.endif\n\n");
		//fprintf(fp,"mov AX, [SP]\n");
		//fprintf(fp,"mov [SP], 1\n");
		//fprintf(fp,"sub [SP], AX\n"); 
	}
	if(A->e==4) //TK_AND
	{
		fprintf(fp,"\tsub BX, 2\n");
		fprintf(fp,"\tmov AX, [BX]\n");
		fprintf(fp,"\tmov CX, [BX-2]\n");
		fprintf(fp,"\tadd AX, CX\n");
		fprintf(fp,"\t.if AX > 1\n");
		fprintf(fp,"\tmov [BX-2], 1\n");
		fprintf(fp,"\t.else\n");
		fprintf(fp,"\tmov [BX-2], 0\n");
		fprintf(fp,"\t.endif\n\n");
		//fprintf(fp,"pop AX\n");
		//fprintf(fp,"add AX, [SP]\n");
		//fprintf(fp,"cmp AX, 1\n");
		//fprintf(fp,"setg [SP]\n");
	}
	if(A->e==13) //TK_OR
	{
		fprintf(fp,"\tsub BX, 2\n");
		fprintf(fp,"\tmov AX, [BX]\n");
		fprintf(fp,"\tmov CX, [BX-2]\n");
		fprintf(fp,"\tadd AX, CX\n");
		fprintf(fp,"\t.if AX > 0\n");
		fprintf(fp,"\tmov [BX-2], 1\n");
		fprintf(fp,"\t.else\n");
		fprintf(fp,"\tmov [BX-2], 0\n");
		fprintf(fp,"\t.endif\n\n");
		//fprintf(fp,"pop AX\n");
		//fprintf(fp,"add AX, [SP]\n");
		//fprintf(fp,"cmp AX, 0\n");
		//fprintf(fp,"setg [SP]\n");
	}
}

void cond_eval(symt s,abstractSyntaxTree A,int scope,FILE *fp)
{
	fprintf(fp,"\n");
	bool_eval(s,A->child[0],scope,fp);
	
	fprintf(fp,"\tsub BX, 2\n");
	fprintf(fp,"\tmov AX, [BX]\n");
	fprintf(fp,"\t.if AX == 1\n");
	travel_scope(s,A->child[1],scope,fp);
	if(A->child[2]!=NULL)
	{
		fprintf(fp,"\t.else\n");
		travel_scope(s,A->child[2],scope,fp);
	}
	fprintf(fp,"\t.endif\n\n");
	
	/*
	fprintf(fp,"pop AX\n"); 
	fprintf(fp,"cmp AX, 1\n");
	fprintf(fp,"jne [label%dFalse]\n\n",jump);
	travel_scope(s,A->child[1],scope); //TURE
	fprintf(fp,"jmp [label%dEnd]\n\n",jump); //Don't evaluate false
	fprintf(fp,"label%dFalse:",jump); //FALSE
	
	if(A->child[2]!=NULL)
	{
		travel_scope(s,A->child[2],scope);
	}
	fprintf(fp,"\nlabel%dEnd:\n",jump); //END of IF scope
	jump++;
	*/
}

void loop_eval(symt s,abstractSyntaxTree A,int scope,FILE *fp)
{
	fprintf(fp,"\n");
	fprintf(fp,"\tLOOP%d:\n",jump);
	int a=jump;
	jump++;
	
	bool_eval(s,A->child[0],scope,fp);
	
	fprintf(fp,"\tsub BX, 2\n");
	fprintf(fp,"\tmov AX, [BX]\n");
	fprintf(fp,"\t.if AX == 0\n");
	fprintf(fp,"\tjmp ENDLOOP%d\n",a);
	
	fprintf(fp,"\t.else\n");
	travel_scope(s,A->child[1],scope,fp);
	travel_scope(s,A->child[2],scope,fp);
	fprintf(fp,"\t.endif\n");
	
	fprintf(fp,"\tjmp LOOP%d\n",a);
	fprintf(fp,"\n\tENDLOOP%d:\n",a);
}

int var_size(symt s,char *idname,int scope)
{
	int res=0;
	
	id temp;
	temp=s->glist;
	
	int flag=0;
	
	while(flag==0 && temp!=NULL)
	{
		if(strcmp(temp->id_name,idname)==0)
		{
			res=temp->size;
			flag++;
		}
		temp=temp->next;
	}
	
	temp=s->flist[scope]->idlist;
	
	flag=0;
	
	while(flag==0 && temp!=NULL)
	{
		if(strcmp(temp->id_name,idname)==0)
		{
			res=temp->size;
			flag++;
		}
		temp=temp->next;
	}
	
	return res;
}
