//Batch No						38
//Krishna Aditya Gabbita		2009B3A7608P
//Varun Sah						2010A7PS078P

#include "lexer.h"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

FILE *getStream(FILE *fp,char *B,int k)
{
    int i=0;

    while(i<k && ((B[i++]=fgetc(fp))!=EOF));
    B[i]='\0';

    return fp;
}

tokenInfo *getNextToken(char *B,int k)
{
	//printf("state:%d\tj:%d\n",*state,*j);
	
	int i,x,ch;
	tokenInfo *ti;
	ti=(tokenInfo *)malloc(sizeof(tokenInfo));
	ti->lnum=*lnum;
	
	//printf("!!!\n");
	
	for(i=0;B[i]=='^';i++);
	
	//printf("i:%d\n",i);
	
	for(i;i<k;i++)
	{
		//printf("i:%d\n",i);
		lx[(*j)++]=B[i];
		
		if(B[i]=='\n')
		{
			(*lnum)++;
		}
		
		//printf("lx:%s\n",lx);
		
		int a;
		a=(int)B[i];
		
		if(a<0)
		{
			ch=84;	//eof
		}
		else
		{
			ch=asc[a];
		}
		
		*state=st[*state][ch];
		
		//printf("%c: %d %d\n",B[i],ch,*state);
		
		if(*state<1000 && i!=k-1)
		{
			if(st[*state][ch]==1000)
         {
         	lx[*j]='\0';
         	//printf("%s:\t%s\n",lx,tk[state]);
            strcpy(ti->lex,lx);
            strcpy(ti->tok,tk[*state]);
            ti->tkclass=tk_class(*state);
            *state=0;*j=0;
            
            for(x=0;x<=i;x++)
            {
            	B[x]='^';
            }
            
            return ti;
         }
         if(st[*state][ch]==1001)
         {
            if(B[i]=='\n')
            {
            	(*lnum)--;
            }
            lx[--(*j)]='\0';
            //printf("%s:\t%s\n",lx,tk[state]);
            strcpy(ti->lex,lx);
            strcpy(ti->tok,tk[*state]);
            ti->tkclass=tk_class(*state);
            i--;
            
            if(*state==53 && ((*j)>20))
            {
            	strcpy(ti->tok,"TK_ERROR1");
            	ti->tkclass=-1;
           	}
            
            *state=0;*j=0;
            
            for(x=0;x<=i;x++)
            {
            	B[x]='^';
            }
            
            return ti;
         }
		}
		
		if(i==k-1)
      {
      	if(*state!=1002)
         {
      	   if(st[*state][ch]!=1000 && st[*state][ch]!=1001)
            {
      	      lx[*j]='\0';
               //printf("%s:\tIncomplete\n",lx);
               //printf("a\n");
               strcpy(ti->lex,"^");
               strcpy(ti->tok,"A");
               
               for(x=0;x<=i;x++)
            	{
            		B[x]='^';
            	}
            	
            	return ti;
            }
            if(st[*state][ch]==1000)
            {
               lx[*j]='\0';
               //printf("%s:\t%s\n",lx,tk[state]);if(*state==53 && *j>20)
            	{
            		strcpy(ti->tok,"TK_ERROR1");
            		ti->tkclass=-1;
            	}
               strcpy(ti->lex,lx);
            	strcpy(ti->tok,tk[*state]);
            	ti->tkclass=tk_class(*state);
               *state=0;*j=0;
               
               for(x=0;x<=i;x++)
            	{
            		B[x]='^';
            	}
            
            	return ti;
            }
            if(st[*state][ch]==1001)
            {
            	if(B[i]=='\n')
            	{
            		(*lnum)--;
            	}            	
            	lx[--(*j)]='\0';
            	//printf("%s:\t%s\n",lx,tk[state]);
            	strcpy(ti->lex,lx);
            	strcpy(ti->tok,tk[*state]);
            	ti->tkclass=tk_class(*state);
            	i--;
            	
            	if(*state==53 && ((*j)>20))
            	{
            		strcpy(ti->tok,"TK_ERROR1");
            		ti->tkclass=-1;
            	}
            	
            	
            	*state=0;*j=0;
            
            	for(x=0;x<=i;x++)
            	{
            		B[x]='^';
            	}
            
            	return ti;
            }
         }
      } 
         
      if(*state==1002)
		{
			lx[*j]='\0';
			
			int symbol_flag=0;
			if(*j==1)
			{
				symbol_flag++;
			}
			
			*state=0;
     		*j=0;
     		strcpy(ti->lex,lx);
     		if(symbol_flag!=0)
     		{
     			strcpy(ti->tok,"TK_ERROR2");
     			ti->tkclass=-2;
     		}
     		else
     		{
     			strcpy(ti->tok,"TK_ERROR3");
     			ti->tkclass=-3;
     		}
     		
     		//printf("lx:%s\ttok:%s\n",lx,ti->tok);
     		
     		for(x=0;x<=i;x++)
         {
         	B[x]='^';
         }
            
         return ti;
     	}
	}
}

FILE *process(FILE *fp1,FILE *fp)
{
	char c;
	while((c=getc(fp1))!= EOF)
 	{
  		if (c=='%')              //is comment
  		{
    		char p;
    		fputc(' ', fp);               //replace comment with space
    		do
    		{
     			p=c;
     			c=getc(fp1);
    		} 
    		while (c!='\n' && c!= EOF);
			    
    		if(c=='\n')
    		{
    			fputc(c,fp);
    		}
    		else if(c==EOF)
    		{
    			fputc(c,fp);
    			return fp;
    		}
    	}
  
  		else
  		{
   		fputc(c, fp);
  		}
 	}
 
 	return fp;
}

tlist add(tlist t,tokenInfo ti)
{
	tlist new;
	new= (tlist) malloc (sizeof(struct tnode));
	new->ti = ti;
	new->next=NULL;
	
	tlist temp;
	temp = t;
	
	if(temp==NULL)
	{
		t = new;
		return t;
	}
	
	while((temp->next) != NULL)
	{
	temp = temp->next;
	}
	
	temp->next= new;
	return t;
}

tlist initList ()
{
	tlist t;
	t= NULL;
	return t;
}

void print_tlist(tlist t)
{
	tlist temp;
	temp=t;
	
	printf("Lexemes:\n");
	
	while(temp!=NULL)
	{
		tokenInfo ti_temp;
		ti_temp=temp->ti;
		
		printf("%s %s %d %d\n",ti_temp.lex,ti_temp.tok,ti_temp.tkclass,ti_temp.lnum);
		temp=temp->next;
	}
}

void popst()
{
   int i,j;

	for(i=0;i<170;i++)			// Initializing the symbol table
	{
		for(j=0;j<85;j++)
		{
			st[i][j]=0;
		}
	}

	// ASCII values

	for(i=0;i<26;i++)		// small alphabets
	{
		st[169][i]=(i+97);
	}

	for(i=26;i<52;i++)		// capital alphabets
	{
		st[169][i]=(i-26)+65;
	}

	for(i=52;i<62;i++)		// digits
	{
		st[169][i]=(i-52)+48;
	}

	st[169][62]=45;           // special characters
	st[169][63]=33;
	st[169][64]=35;
	st[169][65]=37;
	st[169][66]=38;
	st[169][67]=40;
	st[169][68]=41;
	st[169][69]=42;
	st[169][70]=46;
	st[169][71]=47;
	st[169][72]=58;
	st[169][73]=59;
	st[169][74]=64;
	st[169][75]=91;
	st[169][76]=93;
	st[169][77]=95;
	st[169][78]=126;
	st[169][79]=43;
	st[169][80]=60;
	st[169][81]=61;
	st[169][82]=62;
	st[169][83]=44;

	// Non alphabet characters

	for(i=0;i<170;i++)
	{
		st[i][84]=1002;
	}

	// State 0

	for(i=26;i<52;i++)  //error for all caps
	{
	    st[0][i]=1002;
	}

	for(i=0;i<26;i++)
	{
	    st[0][i]=165;
	}

	for(i=52;i<62;i++)
	{
	    st[0][i]=35;
	}

	st[0][1]=49;
	st[0][3]=49;
	st[0][2]=54;
	st[0][19]=59;
	st[0][22]=68;
	st[0][67]=21;
	st[0][68]=22;
	st[0][4]=83;
   st[0][6]=107;
	st[0][15]=114;
	st[0][17]=126;
	st[0][8]=143;
	st[0][14]=153;
	st[0][11]=160;
	st[0][80]=1;
	st[0][82]=7;
	st[0][81]=10;
	st[0][63]=12;
	st[0][66]=14;
	st[0][74]=17;
   st[0][75]=24;
	st[0][76]=25;
	st[0][79]=26;
	st[0][62]=27;
	st[0][69]=28;
	st[0][71]=29;
	st[0][73]=30;
	st[0][72]=31;
	st[0][77]=40;
	st[0][70]=32;
	st[0][64]=33;
	st[0][65]=23;
	st[0][83]=168;
	st[0][78]=20;

    // State 1

    for(i=0;i<85;i++)
    {
        st[1][i]=6;
    }

    st[1][62]=2;
    st[1][81]=5;

    // State 2

    for(i=0;i<84;i++)
    {
        st[2][i]=1002;
    }

    st[2][62]=3;

    // State 3

    for(i=0;i<84;i++)
    {
        st[3][i]=1002;
    }

    st[3][62]=4;

    // State 4

    for(i=0;i<85;i++)
    {
        st[4][i]=1000;
    }

    // State 5

    for(i=0;i<85;i++)
    {
        st[5][i]=1000;
    }

    // State 6

    for(i=0;i<85;i++)
    {
        st[6][i]=1001;
    }

    // State 7

    for(i=0;i<85;i++)
    {
        st[7][i]=9;
    }

    st[7][81]=8;

    // State 8

    for(i=0;i<85;i++)
    {
        st[8][i]=1000;
    }

    // State 9

    for(i=0;i<85;i++)
    {
        st[9][i]=1001;
    }

    // State 10

    for(i=0;i<84;i++)
    {
        st[10][i]=1002;
    }

    st[10][81]=11;

    // State 11

    for(i=0;i<85;i++)
    {
        st[11][i]=1000;
    }

    // State 12

    for(i=0;i<85;i++)
    {
        st[12][i]=1002;
    }

    st[12][81]=13;

    // State 13

    for(i=0;i<85;i++)
    {
        st[13][i]=1000;
    }

    // State 14

    for(i=0;i<84;i++)
    {
        st[14][i]=1002;
    }

    st[14][66]=15;

    // State 15

    for(i=0;i<84;i++)
    {
        st[15][i]=1002;
    }

    st[15][66]=16;

    // State 16

    for(i=0;i<85;i++)
    {
        st[16][i]=1000;
    }

    // State 17

    for(i=0;i<84;i++)
    {
        st[17][i]=1002;
    }

    st[17][74]=18;

    // State 18

    for(i=0;i<84;i++)
    {
        st[18][i]=1002;
    }

    st[18][74]=19;

    // State 19

    for(i=0;i<85;i++)
    {
        st[19][i]=1000;
    }

    // State 20-32

    for(i=20;i<33;i++)
    {
        for(j=0;j<85;j++)
        {
            st[i][j]=1000;
        }
    }

    // State 33

    for(i=0;i<84;i++)
    {
        st[33][i]=1002;
    }

    for(i=0;i<26;i++)
    {
        st[33][i]=34;
    }

    // State 34

    for(i=0;i<85;i++)
    {
        st[34][i]=167;
    }

    for(i=0;i<26;i++)
    {
        st[34][i]=34;
    }

    // State 35

    for(i=0;i<85;i++)
    {
        st[35][i]=38;
    }

    st[35][70]=36;

    for(i=52;i<62;i++)
    {
        st[35][i]=35;
    }

    // State 36

    for(i=0;i<84;i++)
    {
        st[36][i]=1002;
    }

    for(i=52;i<62;i++)
    {
        st[36][i]=37;
    }

    // State 37

    for(i=0;i<84;i++)
    {
        st[37][i]=1002;
    }

    for(i=52;i<62;i++)
    {
        st[37][i]=39;
    }

    // State 38

    for(i=0;i<85;i++)
    {
        st[38][i]=1001;
    }

    // State 39

    for(i=0;i<85;i++)
    {
        st[39][i]=1000;
    }

    // State 40

    for(i=0;i<84;i++)
    {
        st[40][i]=1002;
    }

    for(i=0;i<52;i++)
    {
        st[40][i]=46;
    }

    st[40][12]=41;

    // State 41

    for(i=0;i<85;i++)
    {
        st[41][i]=48;
    }

    for(i=0;i<52;i++)
    {
        st[41][i]=46;
    }

    st[41][0]=42;

    for(i=52;i<62;i++)
    {
        st[41][i]=47;
    }

    // State 42

    for(i=0;i<85;i++)
    {
        st[42][i]=48;
    }

    for(i=0;i<52;i++)
    {
        st[42][i]=46;
    }

    st[42][8]=43;

    for(i=52;i<62;i++)
    {
        st[42][i]=47;
    }

    // State 43

    for(i=0;i<85;i++)
    {
        st[43][i]=48;
    }

    for(i=0;i<52;i++)
    {
        st[43][i]=46;
    }

    st[43][13]=44;

    for(i=52;i<62;i++)
    {
        st[43][i]=47;
    }

    // State 44

    for(i=0;i<85;i++)
    {
        st[44][i]=45;
    }

    for(i=0;i<52;i++)
    {
        st[44][i]=46;
    }

    for(i=52;i<62;i++)
    {
        st[44][i]=47;
    }

    // State 45

    for(i=0;i<85;i++)
    {
        st[45][i]=1001;
    }

    // State 46

    for(i=0;i<85;i++)
    {
        st[46][i]=48;
    }

    for(i=0;i<52;i++)
    {
        st[46][i]=46;
    }

    for(i=52;i<62;i++)
    {
        st[46][i]=47;
    }

    // State 47

    for(i=0;i<85;i++)
    {
        st[47][i]=48;
    }

    for(i=52;i<62;i++)
    {
        st[47][i]=47;
    }

    // State 48

    for(i=0;i<85;i++)
    {
        st[48][i]=1001;
    }

    // State 49

    for(i=0;i<85;i++)
    {
        st[49][i]=166;
    }

    for(i=0;i<26;i++)
    {
        st[49][i]=165;
    }

    for(i=54;i<60;i++)
    {
        st[49][i]=50;
    }

    // State 50

    for(i=0;i<85;i++)
    {
        st[50][i]=53;
    }

    for(i=1;i<4;i++)
    {
        st[50][i]=51;
    }

    for(i=54;i<60;i++)
    {
        st[50][i]=52;
    }

    // State 51

    for(i=0;i<85;i++)
    {
        st[51][i]=53;
    }

    for(i=1;i<4;i++)
    {
        st[51][i]=51;
    }

    for(i=54;i<60;i++)
    {
        st[51][i]=52;
    }

    // State 52

    for(i=0;i<85;i++)
    {
        st[52][i]=53;
    }

    for(i=54;i<60;i++)
    {
        st[52][i]=52;
    }

    // State 53

    for(i=0;i<85;i++)
    {
        st[53][i]=1001;
    }

    // State 54

    for(i=0;i<85;i++)
    {
        st[54][i]=166;
    }

    for(i=0;i<26;i++)
    {
        st[54][i]=165;
    }

    for(i=54;i<60;i++)
    {
        st[54][i]=50;
    }

    st[54][0]=55;

    // State 55

    for(i=0;i<85;i++)
    {
        st[55][i]=166;
    }

    for(i=0;i<26;i++)
    {
        st[55][i]=165;
    }

    st[55][11]=56;

    // State 56

    for(i=0;i<85;i++)
    {
        st[56][i]=166;
    }

    for(i=0;i<26;i++)
    {
        st[56][i]=165;
    }

    st[56][11]=57;

    // State 57

    for(i=0;i<85;i++)
    {
        st[57][i]=58;
    }

    for(i=0;i<26;i++)
    {
        st[57][i]=165;
    }

    // State 58

    for(i=0;i<85;i++)
    {
        st[58][i]=1001;
    }

    // State 59

    for(i=0;i<85;i++)
    {
        st[59][i]=166;
    }

    for(i=0;i<26;i++)
    {
        st[59][i]=165;
    }

    st[59][7]=60;
    st[59][24]=64;

    // State 60

    for(i=0;i<85;i++)
    {
        st[60][i]=166;
    }

    for(i=0;i<26;i++)
    {
        st[60][i]=165;
    }

    st[60][4]=61;

    // State 61

    for(i=0;i<85;i++)
    {
        st[61][i]=166;
    }

    for(i=0;i<26;i++)
    {
        st[61][i]=165;
    }

    st[61][13]=62;

    // State 62

    for(i=0;i<85;i++)
    {
        st[62][i]=63;
    }

    for(i=0;i<26;i++)
    {
        st[62][i]=165;
    }

    // State 63

    for(i=0;i<85;i++)
    {
        st[63][i]=1001;
    }

    // State 64

    for(i=0;i<85;i++)
    {
        st[64][i]=166;
    }

    for(i=0;i<26;i++)
    {
        st[64][i]=165;
    }

    st[64][15]=65;

    // State 65

    for(i=0;i<85;i++)
    {
        st[65][i]=166;
    }

    for(i=0;i<26;i++)
    {
        st[65][i]=165;
    }

    st[65][4]=66;

    // State 66

    for(i=0;i<85;i++)
    {
        st[66][i]=67;
    }

    for(i=0;i<26;i++)
    {
        st[66][i]=165;
    }

    // State 67

    for(i=0;i<85;i++)
    {
        st[67][i]=1001;
    }

    // State 68

    for(i=0;i<85;i++)
    {
        st[68][i]=166;
    }

    for(i=0;i<26;i++)
    {
        st[68][i]=165;
    }

    st[68][7]=69;
    st[68][8]=74;
    st[68][17]=78;

    // State 69

    for(i=0;i<85;i++)
    {
        st[69][i]=166;
    }

    for(i=0;i<26;i++)
    {
        st[69][i]=165;
    }

    st[69][8]=70;

    // State 70

    for(i=0;i<85;i++)
    {
        st[70][i]=166;
    }

    for(i=0;i<26;i++)
    {
        st[70][i]=165;
    }

    st[70][11]=71;

    // State 71

    for(i=0;i<85;i++)
    {
        st[71][i]=166;
    }

    for(i=0;i<26;i++)
    {
        st[71][i]=165;
    }

    st[71][4]=72;

    // State 72

    for(i=0;i<85;i++)
    {
        st[72][i]=73;
    }

    for(i=0;i<26;i++)
    {
        st[72][i]=165;
    }

    // State 73

    for(i=0;i<85;i++)
    {
        st[73][i]=1001;
    }

    // State 74

    for(i=0;i<85;i++)
    {
        st[74][i]=166;
    }

    for(i=0;i<26;i++)
    {
        st[74][i]=165;
    }

    st[74][19]=75;

    // State 75

    for(i=0;i<85;i++)
    {
        st[75][i]=166;
    }

    for(i=0;i<26;i++)
    {
        st[75][i]=165;
    }

    st[75][7]=76;

    // State 76

    for(i=0;i<85;i++)
    {
        st[76][i]=77;
    }

    for(i=0;i<26;i++)
    {
        st[76][i]=165;
    }

    // State 77

    for(i=0;i<85;i++)
    {
        st[77][i]=1001;
    }

    // State 78

    for(i=0;i<85;i++)
    {
        st[78][i]=166;
    }

    for(i=0;i<26;i++)
    {
        st[78][i]=165;
    }

    st[78][8]=79;

    // State 79

    for(i=0;i<85;i++)
    {
        st[79][i]=166;
    }

    for(i=0;i<26;i++)
    {
        st[79][i]=165;
    }

    st[79][19]=80;

    // State 80

    for(i=0;i<85;i++)
    {
        st[80][i]=166;
    }

    for(i=0;i<26;i++)
    {
        st[80][i]=165;
    }

    st[80][4]=81;

    // State 81

    for(i=0;i<85;i++)
    {
        st[81][i]=82;
    }

    for(i=0;i<26;i++)
    {
        st[81][i]=165;
    }

    // State 82

    for(i=0;i<85;i++)
    {
        st[82][i]=1001;
    }

    // State 83

    for(i=0;i<85;i++)
    {
        st[83][i]=166;
    }

    for(i=0;i<26;i++)
    {
        st[83][i]=165;
    }

    st[83][11]=84;
    st[83][13]=88;

    // State 84

    for(i=0;i<85;i++)
    {
        st[84][i]=166;
    }

    for(i=0;i<26;i++)
    {
        st[84][i]=165;
    }

    st[84][18]=85;

    // State 85

    for(i=0;i<85;i++)
    {
        st[85][i]=166;
    }

    for(i=0;i<26;i++)
    {
        st[85][i]=165;
    }

    st[85][4]=86;

    // State 86

    for(i=0;i<85;i++)
    {
        st[86][i]=87;
    }

    for(i=0;i<26;i++)
    {
        st[86][i]=165;
    }

    // State 87

    for(i=0;i<85;i++)
    {
        st[87][i]=1001;
    }

    // State 88

    for(i=0;i<85;i++)
    {
        st[88][i]=166;
    }

    for(i=0;i<26;i++)
    {
        st[88][i]=165;
    }

    st[88][3]=89;

    // State 89

    for(i=0;i<85;i++)
    {
        st[89][i]=93;
    }

    for(i=0;i<26;i++)
    {
        st[89][i]=165;
    }

    st[89][22]=94;
    st[89][8]=90;
    st[89][17]=100;

    // State 90

    for(i=0;i<85;i++)
    {
        st[90][i]=166;
    }

    for(i=0;i<26;i++)
    {
        st[90][i]=165;
    }

    st[90][5]=91;

    // State 91

    for(i=0;i<85;i++)
    {
        st[91][i]=92;
    }

    for(i=0;i<26;i++)
    {
        st[91][i]=165;
    }

    // State 92

    for(i=0;i<85;i++)
    {
        st[92][i]=1001;
    }

    // State 93

    for(i=0;i<85;i++)
    {
        st[93][i]=1001;
    }

    // State 94

    for(i=0;i<85;i++)
    {
        st[94][i]=166;
    }

    for(i=0;i<26;i++)
    {
        st[94][i]=165;
    }

    st[94][7]=95;

    // State 95

    for(i=0;i<85;i++)
    {
        st[95][i]=166;
    }

    for(i=0;i<26;i++)
    {
        st[95][i]=165;
    }

    st[95][8]=96;

    // State 96

    for(i=0;i<85;i++)
    {
        st[96][i]=166;
    }

    for(i=0;i<26;i++)
    {
        st[96][i]=165;
    }

    st[96][11]=97;

    // State 97

    for(i=0;i<85;i++)
    {
        st[97][i]=166;
    }

    for(i=0;i<26;i++)
    {
        st[97][i]=165;
    }

    st[97][4]=98;

    // State 98

    for(i=0;i<85;i++)
    {
        st[98][i]=99;
    }

    for(i=0;i<26;i++)
    {
        st[98][i]=165;
    }

    // State 99

    for(i=0;i<85;i++)
    {
        st[99][i]=1001;
    }

    // State 100

    for(i=0;i<85;i++)
    {
        st[100][i]=166;
    }

    for(i=0;i<26;i++)
    {
        st[100][i]=165;
    }

    st[100][4]=101;

    // State 101

    for(i=0;i<85;i++)
    {
        st[101][i]=166;
    }

    for(i=0;i<26;i++)
    {
        st[101][i]=165;
    }

    st[101][2]=102;

    // State 102

    for(i=0;i<85;i++)
    {
        st[102][i]=166;
    }

    for(i=0;i<26;i++)
    {
        st[102][i]=165;
    }

    st[102][14]=103;

    // State 103

    for(i=0;i<85;i++)
    {
        st[103][i]=166;
    }

    for(i=0;i<26;i++)
    {
        st[103][i]=165;
    }

    st[103][17]=104;

    // State 104

    for(i=0;i<85;i++)
    {
        st[104][i]=166;
    }

    for(i=0;i<26;i++)
    {
        st[104][i]=165;
    }

    st[104][3]=105;

    // State 105

    for(i=0;i<85;i++)
    {
        st[105][i]=106;
    }

    for(i=0;i<26;i++)
    {
        st[105][i]=165;
    }

    // State 106

    for(i=0;i<85;i++)
    {
        st[106][i]=1001;
    }

    // State 107

    for(i=0;i<85;i++)
    {
        st[107][i]=166;
    }

    for(i=0;i<26;i++)
    {
        st[107][i]=165;
    }

    st[107][11]=108;

    // State 108

    for(i=0;i<85;i++)
    {
        st[108][i]=166;
    }

    for(i=0;i<26;i++)
    {
        st[108][i]=165;
    }

    st[108][14]=109;

    // State 109

    for(i=0;i<85;i++)
    {
        st[109][i]=166;
    }

    for(i=0;i<26;i++)
    {
        st[109][i]=165;
    }

    st[109][1]=110;

    // State 110

    for(i=0;i<85;i++)
    {
        st[110][i]=166;
    }

    for(i=0;i<26;i++)
    {
        st[110][i]=165;
    }

    st[110][0]=111;

    // State 111

    for(i=0;i<85;i++)
    {
        st[111][i]=166;
    }

    for(i=0;i<26;i++)
    {
        st[111][i]=165;
    }

    st[111][11]=112;

    // State 112

    for(i=0;i<85;i++)
    {
        st[112][i]=113;
    }

    for(i=0;i<26;i++)
    {
        st[112][i]=165;
    }

    // State 113

    for(i=0;i<85;i++)
    {
        st[113][i]=1001;
    }

    // State 114

    for(i=0;i<85;i++)
    {
        st[114][i]=166;
    }

    for(i=0;i<26;i++)
    {
        st[114][i]=165;
    }

    st[114][0]=115;

    // State 115

    for(i=0;i<85;i++)
    {
        st[115][i]=166;
    }

    for(i=0;i<26;i++)
    {
        st[115][i]=165;
    }

    st[115][17]=116;

    // State 116

    for(i=0;i<85;i++)
    {
        st[116][i]=166;
    }

    for(i=0;i<26;i++)
    {
        st[116][i]=165;
    }

    st[116][0]=117;

    // State 117

    for(i=0;i<85;i++)
    {
        st[117][i]=166;
    }

    for(i=0;i<26;i++)
    {
        st[117][i]=165;
    }

    st[117][12]=118;

    // State 118

    for(i=0;i<85;i++)
    {
        st[118][i]=166;
    }

    for(i=0;i<26;i++)
    {
        st[118][i]=165;
    }

    st[118][4]=119;

    // State 119

    for(i=0;i<85;i++)
    {
        st[119][i]=166;
    }

    for(i=0;i<26;i++)
    {
        st[119][i]=165;
    }

    st[119][19]=120;

    // State 120

    for(i=0;i<85;i++)
    {
        st[120][i]=166;
    }

    for(i=0;i<26;i++)
    {
        st[120][i]=165;
    }

    st[120][4]=121;

    // State 121

    for(i=0;i<85;i++)
    {
        st[121][i]=166;
    }

    for(i=0;i<26;i++)
    {
        st[121][i]=165;
    }

    st[121][17]=122;

    // State 122

    for(i=0;i<85;i++)
    {
        st[122][i]=125;
    }

    for(i=0;i<26;i++)
    {
        st[122][i]=165;
    }

    st[122][18]=123;

    // State 123

    for(i=0;i<85;i++)
    {
        st[123][i]=124;
    }

    for(i=0;i<26;i++)
    {
        st[123][i]=165;
    }

    // State 124

    for(i=0;i<85;i++)
    {
        st[124][i]=1001;
    }

    // State 125

    for(i=0;i<85;i++)
    {
        st[125][i]=1001;
    }

    // State 126

    for(i=0;i<85;i++)
    {
        st[126][i]=166;
    }

    for(i=0;i<26;i++)
    {
        st[126][i]=165;
    }

    st[126][4]=127;

    // State 127

    for(i=0;i<85;i++)
    {
        st[127][i]=166;
    }

    for(i=0;i<26;i++)
    {
        st[127][i]=165;
    }

    st[127][19]=128;
    st[127][0]=138;
    st[127][2]=133;

    // State 128

    for(i=0;i<85;i++)
    {
        st[128][i]=166;
    }

    for(i=0;i<26;i++)
    {
        st[128][i]=165;
    }

    st[128][20]=129;

    // State 129

    for(i=0;i<85;i++)
    {
        st[129][i]=166;
    }

    for(i=0;i<26;i++)
    {
        st[129][i]=165;
    }

    st[129][17]=130;

    // State 130

    for(i=0;i<85;i++)
    {
        st[130][i]=166;
    }

    for(i=0;i<26;i++)
    {
        st[130][i]=165;
    }

    st[130][13]=131;

    // State 131

    for(i=0;i<85;i++)
    {
        st[131][i]=132;
    }

    for(i=0;i<26;i++)
    {
        st[131][i]=165;
    }

    // State 132

    for(i=0;i<85;i++)
    {
        st[132][i]=1001;
    }

    // State 133

    for(i=0;i<85;i++)
    {
        st[133][i]=166;
    }

    for(i=0;i<26;i++)
    {
        st[133][i]=165;
    }

    st[133][14]=134;

    // State 134

    for(i=0;i<85;i++)
    {
        st[134][i]=166;
    }

    for(i=0;i<26;i++)
    {
        st[134][i]=165;
    }

    st[134][17]=135;

    // State 135

    for(i=0;i<85;i++)
    {
        st[135][i]=166;
    }

    for(i=0;i<26;i++)
    {
        st[135][i]=165;
    }

    st[135][3]=136;

    // State 136

    for(i=0;i<85;i++)
    {
        st[136][i]=137;
    }

    for(i=0;i<26;i++)
    {
        st[136][i]=165;
    }

    // State 137

    for(i=0;i<85;i++)
    {
        st[137][i]=1001;
    }

    // State 138

    for(i=0;i<85;i++)
    {
        st[138][i]=166;
    }

    for(i=0;i<26;i++)
    {
        st[138][i]=165;
    }

    st[138][3]=139;
    st[138][11]=141;

    // State 139

    for(i=0;i<85;i++)
    {
        st[139][i]=140;
    }

    for(i=0;i<26;i++)
    {
        st[139][i]=165;
    }

    // State 140

    for(i=0;i<85;i++)
    {
        st[140][i]=1001;
    }

    // State 141

    for(i=0;i<85;i++)
    {
        st[141][i]=142;
    }

    for(i=0;i<26;i++)
    {
        st[141][i]=165;
    }

    // State 142

    for(i=0;i<85;i++)
    {
        st[142][i]=1001;
    }

    // State 143

    for(i=0;i<85;i++)
    {
        st[143][i]=166;
    }

    for(i=0;i<26;i++)
    {
        st[143][i]=165;
    }

    st[143][13]=144;
    st[143][5]=151;

    // State 144

    for(i=0;i<85;i++)
    {
        st[144][i]=166;
    }

    for(i=0;i<26;i++)
    {
        st[144][i]=165;
    }

    st[144][15]=145;
    st[144][19]=149;

    // State 145

    for(i=0;i<85;i++)
    {
        st[145][i]=166;
    }

    for(i=0;i<26;i++)
    {
        st[145][i]=165;
    }

    st[145][20]=146;

    // State 146

    for(i=0;i<85;i++)
    {
        st[146][i]=166;
    }

    for(i=0;i<26;i++)
    {
        st[146][i]=165;
    }

    st[146][19]=147;

    // State 147

    for(i=0;i<85;i++)
    {
        st[147][i]=148;
    }

    for(i=0;i<26;i++)
    {
        st[147][i]=165;
    }

    // State 148

    for(i=0;i<85;i++)
    {
        st[148][i]=1001;
    }

    // State 149

    for(i=0;i<85;i++)
    {
        st[149][i]=150;
    }

    for(i=0;i<26;i++)
    {
        st[149][i]=165;
    }

    // State 150

    for(i=0;i<85;i++)
    {
        st[150][i]=1001;
    }

    // State 151

    for(i=0;i<85;i++)
    {
        st[151][i]=152;
    }

    for(i=0;i<26;i++)
    {
        st[151][i]=165;
    }

    // State 152

    for(i=0;i<85;i++)
    {
        st[152][i]=1001;
    }

    // State 153

    for(i=0;i<85;i++)
    {
        st[153][i]=166;
    }

    for(i=0;i<26;i++)
    {
        st[153][i]=165;
    }

    st[153][20]=154;

    // State 154

    for(i=0;i<85;i++)
    {
        st[154][i]=166;
    }

    for(i=0;i<26;i++)
    {
        st[154][i]=165;
    }

    st[154][19]=155;

    // State 155

    for(i=0;i<85;i++)
    {
        st[155][i]=166;
    }

    for(i=0;i<26;i++)
    {
        st[155][i]=165;
    }

    st[155][15]=156;

    // State 156

    for(i=0;i<85;i++)
    {
        st[156][i]=166;
    }

    for(i=0;i<26;i++)
    {
        st[156][i]=165;
    }

    st[156][20]=157;

    // State 157

    for(i=0;i<85;i++)
    {
        st[157][i]=166;
    }

    for(i=0;i<26;i++)
    {
        st[157][i]=165;
    }

    st[157][19]=158;

    // State 158

    for(i=0;i<85;i++)
    {
        st[158][i]=159;
    }

    for(i=0;i<26;i++)
    {
        st[158][i]=165;
    }

    // State 159

    for(i=0;i<85;i++)
    {
        st[159][i]=1001;
    }

    // State 160

    for(i=0;i<85;i++)
    {
        st[160][i]=166;
    }

    for(i=0;i<26;i++)
    {
        st[160][i]=165;
    }

    st[160][8]=161;

    // State 161

    for(i=0;i<85;i++)
    {
        st[161][i]=166;
    }

    for(i=0;i<26;i++)
    {
        st[161][i]=165;
    }

    st[161][18]=162;

    // State 162

    for(i=0;i<85;i++)
    {
        st[162][i]=166;
    }

    for(i=0;i<26;i++)
    {
        st[162][i]=165;
    }

    st[162][19]=163;

    // State 163

    for(i=0;i<85;i++)
    {
        st[163][i]=164;
    }

    for(i=0;i<26;i++)
    {
        st[163][i]=165;
    }

    // State 164

    for(i=0;i<85;i++)
    {
        st[164][i]=1001;
    }

    // State 165

    for(i=0;i<85;i++)
    {
        st[165][i]=166;
    }

    for(i=0;i<26;i++)
    {
        st[165][i]=165;
    }

    // State 166

    for(i=0;i<85;i++)
    {
        st[166][i]=1001;
    }

    // State 167

    for(i=0;i<85;i++)
    {
        st[167][i]=1001;
    }

    // State 168

    for(i=0;i<85;i++)
    {
        st[168][i]=1000;
    }

    // End of Symbol Table
}

void ascii()
{
    int i;

    for(i=0;i<128;i++)
    {
        asc[i]=84;
    }

    for(i=0;i<32;i++)
    {
        asc[i]=84;
    }
    asc[127]=84;

    for(i=97;i<123;i++)
    {
        asc[i]=(i-97);
    }

    for(i=65;i<91;i++)
    {
        asc[i]=(i-39);
    }

    for(i=48;i<58;i++)
    {
        asc[i]=(i+4);
    }

    asc[45]=62;
    asc[33]=63;
    asc[35]=64;
    asc[37]=65;
    asc[38]=66;
    asc[40]=67;
    asc[41]=68;
    asc[42]=69;
    asc[46]=70;
    asc[47]=71;
    asc[58]=72;
    asc[59]=73;
    asc[64]=74;
    asc[91]=75;
    asc[93]=76;
    asc[95]=77;
    asc[126]=78;
    asc[43]=79;
    asc[60]=80;
    asc[61]=81;
    asc[62]=82;
    asc[44]=83;
}

void print_tables()
{
    int i,j;

    FILE *fp;
    fp=fopen("DFA_Table.txt","w");

    fprintf(fp,"DFA Table entries:\n\n");

    for(i=0;i<170;i++)
    {
        for(j=0;j<85;j++)
        {
            fprintf(fp,"%d ",st[i][j]);
        }
        fprintf(fp,"\n");
    }

    fprintf(fp,"\n\nASCII mappings:\n\n");

    for(i=0;i<128;i++)
    {
        fprintf(fp,"%c(%d):\t%d\n",(char)i,i,asc[i]);
    }

    fclose(fp);
}

void tokens()
{
    int i;

    for(i=0;i<170;i++)
    {
        strcpy(tk[i],"Error");
    }

    strcpy(tk[4],"TK_ASSIGNOP");
    strcpy(tk[5],"TK_LE");
    strcpy(tk[6],"TK_LT");
    strcpy(tk[8],"TK_GE");
    strcpy(tk[9],"TK_GT");
    strcpy(tk[11],"TK_EQ");
    strcpy(tk[13],"TK_NE");
    strcpy(tk[16],"TK_AND");
    strcpy(tk[19],"TK_OR");
    strcpy(tk[20],"TK_NOT");
    strcpy(tk[21],"TK_OP");
    strcpy(tk[22],"TK_CL");
    strcpy(tk[23],"TK_COMMENT");
    strcpy(tk[24],"TK_SQL");
    strcpy(tk[25],"TK_SQR");
    strcpy(tk[26],"TK_PLUS");
    strcpy(tk[27],"TK_MINUS");
    strcpy(tk[28],"TK_MUL");
    strcpy(tk[29],"TK_DIV");
    strcpy(tk[30],"TK_SEM");
    strcpy(tk[31],"TK_COLON");
    strcpy(tk[32],"TK_DOT");
    strcpy(tk[167],"TK_RECORDID");
    strcpy(tk[38],"TK_NUM");
    strcpy(tk[39],"TK_RNUM");
    strcpy(tk[45],"TK_MAIN");
    strcpy(tk[48],"TK_FUNID");
    strcpy(tk[53],"TK_ID");
    strcpy(tk[58],"TK_CALL");
    strcpy(tk[63],"TK_THEN");
    strcpy(tk[67],"TK_TYPE");
    strcpy(tk[73],"TK_WHILE");
    strcpy(tk[77],"TK_WITH");
    strcpy(tk[82],"TK_WRITE");
    strcpy(tk[87],"TK_ELSE");
    strcpy(tk[92],"TK_ENDIF");
    strcpy(tk[93],"TK_END");
    strcpy(tk[99],"TK_ENDWHILE");
    strcpy(tk[106],"TK_ENDRECORD");
    strcpy(tk[113],"TK_GLOBAL");
    strcpy(tk[124],"TK_PARAMETERS");
    strcpy(tk[125],"TK_PARAMETER");
    strcpy(tk[132],"TK_RETURN");
    strcpy(tk[137],"TK_RECORD");
    strcpy(tk[140],"TK_READ");
    strcpy(tk[142],"TK_REAL");
    strcpy(tk[148],"TK_INPUT");
    strcpy(tk[150],"TK_INT");
    strcpy(tk[152],"TK_IF");
    strcpy(tk[159],"TK_OUTPUT");
    strcpy(tk[164],"TK_LIST");
    strcpy(tk[166],"TK_FIELDID");
    strcpy(tk[168],"TK_COMMA");
}

int tk_class(int a)
{
	switch (a)
	{
		case 4:	a=25;
					break;
		case 5:	a=26;
					break;
		case 6:	a=24;
					break;
		case 8:	a=29;
					break;
		case 9:	a=28;
					break;    
		case 11:	a=27;
					break;
		case 13:	a=1;
					break;
		case 16:	a=4;
					break;
		case 19:	a=13;
					break;    
		case 20:	a=22;
					break;    
    	case 21:	a=5;
					break;
    	case 22:	a=6;
					break;
    	case 23:	a=3;
					break;
    	case 24:	a=19;
					break;
		case 25:	a=14;
					break;
		case 26:	a=23;
					break;
		case 27:	a=0;
					break;
		case 28:	a=7;
					break;
		case 29:	a=9;
					break;
		case 30:	a=11;
					break;
		case 31:	a=10;
					break;			   
		case 32:	a=8;
					break;
		case 167:a=2;
					break;
		case 38:	a=15;
					break;
		case 39:	a=16;
					break;
		case 45:	a=21;
					break;
		case 48:	a=20;
					break;
		case 53:	a=18;
					break;
		case 58:	a=30;
					break;    
    	case 63:	a=48;
					break;
		case 67:	a=49;
					break;
		case 73: a=50;
					break;
		case 77:	a=51;
					break;
		case 82:	a=52;
					break;
		case 87:	a=31;
					break;
    	case 92:	a=33;
					break;
		case 93:	a=32;
					break;
		case 99:	a=35;
					break;
		case 106:a=34;
					break;
		case 113:a=36;
					break;
		case 124:a=43;
					break;
		case 125:a=42;
					break;
		case 132:a=47;
					break;
		case 137:a=46;
					break;
		case 140:a=44;
					break;			
    	case 142:a=45;
					break;
		case 148:a=38;
					break;
		case 150:a=39;
					break;
		case 152:a=37;
					break;
		case 159:a=41;
					break;
		case 164:a=40;
					break;
		case 166:a=17;
					break;
		case 168:a=12;
					break;
		default: a=-1;
					break;
	}
	
	return a;
}
