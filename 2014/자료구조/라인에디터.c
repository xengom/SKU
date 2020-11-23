#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_CHAR_PER_LINE 1000
#define MAX_NAME	256
void warning(char *);   
void error(char *); 

#define FALSE 0
#define TRUE 1

typedef struct {
	char a[MAX_CHAR_PER_LINE];
} element;
typedef struct ListNode { 
	element data;
	struct ListNode *link;   	
} ListNode;
typedef struct {
	ListNode *head;     // 헤드 포인터
	int length;		// 노드의 개수
} ListType;

//
void insert_node(ListNode **phead, ListNode *p, 
					ListNode *new_node) 
{
	if (*phead==NULL) {
        *phead = new_node;
        (*phead)->link = NULL;
    } else if (p == NULL) {
        new_node->link = (*phead)->link;
        (*phead)->link = new_node;
    } else {
        new_node->link = p->link;
        p->link = new_node;
    }
}
//
void remove_node(ListNode **phead, ListNode *p, ListNode *removed) 
{
	if (*phead == NULL)
        return;
    else if (p==NULL){
        (*phead) = (ListNode*) (*phead)->link;
    }else{
        p->link = removed->link;
    }
    free(removed);
}
// 리스트를 초기화한다.
void init(ListType *list)
{
	if (list == NULL)
        return;

    list->length = 0;
    list->head = NULL;
}
// 리스트안에서 pos 위치의 노드를 반환한다.
ListNode *get_node_at(ListType *list, int pos)
{
	if (pos < 0 || pos > list->length)
        return NULL;

    int i;
    ListNode *p = list->head;
    for (i=0; i<pos; i++)
        p = p->link;
    
    return p;
} 
// 리스트의 항목의 개수를 반환한다.
int get_length(ListType *list)
{
	return list->length;
}

// 주어진 위치에 데이터를 삽입한다.
void add(ListType *list, int position, element data) 
{
	if (pos < 0 || pos > list->length)
        return;
    
    ListNode *p = get_node_at(list, pos);
        
    ListNode *new_node = (ListNode*) malloc(sizeof(ListNode));
    new_node->data = data;
    new_node->link = NULL;
    
    insert_node(&(list->head), p, new_node);
    list->length++;
} 
// 리스트의 끝에 데이터를 삽입한다.
void add_last(ListType *list, element data)
{
	add(list, get_length(list), data);
}  
// 리스트의 끝에 데이터를 삽입한다.
void add_first(ListType *list, element data)
{
	add(list, 0, data);
}  
//
int is_empty(ListType *list)
{
	return list->length == 0;
}
// 주어진 위치의 데이터를 삭제한다.
void delete(ListType *list, int pos)
{
	 //LinkedListType은 여기서 검사한다
    if (is_empty(list) || pos > list->length)
        return;
    
    //ListNode는 다음 메서드에서 검사한다
    ListNode *p = get_node_at(list, pos-1);
    remove_node(&(list->head), p, p != NULL ? (ListNode*) p->link : NULL);
    list->length--;
}  
//
element get_entry(ListType *list, int pos)
{
	//    if (is_empty(list) || pos > list->length)
	//        return NULL;
    ListNode *p = get_node_at(list, pos);
    return p->data;
}
//
void clear(ListType *list)
{
	int i;
    for (i=0; i<list->length; i++)  delete(list, i);
}
// 버퍼의 내용을 출력한다. 
void display(ListType *buffer)
{
	int i;
	ListNode *tmp_node;
	tmp_node = buffer->head;

	printf("**************\n");
	for(i=0;i<buffer->length;i++){
		printf("%s",tmp_node->data.a);
		tmp_node = tmp_node->link;
	}
	printf("**************\n");
}
// 
void warning(char *message)
{
   fprintf(stderr,"%s\n",message);
}
//
void error(char *message)
{
	fprintf(stderr,"%s\n",message);
	exit(1);
}
//
void help()
{
   printf("**************\n");
   printf("i: 입력\n");
   printf("d: 삭제\n");
   printf("r: 파일읽기\n");
   printf("w: 파일쓰기\n");
   printf("q: 종료\n");
   printf("**************\n");
}

// 디스크 파일로부터 데이터를 읽는다.
void read_file(ListType *buffer)
{
	char fname[MAX_NAME];
	FILE *fd;
	element p;

	if( !is_empty(buffer) ){
		clear(buffer);
	}
	init(buffer);

	printf("파일 이름: ");
	scanf("%s", fname);
	if( (fd = fopen(fname,"r")) == NULL ){
		warning("파일을 열수없습니다.");
		return;
	}
	while(fgets(p.a, MAX_CHAR_PER_LINE, fd)){
	    add_last(buffer, p);
	}
	fclose(fd);
	display(buffer);
}
// 버퍼에 있는 데이터를 디스크 파일에 쓴다.
void write_file(ListType *buffer)
{
	FILE *fd;
	char fname[MAX_NAME];
	element p;
	int i;

	printf("파일 이름: ");
	scanf("%s", fname);
	if( (fd = fopen(fname,"w")) == NULL ){
		printf("파일을 열수없습니다.\n");
		return;
	}
	for(i=0;i<get_length(buffer);i++){
		p = get_entry(buffer, i);
		fputs(p.a, fd);
	}
	fclose(fd);
	display(buffer);
}
// 하나의 라인을 지운다
void delete_line(ListType *buffer)
{
    int position;

    if(is_empty(buffer))
        printf("지울 라인이 없습니다.\n");
    else
    {
        printf("지우고 싶은 라인번호를 입력하세요:\n");
        scanf("%d",&position);
        delete(buffer, position);
	}
	display(buffer);
}
// 하나의 라인을 삽입한다.
void insert_line(ListType *buffer)
{
    int position;
    char line[MAX_CHAR_PER_LINE];
	element p;
    
    printf("입력행번호를 입력하세요: \n");
    scanf("%d", &position);
    
    printf("내용을 입력하세요: ");
	fflush(stdin);
    fgets(line, MAX_CHAR_PER_LINE, stdin);
    strcpy(p.a, line);
    add(buffer, position, p);
	display(buffer);
}
//
void do_command(ListType *buffer, char command)
{
    switch(command)
    {
        case 'd':                   
            delete_line(buffer);
            break;
        case 'i':                   
            insert_line(buffer);     
            break;
        case 'r':                   
            read_file(buffer);     
            break;
        case 'w':                   
            write_file(buffer);     
            break;
        case 'q':                    
            break;
    }
}
// 라인 에디터 메인 프로그램
void main()
{
    char command;
    ListType buffer;

    init(&buffer);
    do{
		help();
       	command = getchar();
        do_command(&buffer, command);
		fflush(stdin);
    } while(command != 'q');
}
