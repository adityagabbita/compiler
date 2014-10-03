//Batch No						38
//Krishna Aditya Gabbita	2009B3A7608P
//Varun Sah						2010A7PS078P

#include "lexer.h"
#include "parser.h"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

stack initStack()
{
	stack s;
	s= NULL;
	return s;	

}
stack push(stack s,int e)
{
	stack temp;
	temp= (stack) malloc(sizeof(struct _stack));
	temp->e=e;
	temp->next=s;
	return temp;
}

int top(stack s)
{
	return (s->e);
}

stack pop(stack s)
{
	stack temp;
	temp= s->next;
	free(s);
	return temp;
}

int isEmpty (stack s)
{
	if(s==NULL)
	{
		return 1;
	}
	else return 0; 
}


void createParseTable(grammar G, table T)
{
	int i,j;
	
	for(i=0;i<ndet;i++)
	{
		for(j=0;j<55;j++)
		{
			T[i][j]=0;
		}
	}
	
	for(i=0;i<ndet;i++)
	{
		rulenode temp;
		temp=G[i].head;
		
		while(temp!=NULL)
		{
			llist tlist;
			tlist=temp->first;
			
			while(tlist!=NULL)
			{
				T[i][(tlist->e)]=temp->rno;
				tlist=tlist->next;
			}
			
			if((temp->eflag)!=0)
			{				
				for(j=0;j<(G[i].followlen);j++)
				{
					T[i][(G[i].follow[j])]=temp->rno;
				}
			}
			
			temp=temp->next;
		}
	}
}

parseTree parseInputSourceCode(FILE *fp, table T,grammar G)
{
	parseTree pt;
	pt=(parseTree)malloc(sizeof(struct _parseTree));
	pt->e=100;
	strcpy(pt->lex,"---");
	pt->linenum=1;
	pt->parent=NULL;
	//pt->rno=1001; 
	
	//printf("Here\n");
	
	int lexerr=0;
	
	tokenInfo *ti;
	
	int i,len;
	char B[31];
	len=30;
	
	//lx[31];    //lexeme
   
   //int *state,*j,*lnum;
   state=(int *)malloc(sizeof(int));
   j=(int *)malloc(sizeof(int));
   lnum=(int *)malloc(sizeof(int));	//current line number
   
   *state=0;*j=0;*lnum=1;
   
   tlist t;
   t=initList();
   
   while(len==30)
   {
		fp=getStream(fp,B,30);
      len=strlen(B);

      if(len<30)
      {
    	  B[len]=(char)32; //space
        B[len+1]='\0';
      }

      //printf("%s\n\n",B);

      //tokenize(B,len,state1,j1,*state1,*j1,st,asc,tk,lx);
	
	   while(B[len-1]!='^')
		{
			ti=getNextToken(B,len);
		 	if((ti->tkclass!=-1) && (ti->tkclass!=-2) && (ti->tkclass!=-3) && strcmp(ti->tok,"A")!=0)
		 	{
		 		tokenInfo temp;
		 		strcpy(temp.lex,ti->lex);
		 		strcpy(temp.tok,ti->tok);
		 		temp.lnum=ti->lnum;
		 		temp.tkclass=ti->tkclass;
		 		
		 		t=add(t,temp);
		 		//printf("%s\t%s\t%d\n",ti->lex,ti->tok,ti->lnum);
			}
			
			if(ti->tkclass==-1)
			{
				//printf("Identifier %s at line %d is longer than the prescribed length of 20 characters\n",ti->lex,ti->lnum);
				lexerr++;
			}
			
			if(ti->tkclass==-2)
			{
				char sym;
				sym=ti->lex[0];
				
				if(sym!=' ' && sym!='\t' && sym!='\n' && sym!=EOF)
				{			
					//printf("Unknown Symbol '%c' at line %d\n",ti->lex[0],ti->lnum);
					lexerr++;
				}
			}
			
			if(ti->tkclass==-3)
			{
				//printf("Unknown pattern %s at line %d\n",ti->lex,ti->lnum);
				lexerr++;
			}
		}
		
	}
   
   if(lexerr>0)
   {
   	printf("Lexical errors present in program! Can't create a parse tree.\n");
   	return NULL;
   }
   
   //print_tlist(t);
   //printf("\n\n\n");
   
   stack s;
	s=initStack();
	
	s=push(s,54); //TK_DOLLAR
	s=push(s,100); //<program>
	s->pt=pt;
	
	/*
	stack temp_stack;
	temp_stack=s;
	
	while(temp_stack!=NULL)
	{
		printf("%s\n",td[temp_stack->e]);
		temp_stack=temp_stack->next;
	}
	*/
	
	tlist curr_lex;
	curr_lex=t;
	
	//printf("abc\n");

	while(top(s)!=54 && curr_lex!=NULL)
	{
		if(top(s)==(curr_lex->ti.tkclass))
		{
			strcpy(s->pt->lex,curr_lex->ti.lex);
			s->pt->linenum=curr_lex->ti.lnum;
			s=pop(s);
			curr_lex=curr_lex->next;
		}
		else if(top(s)>=0 && top(s)<55)
		{
			if(top(s)!=53)
			{
				printf("ERROR_5: The token %s for lexeme %s does not match at line %d. The expected token here is %s\n",curr_lex->ti.tok,curr_lex->ti.lex,curr_lex->ti.lnum,td[top(s)]);
				parse_err++;
				break;
			}
			if(top(s)==53)
			{
				strcpy(s->pt->lex,"EPS");
				s=pop(s);
			}
		}
		else if(T[(top(s)-100)][curr_lex->ti.tkclass]==0)
		{
			printf("Error_6: No rule found for non-terminal %s at line %d\n",td[top(s)],/*curr_lex->ti.lnum*/s->pt->linenum);
			//printf("Determinant/Non-Terminal: %s\tToken: %s\tLexeme:%s\n",td[top(s)],curr_lex->ti.tok,curr_lex->ti.lex);
			printf("Lexeme %s present instead of\n",curr_lex->ti.lex);
			printf("Possible tokens expected:");
			int poss;
			
			for(poss=0;poss<53;poss++)
			{
				if(T[top(s)-100][poss]!=0)
				{
					printf("%s\t",td[poss]);
				}
			}
			
			printf("\n");
			
			parse_err++;
			break;
		}
		else if(T[(top(s)-100)][curr_lex->ti.tkclass]!=0)
		{
			//printf("abc\n");
			int rno=T[(top(s)-100)][curr_lex->ti.tkclass];
			int det=rno/10;
			
			//printf("rno: %d\tdet: %d\n",rno,det);
			
			rulenode rule;
			rule=G[det-100].head;
			
			//printf("abc\n");
			
			int j1;
			
			j1=(rno%10)-1;
			
			//printf("j1: %d\n",j1);
			
			while(j1>0)
			{
				rule=rule->next;
				j1--;
			}
			
			int rlen=rule->len;
			//printf("rlen: %d\n",rlen);
			
			//printf("%s\n",td[100]);
			//printf("%s==> ",td[(det)]);
			
			int *rulestr;
			rulestr=(int *)malloc(rlen*sizeof(int));
			
			//printf("abc\n");
			
			for(i=0;i<rlen;i++)
			{
				rulestr[i]=rule->head[i];
				//printf("%s ",td[rulestr[i]]);
			}
			//printf("\n");
			
			//printf("abc\n");
			
			parseTree temp;
			temp=s->pt;
			
			temp->childnum=rlen;
			temp->child=(parseTree *)malloc(rlen*sizeof(parseTree));
			temp->rno=rno;
			/*
			for(i=0;i<rlen;i++)
			{
				temp->child[i]=(parseTree)malloc(sizeof(struct _parseTree));
			}
			*/
			
			s=pop(s);
			
			//printf("abc\n");
			
			for(i=(rlen-1);i>=0;i--)
			{
				parseTree new;
				new=(parseTree)malloc(sizeof(struct _parseTree));
				
				new->e=rulestr[i];
				new->childnum=0;
				//new->rno=rno;
				temp->child[i]=new;
				
				new->parent=temp;
				strcpy(new->lex,"---");
				
				new->linenum=curr_lex->ti.lnum;
				
				s=push(s,rulestr[i]);
				
				s->pt=new;
			}
		}
	}
	
	if(top(s)==54 && curr_lex==NULL)
	{
		printf("Input source code is syntactically correct!\n");
	}
	else if(top(s)==54 && curr_lex!=NULL)
	{
		printf("Program has unparsed lexemes remaining!\n");
		printf("Lexeme %s not parsed at line %d\n",curr_lex->ti.lex,curr_lex->ti.lnum);
		parse_err++;
	}
	else if(top(s)!=54 && curr_lex==NULL)
	{
		printf("Program ended abruptly!\n");
		printf("Determinant/Token %s not completed at line %d\n",td[top(s)],s->pt->linenum);
		parse_err++;
	}
	
	return pt;
}

void printParseTree(parseTree pt,char *outfile,int *totalAllocatedMemory)
{	
	*totalAllocatedMemory+=sizeof(*pt);
	
	FILE *fp;
	
	if((pt->e)==100)
	{
		fp=fopen(outfile,"w");
	}
	else
	{
		fp=fopen(outfile,"a");
	}
	
	//printf("ABC\n");
	
	int i=0;
	
	char l[31];
	if(pt->e==15 || pt->e==16)
	{
		strcpy(l,pt->lex);
	}
	else
	{
		strcpy(l,"N/A");
	}
	
	char yn[4];
	if(pt->childnum==0)
	{
		strcpy(yn,"yes");
	}
	else
	{
		strcpy(yn,"no");
	}
	
	if(pt->parent!=NULL)
	{
		int e,e1;
		e=pt->e;
		
		parseTree par;
		par=pt->parent;
		e1=par->e;
		
		fprintf(fp,"%20s %3d %30s %10s %30s %3s %30s\n",pt->lex,pt->linenum,td[e],l,td[e1],yn,td[e]);
	}
	else
	{
		char p[4];
		strcpy(p,"N/A");
		
		fprintf(fp,"%20s %3d %30s %10s %30s %3s %30s\n",pt->lex,pt->linenum,td[pt->e],l,p,yn,td[pt->e]);
	}
	
	fclose(fp);
	
	while(i<(pt->childnum))
	{
		printParseTree(pt->child[i],outfile,totalAllocatedMemory);
		i++;
	}
}

void printAST(abstractSyntaxTree A,char *outFile,int *totalAllocatedMemory)
{
	*totalAllocatedMemory+=sizeof(*A);
	
	FILE *fp;
	
	if((A->e)==100)
	{
		fp=fopen(outFile,"w");
	}
	else
	{
		fp=fopen(outFile,"a");
	}
	
	//printf("ABC\n");
	
	int i=0;
	
	char l[31];
	if(A->e==15 || A->e==16)
	{
		strcpy(l,A->lex);
	}
	else
	{
		strcpy(l,"N/A");
	}
	
	char yn[4];
	if(A->childnum==0)
	{
		strcpy(yn,"yes");
	}
	else
	{
		strcpy(yn,"no");
	}
	
	if(A->parent!=NULL)
	{
		int e,e1;
		e=A->e;
		
		abstractSyntaxTree par;
		par=A->parent;
		e1=A->e;
		
		fprintf(fp,"%d %30s %3d %30s %10s %30s %3s %30s\n",A->rno,A->lex,A->linenum,td[e],l,td[e1],yn,td[e]);
		//printf("%s\n",A->lex);
	}
	else
	{
		char p[4];
		strcpy(p,"N/A");
		
		fprintf(fp,"%d %30s %3d %30s %10s %30s %3s %30s\n",A->rno,A->lex,A->linenum,td[A->e],l,p,yn,td[A->e]);
	}
	
	fclose(fp);
	
	while(i<(A->childnum))
	{
		printAST(A->child[i],outFile,totalAllocatedMemory);
		i++;
	}
}

abstractSyntaxTree ast(abstractSyntaxTree *temp,int rno,int childnum)
{
	abstractSyntaxTree A;
	A=(abstractSyntaxTree)malloc(sizeof(struct _abstractSyntaxTree));
	
	A->e=rno/10;
	A->rno=rno;
	A->linenum=temp[0]->linenum;
	A->childnum=childnum;
	strcpy(A->lex,td[(A->e)]);
	
	//printf("A: %s\n",A->lex);
	
	int i;
	for(i=0;i<childnum;i++)
	{
		temp[i]->rno=rno;
		//printf("temp[%d]: %s\t",i,td[temp[i]->e]);
	}
	//printf("\n");
	
	//printf("Here1\n");
	
	if(rno==1001) //<program>
	{
		A->child=(abstractSyntaxTree *)malloc(2*sizeof(abstractSyntaxTree));
		
		A->child[0]=temp[0];
		A->child[1]=temp[1];
	}
	
	//printf("Here2\n");
	
	else if(rno==1011) //<mainFunction>
	{
		A=temp[0];
		A->childnum=1;
				
		A->child=(abstractSyntaxTree *)malloc(1*sizeof(abstractSyntaxTree));
		
		A->child[0]=temp[1];
	}
	
	else if(rno==1021) //<otherFunctions>
	{		
		A->child=(abstractSyntaxTree *)malloc(2*sizeof(abstractSyntaxTree));
		
		A->child[0]=temp[0];
		A->child[1]=temp[1];
	}
	
	else if(rno==1022) //<otherFunctions>
	{
		A=temp[0];
	}
	
	else if(rno==1031) //<function>
	{
		A=temp[0];
		A->childnum=3;
				
		A->child=(abstractSyntaxTree *)malloc(3*sizeof(abstractSyntaxTree));
		
		A->child[0]=temp[1];
		A->child[1]=temp[2];
		A->child[2]=temp[4];
	}
	
	else if(rno==1041) //<input_par>
	{
		A=temp[0];
		A->childnum=1;
				
		A->child=(abstractSyntaxTree *)malloc(1*sizeof(abstractSyntaxTree));
		
		A->child[0]=temp[4];
	}
	
	else if(rno==1051) //<output_par>
	{
		A=temp[0];
		A->childnum=1;
				
		A->child=(abstractSyntaxTree *)malloc(1*sizeof(abstractSyntaxTree));
		
		A->child[0]=temp[4];
	}
	
	else if(rno==1052) //<output_par>
	{
		A=temp[0];
	}
	
	else if(rno==1061) //<parameter_list>
	{
		A=temp[1];
		A->childnum=2;
		
		//printf("AAA\n");
				
		A->child=(abstractSyntaxTree *)malloc(2*sizeof(abstractSyntaxTree));
		
		A->child[0]=temp[0];
		A->child[1]=temp[2];
		
		//printf("ABC\n");
	}
	
	else if(rno==1071) //<dataType>
	{
		A=temp[0];
	}
	
	else if(rno==1072) //<dataType>
	{
		A=temp[0];
	}
	
	//printf("Here3\n");
	
	else if(rno==1081) //<primitiveDatatype>
	{
		A=temp[0];
	}
	
	//printf("Here4\n");
	
	else if(rno==1082) //<primitiveDatatype>
	{
		A=temp[0];
	}
	
	else if(rno==1091) //<constructedDatatype>
	{
		A=temp[1];
	}
	
	else if(rno==1101) //<remaining_list>
	{
		A=temp[1];
	}
	
	else if(rno==1102) //<remaining_list>
	{
		A=temp[0];
	}
	
	else if(rno==1111) //<stmts>
	{		
		A->child=(abstractSyntaxTree *)malloc(4*sizeof(abstractSyntaxTree));
		
		A->child[0]=temp[0];
		A->child[1]=temp[1];
		A->child[2]=temp[2];
		A->child[3]=temp[3];
	}
	
	else if(rno==1121) //<typeDefinitions>
	{
		A=temp[0];
		A->child[1]=temp[1];
	}
	
	else if(rno==1122) //<typeDefinitions>
	{
		A=temp[0];
	}
	
	else if(rno==1131) //<typeDefinition>
	{
		A=temp[1];
		A->childnum=2;
				
		A->child=(abstractSyntaxTree *)malloc(2*sizeof(abstractSyntaxTree));
		
		A->child[0]=temp[2];
		A->child[1]=NULL;
	}
	
	else if(rno==1141) //<fieldDefinitions>
	{
		A=temp[0];
		A->child[1]=temp[1];
		temp[1]->child[1]=temp[2];
	}
	
	else if(rno==1151) //<fieldDefinition>
	{
		A=temp[3];
		A->childnum=2;
				
		A->child=(abstractSyntaxTree *)malloc(2*sizeof(abstractSyntaxTree));
		
		A->child[0]=temp[1];
		A->child[1]=NULL;
	}
	
	else if(rno==1161) //<moreFields>
	{
		A=temp[0];
		A->child[1]=temp[1];
	}
	
	else if(rno==1162) //<moreFields>
	{
		A=temp[0];
	}
	
	//printf("Here n\n");
	
	else if(rno==1171) //<declarations>
	{
		//printf("temp[1]: %s\tdecs childnum: %d\n",td[temp[1]->e],childnum);
		A=temp[0];
		//printf("Hi!\n");
		A->child[2]=temp[1];
		//printf("Hi\n");
	}
	
	else if(rno==1172) //<declarations>
	{
		//printf("temp[0]: %s\tdecs2 childnum: %d\n",td[temp[0]->e],childnum);
		A=temp[0];
	}
	
	else if(rno==1181) //<declaration>
	{
		A=temp[3];
		A->childnum=3;
		
		//printf("dec root: %s\tdec root class: %d\n",td[A->e],A->e);
				
		A->child=(abstractSyntaxTree *)malloc(3*sizeof(abstractSyntaxTree));
		
		A->child[0]=temp[1];
		A->child[1]=temp[4];
		A->child[2]=NULL;
	}
	
	else if(rno==1191) //<global_or_not>
	{
		A=temp[1];
	}
	
	else if(rno==1192) //<global_or_not>
	{
		A=temp[0];
	}
	
	else if(rno==1201) //<otherStmts>
	{
		A->child=(abstractSyntaxTree *)malloc(2*sizeof(abstractSyntaxTree));
		
		A->child[0]=temp[0];
		A->child[1]=temp[1];
	}
	
	else if(rno==1202) //<otherStmts>
	{
		A=temp[0];
	}
	
	else if(rno==1211) //<stmt>
	{
		A=temp[0];
	}
	
	else if(rno==1212) //<stmt>
	{
		A=temp[0];
	}
	
	else if(rno==1213) //<stmt>
	{
		A=temp[0];
	}
	
	else if(rno==1214) //<stmt>
	{
		A=temp[0];
	}
	
	else if(rno==1215) //<stmt>
	{
		A=temp[0];
	}
	
	else if(rno==1221) //<assignmentStatement>
	{
		A=temp[1];
		A->childnum=2;
				
		A->child=(abstractSyntaxTree *)malloc(2*sizeof(abstractSyntaxTree));
		
		A->child[0]=temp[0];
		A->child[1]=temp[2];
	}
	
	//printf("Here n\n");
	
	else if(rno==1231) //<singleOrRecId>
	{
		A=temp[0];
		A->childnum=1;
				
		A->child=(abstractSyntaxTree *)malloc(1*sizeof(abstractSyntaxTree));
		
		A->child[0]=temp[1];
	}
	
	else if(rno==1491) //<singleOrRecIdprime>
	{
		A=temp[0];
		A->childnum=1;
				
		A->child=(abstractSyntaxTree *)malloc(1*sizeof(abstractSyntaxTree));
		
		A->child[0]=temp[1];
	}
	
	else if(rno==1492) //<singleOrRecIdprime>
	{
		A=temp[0];
	}
	
	//printf("Here n\n");
	
	else if(rno==1241) //<funCallStmt>
	{
		A=temp[2];
		A->childnum=2;
				
		A->child=(abstractSyntaxTree *)malloc(2*sizeof(abstractSyntaxTree));
		
		A->child[0]=temp[0];
		A->child[1]=temp[5];
	}
	
	else if(rno==1251) //<outputParameters>
	{
		A=temp[1];
	}
	
	else if(rno==1252) //<outputParameters>
	{
		A=temp[0];
	}
	
	else if(rno==1261) //<inputParameters>
	{
		A=temp[1];
	}
	
	else if(rno==1271) //<iterativeStmt>
	{
		A=temp[0];
		A->childnum=3;
				
		A->child=(abstractSyntaxTree *)malloc(3*sizeof(abstractSyntaxTree));
		
		A->child[0]=temp[2];
		A->child[1]=temp[4];
		A->child[2]=temp[5];
	}
	
	else if(rno==1281) //<conditionalStmt>
	{
		A=temp[0];
		A->childnum=3;
				
		A->child=(abstractSyntaxTree *)malloc(3*sizeof(abstractSyntaxTree));
		
		A->child[0]=temp[2];
		A->child[1]=temp[4];
		A->child[2]=temp[7];
		
		temp[4]->childnum=2;
		temp[4]->child=(abstractSyntaxTree *)malloc(2*sizeof(abstractSyntaxTree));
		
		temp[4]->child[0]=temp[5];
		temp[4]->child[1]=temp[6];
	}
	
	else if(rno==1291) //<elsePart>
	{
		A=temp[0];
		A->childnum=2;
				
		A->child=(abstractSyntaxTree *)malloc(2*sizeof(abstractSyntaxTree));
		
		A->child[0]=temp[1];
		A->child[1]=temp[2];
	}
	
	else if(rno==1292) //<elsePart>
	{
		A=NULL;
	}
	
	//printf("Here n\n");
	
	else if(rno==1301) //<ioStmt>
	{
		A=temp[0];
		A->childnum=1;
				
		A->child=(abstractSyntaxTree *)malloc(1*sizeof(abstractSyntaxTree));
		
		A->child[0]=temp[2];
	}
	
	else if(rno==1302) //<ioStmt>
	{
		A=temp[0];
		A->childnum=1;
				
		A->child=(abstractSyntaxTree *)malloc(1*sizeof(abstractSyntaxTree));
		
		A->child[0]=temp[2];
	}
	
	else if(rno==1311) //<allVar>
	{
		A=temp[0];
	}
	
	else if(rno==1312) //<allVar>
	{
		A=temp[1];
		A->childnum=2;
				
		A->child=(abstractSyntaxTree *)malloc(2*sizeof(abstractSyntaxTree));
		
		A->child[0]=temp[0];
		A->child[1]=temp[2];
	}
	
	else if(rno==1321) //<arithmeticExpression>
	{
		A->child=(abstractSyntaxTree *)malloc(2*sizeof(abstractSyntaxTree));
		
		A->child[0]=temp[0];
		A->child[1]=temp[1];
	}
	
	else if(rno==1331) //<expPrime>
	{
		A=temp[0];
		A->childnum=2;
		
		A->child=(abstractSyntaxTree *)malloc(2*sizeof(abstractSyntaxTree));
		
		A->child[0]=temp[1];
		A->child[1]=temp[2];
	}
	
	else if(rno==1332) //<expPrime>
	{
		A=temp[0];
	}
	
	else if(rno==1341) //<term>
	{
		A->child=(abstractSyntaxTree *)malloc(2*sizeof(abstractSyntaxTree));
		
		A->child[0]=temp[0];
		A->child[1]=temp[1];
	}
	
	else if(rno==1351) //<termPrime>
	{
		A=temp[0];
		A->childnum=2;
		
		A->child=(abstractSyntaxTree *)malloc(2*sizeof(abstractSyntaxTree));
		
		A->child[0]=temp[1];
		A->child[1]=temp[2];
	}
	
	else if(rno==1352) //<termPrime>
	{
		A=temp[0];
	}
	
	else if(rno==1361) //<factor>
	{
		A=temp[1];
	}
	
	else if(rno==1362) //<factor>
	{
		A=temp[0];
	}
	
	else if(rno==1371) //<highprecedenceOperator>
	{
		A=temp[0];
	}
	
	else if(rno==1372) //<highprecedenceOperator>
	{
		A=temp[0];
	}
	
	else if(rno==1381) //<lowprecedenceOperator>
	{
		A=temp[0];
	}
	
	else if(rno==1382) //<lowprecedenceOperator>
	{
		A=temp[0];
	}
	
	else if(rno==1391) //<all>
	{
		//printf("called");
		A=temp[0];
	}
	
	else if(rno==1392) //<all>
	{
		A=temp[0];
	}
	
	else if(rno==1393) //<all>
	{
		A=temp[0];
	}
	
	else if(rno==1394) //<all>
	{
		A=temp[0];
		A->childnum=1;
		
		A->child=(abstractSyntaxTree *)malloc(1*sizeof(abstractSyntaxTree));
		
		A->child[0]=temp[1];
	}
	
	else if(rno==1401) //<temp>
	{
		A=temp[0];
	}
	
	else if(rno==1402) //<temp>
	{
		A=temp[0];
		A->childnum=1;
		
		A->child=(abstractSyntaxTree *)malloc(1*sizeof(abstractSyntaxTree));
		
		A->child[0]=temp[1];
	}
	
	else if(rno==1411) //<booleanExpression>
	{
		A=temp[3];
		A->childnum=2;
		
		A->child=(abstractSyntaxTree *)malloc(2*sizeof(abstractSyntaxTree));
		
		A->child[0]=temp[1];
		A->child[1]=temp[5];
	}
	
	else if(rno==1412) //<booleanExpression>
	{
		A=temp[1];
		A->childnum=2;
		
		A->child=(abstractSyntaxTree *)malloc(2*sizeof(abstractSyntaxTree));
		
		A->child[0]=temp[0];
		A->child[1]=temp[2];
	}
	
	else if(rno==1413) //<booleanExpression>
	{
		A=temp[0];
		A->childnum=1;
		
		A->child=(abstractSyntaxTree *)malloc(1*sizeof(abstractSyntaxTree));
		
		A->child[0]=temp[1];
	}
	
	else if(rno==1421) //<var>
	{
		//printf("AAA\n");
		A=temp[0];
	}
	
	else if(rno==1422) //<var>
	{
		A=temp[0];	
	}
	
	else if(rno==1423) //<var>
	{
		A=temp[0];	
	}
	
	else if(rno==1431) //<logicalOp>
	{
		A=temp[0];
	}
	
	else if(rno==1432) //<logicalOp>
	{
		A=temp[0];
	}
	
	else if(rno==1441) //<relationalOp>
	{
		A=temp[0];
	}
	
	else if(rno==1442) //<relationalOp>
	{
		A=temp[0];
	}
	
	else if(rno==1443) //<relationalOp>
	{
		A=temp[0];
	}
	
	else if(rno==1444) //<relationalOp>
	{
		A=temp[0];
	}
	
	else if(rno==1445) //<relationalOp>
	{
		A=temp[0];
	}
	
	else if(rno==1446) //<relationalOp>
	{
		A=temp[0];
	}
	
	else if(rno==1451) //<returnStmt>
	{
		A=temp[1];
	}
	
	else if(rno==1461) //<optionalReturn>
	{
		A=temp[1];
	}
	
	else if(rno==1462) //<optionalReturn>
	{
		A=temp[0];
	}
	
	else if(rno==1471) //<idList>
	{
		A=temp[0];
		A->childnum=1;
		
		A->child=(abstractSyntaxTree *)malloc(1*sizeof(abstractSyntaxTree));
		
		A->child[0]=temp[1];
	}
	
	else if(rno==1481) //<more_ids>
	{
		A=temp[1];
	}
	
	//printf("Here n\n");
	
	else if(rno==1482) //<more_ids>
	{
		A=temp[0];
	}
	
	else
	{
		printf("Rule %d not found.\n",rno);
	}
	
	return A;
}

abstractSyntaxTree createAbstractSyntaxtree(parseTree T,abstractSyntaxTree A)
{	
	//A=(abstractSyntaxTree)malloc(sizeof(struct _abstractSyntaxTree));
		
	abstractSyntaxTree *temp;
	temp=(abstractSyntaxTree *)malloc((T->childnum)*sizeof(abstractSyntaxTree));
	
	int i;
	
	for(i=0;i<(T->childnum);i++)
	{
		temp[i]=(abstractSyntaxTree)malloc(sizeof(struct _abstractSyntaxTree));
	}

	//printf("%s\n",td[T->e]);
	
	i=0;
	
	while(i<(T->childnum))
	{
		temp[i]=createAbstractSyntaxtree(T->child[i],temp[i]);
		temp[i]->parent=A;
		i++;
	}
	
	if((T->e)>=100)
	{
		//printf("non-term: %s\trno: %d\tchildnum: %d\n",td[T->e],T->rno,T->childnum);
		A=ast(temp,(T->rno),(T->childnum));
		//printf("new root: %s\trno: %d\tchildnum: %d\n",td[A->e],A->rno,A->childnum);
	}
	
	else
	{
		A->e=T->e;
		A->rno=T->rno;
		A->linenum=T->linenum;
		A->childnum=0; //terminal
		strcpy(A->lex,T->lex);
		//A->child=NULL;
		/*
		if(A->e==15)
		{
			printf("TK_NUM: %s\n",A->lex);
		}
		*/
	}
	
	return A;
}

llist insertfirst(llist first,int e)
{
	llist temp,prevtemp,new;
	temp=first;
	prevtemp=first;
	
	new=(llist)malloc(sizeof(struct _llist));
	new->e=e;
	new->next=NULL;
	
	int f; //f set to 1 if e already present
	f=0;
	
	while(temp!=NULL && f==0)
	{
		if(temp->e==e)
		{
			f++;
		}
		prevtemp=temp;
		temp=temp->next;
	}
	
	if(f==0)
	{
		if(prevtemp==NULL) //insert at begining of list
		{
			first=new;
		}
		else
		{
			prevtemp->next=new;
		}
	}
	
	return first;		
}

llist insertfirstset(llist first,int e,int *flag,grammar G)
{
	int i,f;
	f=0;//f is set to 1 if epsilon is encountered
	
	for(i=0;i<(G[(e-100)].firstlen);i++)
	{
		if(G[(e-100)].first[i]!=53)
		{
			first=insertfirst(first,(G[(e-100)].first[i]));
		}
		else
		{
			f++;
		}
	}
	
	if(f==0)
	{
		*flag=1;
	}
	
	return first;		
}

grammar init_grammar(FILE *fp)
{
	grammar G;
	
	int nd,nr; //nd=no. of determinants, nr=no. of rules
	int i,j;
	
	fscanf(fp,"%d %d\n",&nd,&nr);
	//printf("nd:%d nr:%d\n\n",nd,nr);
	
	G=(grammar)malloc(nd*sizeof(determinant));
	
	for(i=0;i<nd;i++)
	{
		G[i].num_rules=0;
		G[i].head=NULL;
	}
	
	for(i=0;i<nr;i++)
	{
		int det,rn,len; //det=determinant no., rn=rule no. of determinant, len=length of rule
		
		fscanf(fp,"%d %d %d",&det,&rn,&len);
		//printf("%d %d %d",det,rn,len);
		
		int *rule_str; //rule string
		rule_str=(int *)malloc(len*sizeof(int));
		
		rulenode rnode,temp;
		rnode=(rulenode)malloc(sizeof(rule));
		
		(G[(det-100)].num_rules)++;
		rnode->rno=(det*10)+rn; //universal rule number
		rnode->len=len;
		rnode->next=NULL;
		rnode->eflag=0;
		
		for(j=0;j<len;j++) //read the grammar string 
		{
			int r;
			
			fscanf(fp," %d",&r);
			//printf(" %d",r);
			rule_str[j]=r;
		}
		
		//printf("\n");
		
		rnode->head=rule_str; //assigning the head to point to the integer array
		
		temp=G[(det-100)].head;
		
		if(G[(det-100)].head==NULL) //if determinant is empty
		{
			G[(det-100)].head=rnode;
			//printf("NULL:%d\n",det-100);
		}
		else //insert rule at end of determinant
		{
			while((temp->next)!=NULL)
			{
				temp=temp->next;
			}
			temp->next=rnode;
			//printf("NOT NULL:%d\n",det-100);
		}
	}
	
	return G;
}

void print_grammar(grammar G)
{
	int i,j;
	rulenode temp;
	
	printf("Grammar Rules:\n\n");
	
	for(i=0;i<ndet;i++)
	{
		temp=G[i].head;
		
		while(temp!=NULL)
		{
			printf("%s===> ",td[i+100]);
			
			for(j=0;j<(temp->len);j++)
			{
				/*if((temp->head[j])>=100)
				{
					printf("<%d> ",temp->head[j]);
				}
				else
				{
					printf("%d ",temp->head[j]);
				}*/
				printf("%s ",td[(temp->head[j])]);
			}
			
			printf("\n");
			
			temp=temp->next;
		}
	}
	
	printf("\nFIRST Sets:\n\n");
	
	for(i=0;i<ndet;i++)
	{
		printf("FIRST(%s): {",td[i+100]);
		
		for(j=0;j<(G[i].firstlen);j++)
		{
			printf("%s ",td[(G[i].first[j])]);
			if(j!=((G[i].firstlen)-1))
			{
				printf(",");
			}
		}
		
		printf("}\n");
	}
	
	printf("\nFOLLOW Sets:\n\n");
	
	for(i=0;i<ndet;i++)
	{
		printf("FOLLOW(%s): {",td[i+100]);
		
		for(j=0;j<(G[i].followlen);j++)
		{
			printf("%s ",td[(G[i].first[j])]);
			if(j!=((G[i].followlen)-1))
			{
				printf(",");
			}
		}
		
		printf("}\n");
	}
}

grammar init_ff(grammar G, FILE *fp1, FILE * fp2)
{
	int i,j;
		
	for(i=0;i<ndet;i++)
	{
		int det,lenfirst; //det=determinant no., lenfirst=no. of elements in first
		
		fscanf(fp1,"%d %d",&det,&lenfirst);
		//printf("%d %d",det,lenfirst);
		int *firststring;
		firststring= (int *)malloc(lenfirst*(sizeof(int)));
	
		for (j=0;j<lenfirst;j++)
		{
			int a;
			fscanf(fp1," %d",&a);
			//printf(" %d",a);
			firststring[j]=a;
		}
		
		//printf("\n");
			
		G[i].first=firststring;
		G[i].firstlen=lenfirst;
	}

	for(i=0;i<ndet;i++)
	{
		int det,lenfollow; //det=determinant no., lenfollow=no. of elements in follow
		
		fscanf(fp2,"%d %d",&det,&lenfollow);
		//printf("%d %d",det,lenfollow);
		int *followstring;
		followstring= (int *)malloc(lenfollow*(sizeof(int)));
	
		for (j=0;j<lenfollow;j++)
		{
			int a;
			fscanf(fp2," %d",&a);
			//printf(" %d",a);
			followstring[j]=a;
		}
		
		//printf("\n");
			
		G[i].follow=followstring;
		G[i].followlen=lenfollow;
	}
	
	return G;
}

grammar rulefirst(grammar G)
{
	int i,j;
	
	for(i=0;i<ndet;i++)
	{
		rulenode temprule;
		temprule=G[i].head;
		
		while(temprule!=NULL)
		{
			int len,flag; //flag set to 1 if first computation is over
			len=temprule->len;
			
			j=0;flag=0;
			
			while(j<len && flag==0)
			{
				if(temprule->head[j]<100 && temprule->head[j]!=53)
				{
					temprule->first=insertfirst(temprule->first,temprule->head[j]);
					flag++;
				}
				else if((temprule->head[j])>=100)
				{
					temprule->first=insertfirstset(temprule->first,temprule->head[j],&flag,G);
				}
				j++;
			}
			
			if(j==len && flag==0) //epsilon at the end
			{
				temprule->first=insertfirst(temprule->first,53);
				temprule->eflag++;
			}
			
			temprule=temprule->next;
		}
	}
	
	return G;	
}

void print_firstrules(grammar G)
{
	int i,j;
	rulenode temp;
	
	printf("\nFIRST SET of Rules:\n\n");
	
	for(i=0;i<ndet;i++)
	{
		temp=G[i].head;
		
		while(temp!=NULL)
		{
			llist tlist=temp->first;
			
			printf("Rule %d: {",temp->rno);
			
			while(tlist!=NULL)
			{
				printf("%s",td[tlist->e]);
				tlist=tlist->next;
				if(tlist!=NULL)
				{
					printf(",");
				}
			}
			
			printf("}\n");
			
			temp=temp->next;
		}
	}
}

void print_parsetable(table T)
{
	int i,j;
	
	printf("\nParse Table:\n\n");
	
	for(i=0;i<ndet;i++)
	{
		for(j=0;j<55;j++)
		{
			printf("%d ",T[i][j]);
		}
		printf("\n");
	}
}

void dfs(parseTree pt)
{
	int i=0;
	
	char l[31];
	if(pt->e==15 || pt->e==16)
	{
		strcpy(l,pt->lex);
	}
	else
	{
		strcpy(l,"N/A");
	}
	
	char yn[4];
	if(pt->childnum==0)
	{
		strcpy(yn,"yes");
	}
	else
	{
		strcpy(yn,"no");
	}
	
	if(pt->parent!=NULL)
	{
		int e,e1;
		e=pt->e;
		
		parseTree par;
		par=pt->parent;
		e1=par->e;
		
		printf("%s\t\t%d\t\t%s\t\t%s\t\t%s\t\t%s\t\t%s\n",pt->lex,pt->linenum,td[e],l,td[e1],yn,td[e]);
	}
	else
	{
		char p[4];
		strcpy(p,"N/A");
		
		printf("%s\t\t%d\t\t%s\t\t%s\t\t%s\t\t%s\t\t%s\n",pt->lex,pt->linenum,td[pt->e],l,p,yn,td[pt->e]);
	}
	
	while(i<(pt->childnum))
	{
		dfs(pt->child[i]);
		i++;
	}
}

void parse(FILE *fp,table T,grammar G)
{
	tokenInfo *ti;
	
	int i,len;
	char B[31];
	len=30;
	
	char lx[31];    //lexeme
   
   int *state,*j,*lnum;
   state=(int *)malloc(sizeof(int));
   j=(int *)malloc(sizeof(int));
   lnum=(int *)malloc(sizeof(int));	//current line number
   
   *state=0;*j=0;*lnum=1;
   
   tlist t;
   t=initList();
   
   while(len==30)
   {
		fp=getStream(fp,B,30);
      len=strlen(B);

      if(len<30)
      {
    	  B[len]=(char)32; //space
        B[len+1]='\0';
      }

      //printf("%s\n\n",B);

      //tokenize(B,len,state1,j1,*state1,*j1,st,asc,tk,lx);
	
	   while(B[len-1]!='^')
		{
			ti=getNextToken(B,len/*,state,j,st,asc,tk,lx,lnum*/);
		 	if(strcmp(ti->tok,"Error")!=0 && strcmp(ti->tok,"A")!=0)
		 	{
		 		tokenInfo temp;
		 		strcpy(temp.lex,ti->lex);
		 		strcpy(temp.tok,ti->tok);
		 		temp.lnum=ti->lnum;
		 		temp.tkclass=ti->tkclass;
		 		
		 		t=add(t,temp);
		 		//printf("%s\t%s\t%d\n",ti->lex,ti->tok,ti->lnum);
			}
		}
		
	}
   
   print_tlist(t);
   
   stack s;
	s=initStack();
	
	s=push(s,54); //TK_DOLLAR
	s=push(s,100); //<program>
	
	/*
	stack temp_stack;
	temp_stack=s;
	
	while(temp_stack!=NULL)
	{
		printf("%s\n",td[temp_stack->e]);
		temp_stack=temp_stack->next;
	}
	*/
	
	tlist curr_lex;
	curr_lex=t;
	
	//printf("abc\n");

	while(top(s)!=54 && curr_lex!=NULL)
	{
		if(top(s)==(curr_lex->ti.tkclass))
		{
			s=pop(s);
			curr_lex=curr_lex->next;
		}
		else if(top(s)>=0 && top(s)<55)
		{
			if(top(s)!=53)
			{
				printf("Error: Expected %s missing at line %d\n",td[top(s)],curr_lex->ti.lnum);
				break;
			}
			if(top(s)==53)
			{
				s=pop(s);
			}
		}
		else if(T[(top(s)-100)][curr_lex->ti.tkclass]==0)
		{
			printf("Error: No rule found for non-terminal %s at line %d\n",curr_lex->ti.tok,curr_lex->ti.lnum);
			printf("det: %d\tlex: %d\n",top(s),curr_lex->ti.tkclass);
			break;
		}
		else if(T[(top(s)-100)][curr_lex->ti.tkclass]!=0)
		{
			//printf("abc\n");
			int rno=T[(top(s)-100)][curr_lex->ti.tkclass];
			int det=rno/10;
			
			//printf("rno: %d\tdet: %d\n",rno,det);
			
			rulenode rule;
			rule=G[det-100].head;
			
			//printf("abc\n");
			
			int j1;
			
			j1=(rno%10)-1;
			
			//printf("j1: %d\n",j1);
			
			while(j1>0)
			{
				rule=rule->next;
				j1--;
			}
			
			int rlen=rule->len;
			//printf("rlen: %d\n",rlen);
			
			//printf("%s\n",td[100]);
			printf("%s==> ",td[(det)]);
			
			int *rulestr;
			rulestr=(int *)malloc(rlen*sizeof(int));
			
			//printf("abc\n");
			
			for(i=0;i<rlen;i++)
			{
				rulestr[i]=rule->head[i];
				printf("%s ",td[rulestr[i]]);
			}
			printf("\n");
			
			//printf("abc\n");
			
			s=pop(s);
			
			//printf("abc\n");
			
			for(i=(rlen-1);i>=0;i--)
			{
				s=push(s,rulestr[i]);
			}
		}
	}
	
	if(top(s)==54 && curr_lex==NULL)
	{
		printf("Program successfully parsed!\n");
	}
	else if(top(s)==54 && curr_lex!=NULL)
	{
		printf("Program has unparsed lexemes remaining!\n");
	}
	else if(top(s)!=54 && curr_lex==NULL)
	{
		printf("Program ended abruptly!\n");
	}
}

void init_td()
{
	int i;
	
	for(i=0;i<150;i++)
	{
		strcpy(td[i],"Error");
	}
	
	strcpy(td[0],"TK_MINUS");
	strcpy(td[1],"TK_NE");
	strcpy(td[2],"TK_RECORDID");
	strcpy(td[3],"TK_COMMENT");
	strcpy(td[4],"TK_AND");
	strcpy(td[5],"TK_OP");
	strcpy(td[6],"TK_CL");
	strcpy(td[7],"TK_MUL");
	strcpy(td[8],"TK_DOT");
	strcpy(td[9],"TK_DIV");
	strcpy(td[10],"TK_COLON");
	strcpy(td[11],"TK_SEM");
	strcpy(td[12],"TK_COMMA");
	strcpy(td[13],"TK_OR");
	strcpy(td[14],"TK_SQR");
	strcpy(td[15],"TK_NUM");
	strcpy(td[16],"TK_RNUM");
	strcpy(td[17],"TK_FIELDID");
	strcpy(td[18],"TK_ID");
	strcpy(td[19],"TK_SQL");
	strcpy(td[20],"TK_FUNID");
	strcpy(td[21],"TK_MAIN");
	strcpy(td[22],"TK_NOT");
	strcpy(td[23],"TK_PLUS");
	strcpy(td[24],"TK_LT");
	strcpy(td[25],"TK_ASSIGNOP");
	strcpy(td[26],"TK_LE");
	strcpy(td[27],"TK_EQ");
	strcpy(td[28],"TK_GT");
	strcpy(td[29],"TK_GE");
	strcpy(td[30],"TK_CALL");
	strcpy(td[31],"TK_ELSE");
	strcpy(td[32],"TK_END");
	strcpy(td[33],"TK_ENDIF");
	strcpy(td[34],"TK_ENDRECORD");
	strcpy(td[35],"TK_ENDWHILE");
	strcpy(td[36],"TK_GLOBAL");
	strcpy(td[37],"TK_IF");
	strcpy(td[38],"TK_INPUT");
	strcpy(td[39],"TK_INT");
	strcpy(td[40],"TK_LIST");
	strcpy(td[41],"TK_OUTPUT");
	strcpy(td[42],"TK_PARAMETER");
	strcpy(td[43],"TK_PARAMETERS");
	strcpy(td[44],"TK_READ");
	strcpy(td[45],"TK_REAL");
	strcpy(td[46],"TK_RECORD");
	strcpy(td[47],"TK_RETURN");
	strcpy(td[48],"TK_THEN");
	strcpy(td[49],"TK_TYPE");
	strcpy(td[50],"TK_WHILE");
	strcpy(td[51],"TK_WITH");
	strcpy(td[52],"TK_WRITE");
	strcpy(td[53],"TK_EPS");
	strcpy(td[54],"TK_DOLLAR");
	
	strcpy(td[100],"<program>");
	strcpy(td[101],"<mainFunction>");
	strcpy(td[102],"<otherFunctions>");
	strcpy(td[103],"<function>");
	strcpy(td[104],"<input_par>");
	strcpy(td[105],"<output_par>");
	strcpy(td[106],"<parameter_list>");
	strcpy(td[107],"<dataType>");
	strcpy(td[108],"<primitiveDatatype>");
	strcpy(td[109],"<constructedDatatype>");
	strcpy(td[110],"<remaining_list>");
	strcpy(td[111],"<stmts>");
	strcpy(td[112],"<typeDefinitions>");
	strcpy(td[113],"<typeDefinition>");
	strcpy(td[114],"<fieldDefinitions>");
	strcpy(td[115],"<filedDefinition>");
	strcpy(td[116],"<moreFields>");
	strcpy(td[117],"<declarations>");
	strcpy(td[118],"<declaration>");
	strcpy(td[119],"<global_or_not>");
	strcpy(td[120],"<otherStmts>");
	strcpy(td[121],"<stmt>");
	strcpy(td[122],"<assignmentStmt>");
	strcpy(td[123],"<singleOrRecId>");
	strcpy(td[124],"<funCallStmt>");
	strcpy(td[125],"<outputParameters>");
	strcpy(td[126],"<inputParameters>");
	strcpy(td[127],"<iterativeStmt>");
	strcpy(td[128],"<conditionalStmt>");
	strcpy(td[129],"<elsePart>");
	strcpy(td[130],"<ioStmt>");
	strcpy(td[131],"<allVar>");
	strcpy(td[132],"<arithmeticExpression>");
	strcpy(td[133],"<expPrime>");
	strcpy(td[134],"<term>");
	strcpy(td[135],"<termPrime>");
	strcpy(td[136],"<factor>");
	strcpy(td[137],"<highPrecedenceOperators>");
	strcpy(td[138],"<lowPrecedenceOperators>");
	strcpy(td[139],"<all>");
	strcpy(td[140],"<temp>");
	strcpy(td[141],"<booleanExpression>");
	strcpy(td[142],"<var>");
	strcpy(td[143],"<logicalOp>");
	strcpy(td[144],"<relationalOp>");
	strcpy(td[145],"<returnStmt>");
	strcpy(td[146],"<optionalReturn>");
	strcpy(td[147],"<idList>");
	strcpy(td[148],"<more_ids>");
	strcpy(td[149],"<singleOrRecIdprime>");
}

void print_parsetable_file(FILE *fp,table T)
{
	int i,j;
	
	fprintf(fp,"                    ");
	
	for(i=0;i<55;i++)
	{
		fprintf(fp,"%s ",td[i]);
	}
	fprintf(fp,"\n");
	
	for(i=0;i<50;i++)
	{
		fprintf(fp,"%s\t",td[i+100]);
		for(j=0;j<55;j++)
		{
			fprintf(fp,"%d\t",T[i][j]);
		}
		fprintf(fp,"\n");
	}
}

/*
rulenode add_rule(grammar G,int *r,int det,int rn,int len)
{
	rulenode rnode;
	rnode=(rulenode)malloc(sizeof(rule));
	
	(G[(det-100)].num_rules)++;
	rnode->rno=(det*10)+rn; //universal rule number
	rnode->len=len;
	rnode->next=NULL;
	
	rnode->head=r; //assigning the head to point to the integer array
	
	temp=G[(det-100)].head;
		
	if(G[(det-100)].head==NULL) //if determinant is empty
	{
		G[(det-100)].head=rnode;
	}
	else //insert rule at end of determinant
	{
		while((temp->next)!=NULL)
		{
			temp=temp->next;
		}
		temp->next=rnode;
	}
	
	return r;
}
*/
