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
        printf("스택이 꽉찼습니다\n"); return ; 
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
        printf("스택이 비어있습니다\n");  
        return -1; 
    } 
     
    return p->data[(p->top)--]; 
} 

int Peek(STACK *p) 
{ 
    if(Is_empty(p)) 
    { 
        printf("스택이 비어있습니다\n");  
        return -1; 
    } 
     
    return p->data[p->top]; 
} 

int Prec(char op) // 연산자의 우선순위 반환 함수 
{ 
    switch(op) 
    { 
        case '(' : case ')' : return 0; 
        case '+' : case '-' : return 1; 
        case '*' : case '/' : return 2; 
    } 
     
    return -1; 
} 

int InfixToPostfix(char *exp) // 후위표기식으로 식을 변경하는 함수
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

int Eval(char *exp) // 후위표기식을 계산하는 함수
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
            Push(&s,ch-'0'); // 피연산자 
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

    printf("수식 입력 : "); 
    printf("후위표기식 출력 : ",gets(string)); 
    InfixToPostfix(string); 
    puts("\n-------------------------------------"); 
    printf("계산 수식 입력(후위표기식으로 입력) : "); 
    printf("후위표기식 계산 결과: %d\n",Eval(gets(string))); 
     
    return 0; 
}