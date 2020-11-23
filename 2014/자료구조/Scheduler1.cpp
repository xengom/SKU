#include <stdio.h>
#include <string.h> 
#include <stdlib.h>

#define MAX_LIST_SIZE 100 //리스트크기 매크로

typedef struct ListNode{ // 스케줄의 date, time, data를 문자열로 저장함, 자기참조 구조체
	char date[MAX_LIST_SIZE];
	char time[MAX_LIST_SIZE];
	char data[MAX_LIST_SIZE];
	struct ListNode *link;
}ListNode;

typedef struct{ 
	ListNode *head; //헤드포인터
	int length;		//노드의 개수
}ListType;  //p151 리스트 ADT의 구현 참조

void init(ListType *list)//리스트 초기화
{
	if(list==NULL) return;//이미NULL이면 그냥 반환
	list->length =0; //length값을 0으로 만듬(노드갯수초기화)
	list->head = NULL;
}

int is_empty(ListType *list)//리스트가 비었나 확인
{
	if(list->head == NULL)//head가 NULL을 가리키면 이미 비어있는 것이므로 1을 반환
		return 1;
	else //그외는 0반환
		return 0; 
}

void error(char *message)//오류처리함수
{
	fprintf(stderr,"%s\n",message);
	exit(1);
}

ListNode *get_node_at(ListType *list, int pos) //pos위치의 노드를 일시적으로 tmp_node에 저장하고 반환
{
	int i;
	ListNode *tmp_node=list->head;
	if(pos<0) return NULL;
	for(i=0;i<pos;i++)
		tmp_node=tmp_node->link;
	return tmp_node;
}//p152 프로그램 4.22

void insert_node(ListNode **phead, ListNode *p, ListNode *new_node) //노드 삽입과정
{
	if(new_node==(ListNode *)NULL)
	{
		error("잘못입력하셨습니다.");
	}
	if(*phead==NULL) // 공백리스트인 경우(삽입하려는 노드가 첫번째 노드)
	{
		new_node->link=NULL;
		*phead = new_node;
	}
	else if(p==NULL) // 선행노드를 가리키는 포인터 p가 NULL이면 리스트 맨 앞에 삽입
	{
		new_node->link=*phead;
		*phead = new_node;
	}
	else //p 다음에 삽입
	{
		new_node->link=p->link;
		p->link=new_node;
	}
}//참고 교재 127p 프로그램4.5

void add(ListType *list, int position, ListNode *data1) // 주어진 위치에 노드 추가
{
	ListNode *p;
	if((position>=0)&&(position <=list->length))
	{
		ListNode *node = (ListNode *)malloc(sizeof(ListNode)); //동적메모리할당
		if(node==(ListNode *)NULL)
		{
			error("메모리 할당에러");
		}
		node=data1;
		p=get_node_at(list, position-1); 
		insert_node(&(list->head), p, node);
		list->length++;
	}
}//p153 프로그램 4.22 참조

void remove_node(ListNode **phead, ListNode *p, ListNode *removed) // 노드 삭제 과정
{
	if(p==NULL)
		*phead = (*phead)->link;//p가 NULL이면 첫번째 노드 삭제 헤드포인터 변경 removed노드가 차지하고 있는 공간 반환
	else
		p->link = removed->link;//p가 removed 다음 노드 가리키게 변경
	free(removed); //removed가 차지하던 공간을 시스템에 반환
}//p128 프로그램 4.6 참조

void is_in_list(ListType *list, char a[]) // char a값과 리스트의 노드의 date값이 같으면 내용 출력
{
	ListNode *p;
	p=list->head;
	while(p!=NULL)
	{
		if(strcmp(p->date, a)==0) // p->date의 값(날짜)과 a값이 같으면
		{
			printf("%s %s %s\n", p->date, p->time, p->data); //같은 날의 정보를 다 토해낸다
			p=p->link;
		}
		else
			p=p->link;
	}
}
//p155 프로그램 4.27 참조
void delete_(ListType *list, int pos) //주어진 위치의 노드 삭제
{
	if(pos<0 || pos >= list->length)
	{
		error("위치 오류");
	}
	else if(!is_empty(list) && (pos>=0) && (pos <= list->length)) // 리스트가 비어있지 않고 pos는 전체노드개수 보다 작고 0보다 커야한다.
	{
		ListNode *p = get_node_at(list, pos-1); //pos-1번째의 노드를 p에 저장
		remove_node(&(list->head),p, (p!=NULL)?p->link : NULL);
 // p가 NULL이 아니면 p->link, p가 NULL이면 NULL
		list->length--;
	}
}//p153 프로그램 4.23 참조

void clear(ListType *list) //제일 처음에 있는 노드부분을 계속 삭제
{
	int i;
	for(i=0;i<list->length;i=0) 
		delete_(list, i);
}//p155 프로그램 4.25를 참조

ListNode *create_node(ListNode *link) // 빈 동적노드를 생성한 후 값을 받아서 저장하여 노드를 반환한다.
{
	ListNode *new_node;
	new_node = (ListNode *)malloc(sizeof(ListNode));
	if(new_node==NULL)
	{
		error("새 노드가NULL임");
	}
	else
	{
		printf("year/month/day hour:min memo : >>");
		scanf("%s %s %s", new_node->date, new_node->time, new_node->data);
		new_node->link = link;
	}
	return (new_node);

}

void display(ListType *list) //리스트 노드의 객체들의 값 표현
{
	int i;
	ListNode *node=list->head; // 리스트 제일 앞의 노드주소를 node에 저장
	for(i=0;i<list->length;i++) // 리스트 노드의 개수전까지 for문 수행
	{
		printf("(%d) %s %s %s\n", i, node->date, node->time, node->data);
		node=node->link; //다음노드 진행
	}
}//p155 프로그램 4.26 참조

int main(void)
{
	ListType list1; //list1이라는 리스트를 생성한 후 여기에 노드를 추가한다
	init(&list1);
	printf("\t\t스케줄 관리 프로그램");
	printf("\n\n도움말을 원하시면 h를 눌러주세요\n");
	for(;1;)
	{
		char k1=0;
		printf(">>");
		scanf("%s", &k1);
		if(k1=='h')
		{
			printf("i : 스케줄 입력\n");
			printf("d : 스케줄 삭제\n");
			printf("D : 모든 스케줄 삭제\n");
			printf("p : 스케줄 확인\n");
			printf("P : 모든 스케줄 확인\n");
			printf("m : 스케줄 수정\n");
			printf("q : 프로그램 종료\n");
			printf("h : 도움말\n");
		}
		else if(k1=='i')
		{
			add(&list1, 0, create_node(NULL)); //리스트의 처음에 새로운 노드 삽입
		}
		else if(k1=='d')
		{
			int n;
			display(&list1);
			printf("삭제하실 리스트의 번호를 입력하세요\n");
			scanf("%d", &n);
			delete_(&list1, n);
			printf("%d번이 삭제되었습니다.\n", n);
			display(&list1); 
		}
		else if(k1=='D')
		{
			char k2;
			printf("정말로 모두 삭제하시겠습니까?(y/n)\n");
			scanf("%s",&k2);
			if(k2=='y' || k2=='Y')
			{
				clear(&list1); //y, Y를 누르면 모든 데이터 삭제
				printf("모든 데이터가 삭제되었습니다.\n");
			}
			else if(k2=='n' || k2=='N')
			{
				printf("삭제를 취소합니다. \n");
			}
		}
		else if(k1=='p')
		{
			char a[100]; // 리스트노드의 date와 동일한 문자열 생성
			printf("확인할 스케줄 날짜를 입력하시오 \nyear/month/day>>");
			scanf("%s", a);
			is_in_list(&list1, a);
		} 
else if(k1=='P')
		{
			display(&list1); //모든 노드의 값을 보여준다
		}

		else if(k1=='m') // get_node_at은 x번째의 노드를 반환
		{
			int x;
			printf("리스트 번호를 입력하세요\n");
			display(&list1);
			scanf("\n%d", &x); //리스트의 번호를 x로 입력받음
			printf("수정전 : %s %s %s\n", get_node_at(&list1, x)->date, get_node_at(&list1, x)->time, get_node_at(&list1, x)->data);
			delete_(&list1, x);
			add(&list1, x, create_node(NULL));
			printf("수정후 : ");
			printf("%s %s %s\n", get_node_at(&list1, x)->date, get_node_at(&list1, x)->time, get_node_at(&list1, x)->data);
		} 
else if(k1=='q')
		{
			exit(1);
		}

		else
			printf("잘못입력하셨습니다.\n");
	}
	return 0;
}
