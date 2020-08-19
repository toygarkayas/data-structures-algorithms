#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 100
int number=0;

typedef struct{
	int item[MAX];
	int top;
}STACK;

int isFull(STACK *s)
{
	if(s->top==MAX)
		return 1;
	return 0;
}

int isEmpty(STACK *s)
{
	if(s->top==0)
		return 1;
	return 0;
}

int push(int x,STACK *s)
{
	if(isFull(s))
		return 0;
	else
	{
		s->item[s->top]=x;
		s->top++;
		return 1;
	}
}

int peak(STACK *s,int *x)
{

	int adr;
	if(isEmpty(s))
		return 0;
	else
	{
		adr=s->top-1;
		*x=s->item[adr];
		return 1;	
	}	
}

int pop(STACK *s, int *x)
{
	if(isEmpty(s))
		return 0;
	else
	{
		s->top--;
		*x=s->item[s->top];
		return 1;
	}
}

int hesapla(int x,int y,int islem)
{
	switch((char)islem)
	{
		case '+': return y+x;
		case '-': return y-x;
		case '/': return y/x;	
		case '*': return y*x;	
		default : return 0;	
	}
}

void durum(STACK *a,STACK *b)
{
	int a1,a2,ptr;
	if(pop(a,&a1) && pop(a,&a2) && pop(b,&ptr))
	{
		push(hesapla(a1,a2,ptr),a);
	}
}

char kontrol(char x,int ptr)
{
	switch(x){
		case '+' :	case '-': return (char)ptr!='(';
		case '*' :  case '/': return (char)ptr=='/' || (char)ptr=='*';
		default  : return 0;
	}
}

void sayilar(char *c,int i,STACK *a)
{
	number*=10;
	number+=c[i]-'0';	
	if(c[i+1]<'0' || c[i+1]>'9')
	{
		push(number,a);
		number=0;
	}
}

void yazdir(STACK *a,STACK *b)
{
	int x;
	printf("\nSayi Yigini : ");
	for(x=0;x<a->top;x++)
		printf("%d  ",a->item[x]);
	printf("\nIslem Yigini: ");
	for(x=0;x<b->top;x++)
		printf("%c  ",b->item[x]);
	printf("\n");
}
int main() {
	int i=0,ptr;
	STACK *a,*b;
	a=(STACK *)calloc(1,sizeof(STACK));
	b=(STACK *)calloc(1,sizeof(STACK));
	char ch[MAX];
	printf("Yapilacak islemi giriniz.\n");
	scanf("%s",ch);	
	while(ch[i]!='\0')
	{
		switch(ch[i]){
			case('('):
				b->item[b->top]='(';
				b->top++;
			break;
			
			case(')'):
				while(peak(b,&ptr) && ptr!='(')
				{
					durum(a,b);
				} 
				pop(b,&ptr);
			break;
			
			case '+':case '-':case '/':case '*':
				if(peak(b,&ptr))
				{
					if(kontrol(ch[i],ptr))
					{
						durum(a,b);
					}
				}
				push((int)ch[i],b);
			break;
			
			default:
				sayilar(ch,i,a);
		}
		yazdir(a,b);
		i++;
	}

	int a1,a2;
	while(!isEmpty(a) && !isEmpty(b))
	{
		if(pop(a,&a1) && pop(a,&a2) && pop(b,&ptr))
			{
				push(hesapla(a1,a2,ptr),a);
				yazdir(a,b);
			}
	}
	peak(a,&ptr);
	printf("\nSONUC:%d\n",ptr);
	return 0;
}
