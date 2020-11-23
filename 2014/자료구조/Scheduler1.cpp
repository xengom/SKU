#include <stdio.h>
#include <string.h> 
#include <stdlib.h>

#define MAX_LIST_SIZE 100 //����Ʈũ�� ��ũ��

typedef struct ListNode{ // �������� date, time, data�� ���ڿ��� ������, �ڱ����� ����ü
	char date[MAX_LIST_SIZE];
	char time[MAX_LIST_SIZE];
	char data[MAX_LIST_SIZE];
	struct ListNode *link;
}ListNode;

typedef struct{ 
	ListNode *head; //���������
	int length;		//����� ����
}ListType;  //p151 ����Ʈ ADT�� ���� ����

void init(ListType *list)//����Ʈ �ʱ�ȭ
{
	if(list==NULL) return;//�̹�NULL�̸� �׳� ��ȯ
	list->length =0; //length���� 0���� ����(��尹���ʱ�ȭ)
	list->head = NULL;
}

int is_empty(ListType *list)//����Ʈ�� ����� Ȯ��
{
	if(list->head == NULL)//head�� NULL�� ����Ű�� �̹� ����ִ� ���̹Ƿ� 1�� ��ȯ
		return 1;
	else //�׿ܴ� 0��ȯ
		return 0; 
}

void error(char *message)//����ó���Լ�
{
	fprintf(stderr,"%s\n",message);
	exit(1);
}

ListNode *get_node_at(ListType *list, int pos) //pos��ġ�� ��带 �Ͻ������� tmp_node�� �����ϰ� ��ȯ
{
	int i;
	ListNode *tmp_node=list->head;
	if(pos<0) return NULL;
	for(i=0;i<pos;i++)
		tmp_node=tmp_node->link;
	return tmp_node;
}//p152 ���α׷� 4.22

void insert_node(ListNode **phead, ListNode *p, ListNode *new_node) //��� ���԰���
{
	if(new_node==(ListNode *)NULL)
	{
		error("�߸��Է��ϼ̽��ϴ�.");
	}
	if(*phead==NULL) // ���鸮��Ʈ�� ���(�����Ϸ��� ��尡 ù��° ���)
	{
		new_node->link=NULL;
		*phead = new_node;
	}
	else if(p==NULL) // �����带 ����Ű�� ������ p�� NULL�̸� ����Ʈ �� �տ� ����
	{
		new_node->link=*phead;
		*phead = new_node;
	}
	else //p ������ ����
	{
		new_node->link=p->link;
		p->link=new_node;
	}
}//���� ���� 127p ���α׷�4.5

void add(ListType *list, int position, ListNode *data1) // �־��� ��ġ�� ��� �߰�
{
	ListNode *p;
	if((position>=0)&&(position <=list->length))
	{
		ListNode *node = (ListNode *)malloc(sizeof(ListNode)); //�����޸��Ҵ�
		if(node==(ListNode *)NULL)
		{
			error("�޸� �Ҵ翡��");
		}
		node=data1;
		p=get_node_at(list, position-1); 
		insert_node(&(list->head), p, node);
		list->length++;
	}
}//p153 ���α׷� 4.22 ����

void remove_node(ListNode **phead, ListNode *p, ListNode *removed) // ��� ���� ����
{
	if(p==NULL)
		*phead = (*phead)->link;//p�� NULL�̸� ù��° ��� ���� ��������� ���� removed��尡 �����ϰ� �ִ� ���� ��ȯ
	else
		p->link = removed->link;//p�� removed ���� ��� ����Ű�� ����
	free(removed); //removed�� �����ϴ� ������ �ý��ۿ� ��ȯ
}//p128 ���α׷� 4.6 ����

void is_in_list(ListType *list, char a[]) // char a���� ����Ʈ�� ����� date���� ������ ���� ���
{
	ListNode *p;
	p=list->head;
	while(p!=NULL)
	{
		if(strcmp(p->date, a)==0) // p->date�� ��(��¥)�� a���� ������
		{
			printf("%s %s %s\n", p->date, p->time, p->data); //���� ���� ������ �� ���س���
			p=p->link;
		}
		else
			p=p->link;
	}
}
//p155 ���α׷� 4.27 ����
void delete_(ListType *list, int pos) //�־��� ��ġ�� ��� ����
{
	if(pos<0 || pos >= list->length)
	{
		error("��ġ ����");
	}
	else if(!is_empty(list) && (pos>=0) && (pos <= list->length)) // ����Ʈ�� ������� �ʰ� pos�� ��ü��尳�� ���� �۰� 0���� Ŀ���Ѵ�.
	{
		ListNode *p = get_node_at(list, pos-1); //pos-1��°�� ��带 p�� ����
		remove_node(&(list->head),p, (p!=NULL)?p->link : NULL);
 // p�� NULL�� �ƴϸ� p->link, p�� NULL�̸� NULL
		list->length--;
	}
}//p153 ���α׷� 4.23 ����

void clear(ListType *list) //���� ó���� �ִ� ���κ��� ��� ����
{
	int i;
	for(i=0;i<list->length;i=0) 
		delete_(list, i);
}//p155 ���α׷� 4.25�� ����

ListNode *create_node(ListNode *link) // �� ������带 ������ �� ���� �޾Ƽ� �����Ͽ� ��带 ��ȯ�Ѵ�.
{
	ListNode *new_node;
	new_node = (ListNode *)malloc(sizeof(ListNode));
	if(new_node==NULL)
	{
		error("�� ��尡NULL��");
	}
	else
	{
		printf("year/month/day hour:min memo : >>");
		scanf("%s %s %s", new_node->date, new_node->time, new_node->data);
		new_node->link = link;
	}
	return (new_node);

}

void display(ListType *list) //����Ʈ ����� ��ü���� �� ǥ��
{
	int i;
	ListNode *node=list->head; // ����Ʈ ���� ���� ����ּҸ� node�� ����
	for(i=0;i<list->length;i++) // ����Ʈ ����� ���������� for�� ����
	{
		printf("(%d) %s %s %s\n", i, node->date, node->time, node->data);
		node=node->link; //������� ����
	}
}//p155 ���α׷� 4.26 ����

int main(void)
{
	ListType list1; //list1�̶�� ����Ʈ�� ������ �� ���⿡ ��带 �߰��Ѵ�
	init(&list1);
	printf("\t\t������ ���� ���α׷�");
	printf("\n\n������ ���Ͻø� h�� �����ּ���\n");
	for(;1;)
	{
		char k1=0;
		printf(">>");
		scanf("%s", &k1);
		if(k1=='h')
		{
			printf("i : ������ �Է�\n");
			printf("d : ������ ����\n");
			printf("D : ��� ������ ����\n");
			printf("p : ������ Ȯ��\n");
			printf("P : ��� ������ Ȯ��\n");
			printf("m : ������ ����\n");
			printf("q : ���α׷� ����\n");
			printf("h : ����\n");
		}
		else if(k1=='i')
		{
			add(&list1, 0, create_node(NULL)); //����Ʈ�� ó���� ���ο� ��� ����
		}
		else if(k1=='d')
		{
			int n;
			display(&list1);
			printf("�����Ͻ� ����Ʈ�� ��ȣ�� �Է��ϼ���\n");
			scanf("%d", &n);
			delete_(&list1, n);
			printf("%d���� �����Ǿ����ϴ�.\n", n);
			display(&list1); 
		}
		else if(k1=='D')
		{
			char k2;
			printf("������ ��� �����Ͻðڽ��ϱ�?(y/n)\n");
			scanf("%s",&k2);
			if(k2=='y' || k2=='Y')
			{
				clear(&list1); //y, Y�� ������ ��� ������ ����
				printf("��� �����Ͱ� �����Ǿ����ϴ�.\n");
			}
			else if(k2=='n' || k2=='N')
			{
				printf("������ ����մϴ�. \n");
			}
		}
		else if(k1=='p')
		{
			char a[100]; // ����Ʈ����� date�� ������ ���ڿ� ����
			printf("Ȯ���� ������ ��¥�� �Է��Ͻÿ� \nyear/month/day>>");
			scanf("%s", a);
			is_in_list(&list1, a);
		} 
else if(k1=='P')
		{
			display(&list1); //��� ����� ���� �����ش�
		}

		else if(k1=='m') // get_node_at�� x��°�� ��带 ��ȯ
		{
			int x;
			printf("����Ʈ ��ȣ�� �Է��ϼ���\n");
			display(&list1);
			scanf("\n%d", &x); //����Ʈ�� ��ȣ�� x�� �Է¹���
			printf("������ : %s %s %s\n", get_node_at(&list1, x)->date, get_node_at(&list1, x)->time, get_node_at(&list1, x)->data);
			delete_(&list1, x);
			add(&list1, x, create_node(NULL));
			printf("������ : ");
			printf("%s %s %s\n", get_node_at(&list1, x)->date, get_node_at(&list1, x)->time, get_node_at(&list1, x)->data);
		} 
else if(k1=='q')
		{
			exit(1);
		}

		else
			printf("�߸��Է��ϼ̽��ϴ�.\n");
	}
	return 0;
}
