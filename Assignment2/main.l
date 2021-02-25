%option noyywrap nodefault case-insensitive
%{
	#define len 50
    #include<stdio.h>
	#include <math.h>
	struct stack
	{
		int top;
		int type[len];
		void* elements[len];
	};
	struct etnode
	{
		int type;
		void* ele;
		struct etnode* left,*right;
	};
	struct stack st1,st2;
	void push(struct stack* st1,void* scan_val,int temp);
	void* pop(struct stack* st1);
	int order_of_prec(char* op);
	void operand_func(int scan_val);
	void operator_func(char* scan_val);
	void braces_func(char* scan_val);
	void print(struct stack* st1);
	void create_tree(char* op);
	int calculation(struct etnode* t);
%}

%%

[0-9]+ 		{ printf("Scanned: %s\n",yytext);operand_func(atoi(yytext));print(&st1);}
[\+\-\*\/\@]|um { printf("Scanned: %s\n",yytext);operator_func(yytext);print(&st1);}
[\(\)] 	{ printf("Scanned: %s\n",yytext);braces_func(yytext);print(&st1);}
. 
\n 

%%

void print(struct stack* st1)
{
	printf("Current stack st1: ");
	for(int i=0;i<=st1->top;i++)
	{
		printf("%s ",(char*)st1->elements[i]);
	}
	printf("\n");

}

void push(struct stack* st1,void* scan_val,int temp)//2 for operand 1 for operator
{
	if(st1->top==len-1)
	{
		printf("Stack overflow\n");
		exit(0);
	}
	else
	{
		st1->top++;
		st1->type[st1->top] = temp;
		if(temp==1)
		{
			st1->elements[st1->top] = NULL;
			st1->elements[st1->top] = (void*)malloc(sizeof(char));
			strcpy((char*)st1->elements[st1->top],scan_val);
		}
		else  
		{
			st1->elements[st1->top] = NULL;
			st1->elements[st1->top] = (void*)scan_val;
		}
	}
}

void* pop(struct stack* st1)
{
	return st1->elements[st1->top--];
}

int order_of_prec(char* op)//checking order_of_prec of operator and returning order number
{
	if(strcmp(op,"um")==0)return 1;
	else if(strcmp(op,"@")==0)return 2;
	else if(strcmp(op,"/")==0)return 3;
	else if(strcmp(op,"*")==0)return 4;
	else if(strcmp(op,"+")==0)return 5;
	else if(strcmp(op,"-")==0)return 6;
	else return 10; 
}


void operand_func(int scan_val)//puting in exptree
{
	struct etnode* t;
	t = NULL;
	t = (struct etnode*)malloc(sizeof(struct etnode));
	t->type = 1;
	t->left = NULL;
	t->right = NULL;
	t->ele = NULL;
	t->ele = (int*)malloc(sizeof(int));
	*(int*)(t->ele) = scan_val;
	push(&st2,t,2);
}





void operator_func(char* scan_val)//if scanned>stack,insert in stack else pop stack till then and insert in exp tree,after -push in stack
{
	int a = order_of_prec(scan_val),b,f; 
	f = (int)(strcmp(scan_val,"@")==0);
	while(st1.top!=-1 && (b = order_of_prec((char*)st1.elements[st1.top]))<= a)
	{
		if(f&&b==a)
		break;
		create_tree((char*)pop(&st1));
		free(st1.elements[st1.top+1]);
	}
	push(&st1,scan_val,1);
}
void braces_func(char* scan_val)//if (,push,if ) pop till you find ( and insert in exptree
{
	if(strcmp(scan_val,"(")==0)
	{
		push(&st1,scan_val,1);
	}
	else
	{
		while(st1.top!=-1 && strcmp((char*)st1.elements[st1.top],"(")!=0)
		{
			create_tree((char*)pop(&st1));
			free(st1.elements[st1.top+1]);
		}
		if(st1.top==-1)
		{
			printf("braces error\n");
			exit(0);
		}
		else
		{
			pop(&st1);
		}
	}
}



void create_tree(char* op)
{
	struct etnode* t1,*t2,*t;
	if(strcmp(op,"um")==0)
	{
		t1 = (struct etnode*)pop(&st2);
		t = NULL;
		t = (struct etnode*)malloc(sizeof(struct etnode));
		t->type = 2;
		t->left = t1;
		t->right = NULL;
		t->ele = NULL;
		t->ele = (char*)malloc(sizeof(char));
	}
	else
	{
		t1 = (struct etnode*)pop(&st2);
		t2 = (struct etnode*)pop(&st2);
		t = NULL;
		t = (struct etnode*)malloc(sizeof(struct etnode));
		t->type = 2;
		t->left = t2;
		t->right = t1;
		t->ele = NULL;
		t->ele = (char*)malloc(sizeof(char));
	}
	strcpy((char*)(t->ele),op);
	push(&st2,(void*)t,2);
}





void* inorder(struct etnode* t)
{
	if(t!=NULL)
	{
		if(t->type==1)
		{
			printf("%d ",(*(int*)(t->ele)));
		}
		else
		{
			inorder(t->left);
			printf("%s ",(char*)(t->ele));
			inorder(t->right);
			
		}
	}

}

void postorder(struct etnode* t)//binary tree to postorder
{
	if(t!=NULL)
	{
		if(t->type==1)
		{
			printf("%d ",(*(int*)(t->ele)));
		}
		else
		{
			postorder(t->left);
			postorder(t->right);
			printf("%s ",(char*)(t->ele));
		}
	}
}

int calculation(struct etnode* t)
{
	if(t!=NULL)
	{
		if(t->type==1)
		{
			return *(int*)(t->ele);
		}
		else
		{
			
			int l = calculation(t->left);
			int r = calculation(t->right);

			if(strcmp((char*)t->ele,"um")==0)
			return -l;
			else if(strcmp((char*)t->ele,"@")==0)
			return (int)pow(l,r);

			else if(strcmp((char*)t->ele,"+")==0)
			return (l+r);
			else if(strcmp((char*)t->ele,"-")==0)
			return (l-r);

			else if(strcmp((char*)t->ele,"*")==0)
			return (l*r);
			else if(strcmp((char*)t->ele,"/")==0)
			return (l/r);
			else
			return 0;
		}
	}
	return 0;
}


int main(int argc,char* argv[])
{
	st1.top = -1;
	st2.top = -1;
	if(argc>1)
	{
		yyin = fopen(argv[1],"r");
		if(yyin==NULL)
		{
			perror("error");
			exit(0);
		}
	}
	yylex();
	while(st1.top!=-1)
	{
		create_tree(pop(&st1));
	}
	struct etnode* t = (struct etnode*)pop(&st2);
	printf("The inorder of expression tree is: ");
	inorder(t);
	printf("\n");
	printf("The postfix expression is: ");
	postorder(t);
	printf("\n");
	printf("The net value is: %d\n",calculation(t));
	return 0;
}