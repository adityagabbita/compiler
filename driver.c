//Batch No						38
//Krishna Aditya Gabbita		2009B3A7608P
//Varun Sah						2010A7PS078P

#include "lexer.h"
#include "parser.h"
#include "symbolTable.h"
#include "typeExtractor.h"
#include "semantics.h"
#include "codegen.h"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(int argc,char **argv)
{
	if(argc!=3)
	{
		printf("Please enter command line argument as $./toycompiler testcase.txt code.asm\n");
		//printf("Please enter command line argument as $./stage1exe testcase.txt parsetreeOutFile.txt astOutFile.txt\n");
		return 0;
	}
	
	parseTree pt;
	pt=NULL;
	
	int lexerr=0;
	
	ts_err=0; //type extractor and semantic error set to zero
	
	FILE *fp,*fp1,*fp2,*fp3,*fp4,*fpn,*fpcode;
	fp1=fopen("grammar.txt","r");
	fp2=fopen("first.txt","r");
	fp3=fopen("follow.txt","r");
	fp4=fopen("parsetable.txt","w");
	fp=fopen("current_input.txt","w");
	fpcode=fopen(argv[2],"w");
	
	init_td();
	
	grammar G;
	G=init_grammar(fp1);
	G=init_ff(G,fp2,fp3);
	
	popst();
	ascii();
   tokens();
   print_tables();
   
   G=rulefirst(G);
	//print_firstrules(G,td);
	
	table T;
	createParseTable(G,T);
	//print_parsetable(T);
	print_parsetable_file(fp4,T);
	
	fpn=fopen(argv[1],"r");
	fp=process(fpn,fp);
	fclose(fp);
	fclose(fpn);
	
	parse_err=0;
	
	abstractSyntaxTree A;
	A=NULL;
	
	int choice;
	int code=0;
	
	do
	{
		printf("\n1. Print token list of source code %s\n",argv[1]);
		printf("2. Parse the input source code and produce Parse Tree in %s\n","parsetreeOutFile.txt");
		printf("3. Create Abstract Syntax Tree and print in file %s\n","astOutFile.txt");
		printf("4. Print Symbol Table\n");
		printf("5. Compile(front end only) to verify the syntactic and semantic correctness of the input source code\n");
		printf("6. Produce Assembly code in %s\n",argv[2]);
		printf("7. Exit\n");
		
		printf("\nEnter your choice:");
		scanf("%d",&choice);
		
		if(choice==1)
		{
			fp=fopen("current_input.txt","r");
			
			tokenInfo *ti;
	
			tlist t; //to store list of errors to be reported
   		t=initList();
			
			int i,len;
			char B[31];
			len=30;
	
		   state=(int *)malloc(sizeof(int));
   		j=(int *)malloc(sizeof(int));
   		lnum=(int *)malloc(sizeof(int));	//current line number
   
   		*state=0;*j=0;*lnum=1;
   
   		//tlist t;
   		//t=initList();
   
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
		 				//tokenInfo temp;
		 				//strcpy(temp.lex,ti->lex);
		 				//strcpy(temp.tok,ti->tok);
		 				//temp.lnum=ti->lnum;
		 				//temp.tkclass=ti->tkclass;
		 		
		 				//t=add(t,temp);
		 				printf("%20s %20s %6d\n",ti->tok,ti->lex,ti->lnum);
					}
			
					if(ti->tkclass==-1)
					{
						tokenInfo temp;
		 				strcpy(temp.lex,ti->lex);
		 				temp.lnum=ti->lnum;
		 				temp.tkclass=ti->tkclass;
		 		
						t=add(t,temp);
						//printf("ERROR_1: Identifier %s at line %d is longer than the prescribed length of 20 characters\n",ti->lex,ti->lnum);
					}
			
					if(ti->tkclass==-2)
					{
						char sym;
						sym=ti->lex[0];
				
						if(sym!=' ' && sym!='\t' && sym!='\n' && sym!=EOF)
						{			
							tokenInfo temp;
		 					strcpy(temp.lex,ti->lex);
		 					temp.lnum=ti->lnum;
		 					temp.tkclass=ti->tkclass;
		 		
							t=add(t,temp);
							//printf("ERROR_2: Unknown Symbol '%c' at line %d\n",ti->lex[0],ti->lnum);
						}
					}
			
					if(ti->tkclass==-3)
					{
						tokenInfo temp;
		 				strcpy(temp.lex,ti->lex);
		 				temp.lnum=ti->lnum;
		 				temp.tkclass=ti->tkclass;
		 		
						t=add(t,temp);
						//printf("ERROR_3: Unknown pattern %s at line %d\n",ti->lex,ti->lnum);
					}
				}
			}
			
			printf("\n");
			
			tlist print;
			print=t;
			
			if(print==NULL)
			{
				printf("No Lexical errors.\n");
			}
			else
			{
				printf("Lexical errors:\n");
				lexerr++;
			}
			
			while(print!=NULL)
			{
				if(print->ti.tkclass==-1)
				{
					printf("ERROR_1: Identifier %s at line %d is longer than the prescribed length of 20 characters\n",print->ti.lex,print->ti.lnum);
				}
				else if(print->ti.tkclass==-2)
				{
					printf("ERROR_2: Unknown Symbol '%c' at line %d\n",print->ti.lex[0],print->ti.lnum);
				}
				else if(print->ti.tkclass==-3)
				{
					printf("ERROR_3: Unknown pattern %s at line %d\n",print->ti.lex,print->ti.lnum);
				}
				
				print=print->next;
			}
			
			printf("\n");
			
			fclose(fp);	
		}
		
		else if(choice==2)
		{
			if(lexerr==0)
			{
				fp=fopen("current_input.txt","r");
			
				pt=parseInputSourceCode(fp,T,G);
	
				int mem;
				mem=0;
	
				if(pt!=NULL)
				{
					if(parse_err==0)
					{
						printParseTree(pt,"parsetreeOutFile.txt",&mem);
						printf("Parse Tree Total Memory: %d KB\n",mem/1000);
					}
					else
					{
						printf("Syntax errors! Can't print parse tree\n");
					}
				}
				else
				{
					printf("Lexical errors present in program! Can't create a parse tree.\n");
				}
				fclose(fp);
			}
			else
			{
				printf("Lexical errors present in program! Can't create a parse tree.\n");
			}
		}
	
		else if(choice==3)
		{
			if(pt==NULL)
			{
				printf("Parse the input before creating AST!\n");
			}
			
			else
			{
				if(parse_err==0)
				{
					A=createAbstractSyntaxtree(pt,A);
					A->parent=NULL;
		
					int mem;
					mem=0;
	
					printAST(A,"astOutFile.txt",&mem);
					printf("\nAST Total Memory: %d KB\n",mem/1000);
					
					s=init_st(A);
					
					//printf("\nNo. of functions: %d\n",s->fno);
				}
				else
				{
					printf("Syntax errors! Can't create AST\n");
				}
			}
		}
		
		else if(choice==4)
		{
			if(pt==NULL)
			{
				printf("Parse the input before creating Symbol Table!\n");
			}
			
			else if(A==NULL)
			{
				if(parse_err!=0)
				{
					printf("Syntax Errors! Can't create Symbol Table!\n");
				}
				else
				{
					printf("Create AST before creating Symbol Table!\n");
				}
			}
			
			else
			{
				if(parse_err==0)
				{
					printf("\nSymbol Table:\n");
					print_st(s);
					
					//printf("\nNo. of functions: %d\n",s->fno);
				}
				else
				{
					printf("Syntax errors! Can't create Symbol Table\n");
				}
			}
		}
		
		else if(choice==5)
		{
			if(pt==NULL)
			{
				printf("Parse the input before checking for further errors!\n");
			}
			
			else if(A==NULL)
			{
				if(parse_err!=0)
				{
					printf("Syntax Errors! Can't check for further errors!\n");
				}
				else
				{
					printf("Create AST before checking for further errors!\n");
				}
			}
			
			else
			{
				if(parse_err==0)
				{
					printf("\nType checking Errors:\n");
					type_check(s,A); //Type checker
					if(ts_err==0)
					{
						printf("None\n");
					}
					
					printf("\nSemantic Errors:\n");
					sem_checker(s,A); //Semantic checker
					if(ts_err==0)
					{
						printf("Code compiles successfully...\n");
						code++;
					}
					
					//printf("\nNo. of functions: %d\n",s->fno);
				}
				else
				{
					printf("Syntax errors! Can't check for further errors!\n");
				}
			}
		}
		
		else if(choice==6)
		{
			if(pt==NULL)
			{
				printf("Parse the input before generating the Assembly code!\n");
			}
			
			else if(A==NULL)
			{
				if(parse_err!=0)
				{
					printf("Syntax Errors! Can't generate the Assembly code!\n");
				}
				else
				{
					printf("Create AST before generating the Assembly code!\n");
				}
			}
			
			else
			{
				if(parse_err==0 && ts_err==0)
				{
					jump=1;
					
					if(code>0)
					{
						asm_code(s,A,fpcode);
						printf("\nAST code generated in %s\n",argv[2]);
					}
					else
					{
						printf("Check for Type checking & Semntic errors before generating the Assembly code!\n");
					}
					
					//printf("\nNo. of functions: %d\n",s->fno);
				}
				else if(parse_err>0)
				{
					printf("Syntax errors! Can't generate the Assembly code!\n");
				}
				else
				{
					printf("Type checking and Semantic errors! Can't generate the Assembly code!\n");
				}
			}
		}
		
		else if(choice==7)
		{
			printf("Quitting...\n");
		}
		
		else
		{
			printf("Enter correct choice!\n");
		}
	}
	while(choice!=7);
	
	fclose(fpcode);
		
	return 0;
}
