//Batch No						38
//Krishna Aditya Gabbita	2009B3A7608P
//Varun Sah						2010A7PS078P

#ifndef DATA1
#define DATA1

typedef struct _id *id;
typedef struct _fn *fn;
typedef struct _st *symt;

symt s;
int jump;
int ts_err;

struct _id
{
	int type; //0=integer 1=real 2=record
	int rec; //set to 1 if a record's field
	int scope;
	int lno;
	int offset;
	int size; //int=2,real=4,record=variable size
	char *id_name;
	char *rname;
	id next;
};

struct _fn
{
	int scope;
	char *fname;
	id in_list;
	id out_list;
	id idlist;
};

struct _st
{
	int fno; //no. of functions
	id glist; //global variable list
	id rec; //type definitions
	fn *flist;
};

#endif
