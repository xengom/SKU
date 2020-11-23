#include <stdio.h> 
#include <string.h> 
#define MAX_SIZE 100 

typedef struct STACK 
{ 
    int data[MAX_SIZE]; 
    int top; 

}STACK; 


void Init(STACK *p) 
{ 
    p->top=-1; 
} 

int Is_empty(STACK *p) 
{ 
    return (p->top == -1); 
} 

int Is_full(STACK *p) 
{ 
    return (p->top == (MAX_SIZE-1)); 
} 

void Push(STACK *p,int data) 
{ 
    if(Is_full(p)) 
    { 
        printf("������ ��á���ϴ�\n"); return ; 
    } 
    else 
    { 
        p->data[++(p->top)]=data; 
    } 
} 

int Pop(STACK *p) 
{ 
    if(Is_empty(p)) 
    { 
        printf("������ ����ֽ��ϴ�\n");  
        return -1; 
    } 
     
    return p->data[(p->top)--]; 
} 

int Peek(STACK *p) 
{ 
    if(Is_empty(p)) 
    { 
        printf("������ ����ֽ��ϴ�\n");  
        return -1; 
    } 
     
    return p->data[p->top]; 
} 

int Prec(char op) // �������� �켱���� ��ȯ �Լ� 
{ 
    switch(op) 
    { 
        case '(' : case ')' : return 0; 
        case '+' : case '-' : return 1; 
        case '*' : case '/' : return 2; 
    } 
     
    return -1; 
} 

int InfixToPostfix(char *exp) // ����ǥ������� ���� �����ϴ� �Լ�
{ 
    STACK s; 
    char ch,top_op; 
    int i, len=strlen(exp); 
     
    Init(&s); 

    for(i=0;i<len;i++) 
    { 
        ch=exp[i]; 

        switch(ch) 
        { 
            case '+' :    case '-' :    case '*' :    case '/' : 
                while(!Is_empty(&s) && Prec(ch) <= Prec(Peek(&s))) 
                    printf("%c",Pop(&s)); 
                 
                Push(&s,ch); 
                break; 

            case '(' : 
                Push(&s,ch); 
                break; 

            case ')' : 
                top_op=Pop(&s); 

                while(top_op != '(') 
                { 
                    printf("%c",top_op); 
                    top_op=Pop(&s); 
                } 
                break; 

            default : 
                printf("%c",ch); 
                break; 
        } 
    } 

    while(!Is_empty(&s)) 
        printf("%c",Pop(&s)); 

    return 0; 
} 

int Eval(char *exp) // ����ǥ����� ����ϴ� �Լ�
{ 
    STACK s; 
    int i,len=strlen(exp); 
    char ch; 
    int a,b,value; 

    Init(&s); 

    for(i=0;i<len;i++) 
    { 
        ch=exp[i]; 
        if(ch != '+' && ch != '-' && ch != '*' && ch != '/') 
            Push(&s,ch-'0'); // �ǿ����� 
        else 
        { 
            b=Pop(&s); 
            a=Pop(&s); 

            switch(ch) 
            { 
                case '+' :    Push(&s,a+b);    break; 
                case '-' :    Push(&s,a-b);    break; 
                case '*' :    Push(&s,a*b);    break; 
                case '/' :    Push(&s,a/b);    break; 
            } 
        } 
    } 

    return Pop(&s); 
} 

int main() 
{ 
    char string[50]; 

    printf("���� �Է� : "); 
    printf("����ǥ��� ��� : ",gets(string)); 
    InfixToPostfix(string); 
    puts("\n-------------------------------------"); 
    printf("��� ���� �Է�(����ǥ������� �Է�) : "); 
    printf("����ǥ��� ��� ���: %d\n",Eval(gets(string))); 
     
    return 0; 
}