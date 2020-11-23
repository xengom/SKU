#include <Turboc.h>
#include<MMSystem.h>              //헤더파일 추가for배경음악
#pragma comment(lib,"Winmm.lib")  //헤더파일 추가for배경음악
#define LEFT 75
#define RIGHT 77
#define UP 72
#define BX 5
#define BY 1
#define BW 10
#define BH 20
#define ESC 27
#define DOWN 80
#define PGUP 73
#define PGDN 81
#define SOL  800
#define DE 7
void DrawScreen();
int start();
BOOL ProcessKey();
BOOL ReverseProcessKey();
void PrintBrick(BOOL Show);
int GetAround(int x, int y, int b, int r);
BOOL MoveDown();
void TestFull();
void DrawBoard();
void PrintInfo();
void bomb();
void Rank();
void DrawRank();
typedef struct{
 char name[9];
 int Score;
}ST;
 
char *arTile[][3]={
 {".","■","□"},
 {" ","■","□"},
 {" ","##","Ⅱ"},
 {" ","●","▣"},
};
int col;
int ttype=0;
int board[BW+2][BH+2];
int nx,ny;
int brick,rot;
int score;
int bricknum;
int GOx, GOy; //게임오버시 이펙트 좌표 
int GOkey; //게임오버시 입력 받는곳
struct Point
{ int x,y; };
void Color(int col)
{SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),col);}
 
Point Shape[][4][4]={
 {{0,0,1,0,2,0,-1,0}, {0,0,0,1,0,-1,0,-2},{0,0,1,0,2,0,-1,0}, {0,0,0,1,0,-1,0,-2}},
 {{0,0,1,0,0,1,1,1}, {0,0,1,0,0,1,1,1},{0,0,1,0,0,1,1,1}, {0,0,1,0,0,1,1,1}},
 {{0,0,-1,0,0,-1,1,-1},{0,0,0,1,-1,0,-1,-1},{0,0,-1,0,0,-1,1,-1},{0,0,0,1,-1,0,-1,-1}},
 {{0,0,-1,-1,0,-1,1,0},{0,0,-1,0,-1,1,0,-1},{0,0,-1,-1,0,-1,1,0},{0,0,-1,0,-1,1,0,-1}},
 {{0,0,-1,0,1,0,-1,-1},{0,0,0,-1,0,1,-1,1},{0,0,-1,0,1,0,1,1},{0,0,0,-1,0,1,1,-1}},
 {{0,0,1,0,-1,0,1,-1},{0,0,0,1,0,-1,-1,-1},{0,0,1,0,-1,0,-1,1},{0,0,0,-1,0,1,1,1}},
 {{0,0,-1,0,1,0,0,1},{0,0,0,-1,0,1,1,0},{0,0,-1,0,1,0,0,-1},{0,0,-1,0,0,-1,0,1}},
 {{0,0,0,0,0,0,0,0,},{0,0,0,0,0,0,0,0,},{0,0,0,0,0,0,0,0,},{0,0,0,0,0,0,0,0,}}
};
enum {EMPTY,BRICK,WALL};
void DrawNext();
int nbrick;
void main()
{
 int x, y;
 int nFrame, nStay;
 start();
 setcursortype(NOCURSOR);
 randomize();
 for(;3;){
  sndPlaySoundA("C:\\123.wav",SND_ASYNC|SND_NODEFAULT);  //배경음악 불러오기 123.wav 음악파일을 C드라이브에 넣으면됨
 clrscr();
 for(x=0;x<BW+2;x++){
  for (y=0;y<BH+2;y++){
   board[x][y] = (y==0 || y==BH+1 || x==0 || x==BW+1) ? WALL:EMPTY;
  }
 }
 DrawScreen();
 nFrame=20;
 score=0;
 bricknum=0;
 nbrick=(random(sizeof(Shape)/sizeof(Shape[0]))%7);
 
 for(;1;){
  bricknum++;
  brick=nbrick;
  if((bricknum%17)>=16){nbrick=DE;}
  else
  {nbrick=(random(sizeof(Shape)/sizeof(Shape[0]))%7);}
  DrawNext();
  nx=BW/2;
  ny=3;
  rot=0;
  if(nbrick==0){col=7;}
  else if(nbrick==1){col=8;}
  else if(nbrick==2){col=9;}
  else if(nbrick==3){col=10;}
  else if(nbrick==4){col=11;}
  else if(nbrick==5){col=12;}
  else if(nbrick==6){col=13;}
  else if(brick==7 || nbrick==7){col=15;}
  Color(col);
  PrintBrick(TRUE);
 
  if(GetAround(nx,ny,brick,rot)!=EMPTY)break;
  nStay=nFrame;
  if((bricknum%40)>20){
   for(;9;){
   if(--nStay == 0){
    nStay=nFrame;
    if (MoveDown()) break;
   }
   if(ReverseProcessKey()) break;
   delay(1000/20);}
  }
  else for(;2;){
   if(--nStay == 0){
    nStay=nFrame;
    if (MoveDown()) break;
   }
   if(ProcessKey()) break;
   delay(1000/20); 
   
   }
  
   if (bricknum %5 ==0 &&nFrame>5){
   nFrame--;
  }
   PrintInfo();
 }
    //게임오버
  GOy=BH+2;
  while(GOy>0)
  {for(GOx=5;GOx<29;GOx+=2)
  {gotoxy(GOx,GOy);
  printf("■");}
  GOy--; Sleep(100);}
    clrscr();
    gotoxy(30,12);puts("G A M E  O V E R");
    gotoxy(25,13);puts("r을 누르면 랭킹저장가능");
    gotoxy(25,14);puts("다시 시작하려면 Y를 누르세요");
    if(tolower(getch())=='y')continue;
 
    else if (tolower(getch())=='r'){Rank();}
    else break;}
    setcursortype(NORMALCURSOR);
}
int start()
{
 int select;
 int titleloop=0, titime;
  gotoxy(6,3);
  printf("■■■■■");
  while(titleloop<6)
   {gotoxy(10,titleloop+4);
   printf("■");
   titleloop++;}
  titleloop=0;
  gotoxy(18,3);
  printf("■■■■■");
  gotoxy(20,6);
  printf("■■■");
  gotoxy(20,9);
  printf("■■■■");
  while(titleloop<6)
   {gotoxy(18,titleloop+4);
   printf("■");
   titleloop++;}
  titleloop=0;
  gotoxy(30,3);
  printf("■■■■■");
  while(titleloop<6)
   {gotoxy(34,titleloop+4);
   printf("■");
   titleloop++;}
  titleloop=0;
  gotoxy(42,3);
  printf("■■■■");
  while(titleloop<6)
   {gotoxy(42,titleloop+4);
   printf("■");
   titleloop++;}
  gotoxy(50,4);
  printf("■");
  gotoxy(50,5);
  printf("■");
  gotoxy(44,6);
  printf("■■■");
  gotoxy(46,7);
  printf("■");
  gotoxy(48,8);
  printf("■");
  gotoxy(50,9);
  printf("■");
  titleloop=0;
  while(titleloop<7)
   {gotoxy(54,titleloop+3);
   printf("■");
   titleloop++;}
  titleloop=0;
  gotoxy(60,3);
  printf("■■■■");
  gotoxy(58,4);
  printf("■");
  gotoxy(58,5);
  printf("■");
  gotoxy(60,6);
  printf("■■■");
  gotoxy(66,7);
  printf("■");
  gotoxy(66,8);
  printf("■");
  gotoxy(58,9);
  printf("■■■■");
  gotoxy(33,11);
  printf("VER. 2.3.5");
  gotoxy(20,13);
  printf("시작하려면 1을 누르고 엔터를 누르시오. ");
  
  //TETRIS 글자 움직이는 이펙트 
  titime=0;
  while(!kbhit())
   {delay(100);
   if(titime==20)titime=0;
   titime++;
   if(titime==5)
    {gotoxy(6,3);
    printf("  ■■■  ");
    gotoxy(24,5);
    printf("■");
    gotoxy(24,6);
    printf("  ");
    gotoxy(34,5);
    printf(" ■");
    gotoxy(34,6);
    printf(" ■");
    gotoxy(34,7);
    printf(" ■");
    gotoxy(50,9);
    printf(" ");
    gotoxy(50,8);
    printf("■");
    gotoxy(54,6);
    printf(" ■");
    gotoxy(58,4);
    printf(" ■");
    gotoxy(58,5);
    printf(" ■");
    gotoxy(65,7);
    printf("■");
    gotoxy(65,8);
    printf("■");
    gotoxy(59,13);}
   else if(titime==10)
    {gotoxy(6,3);
    printf("    ■    ");
    gotoxy(24,5);
    printf("  ");
    gotoxy(24,6);
    printf("■");
    printf("  ");
    gotoxy(34,5);
    printf(" ■");
    gotoxy(34,6);
    printf("  ■");
    gotoxy(34,7);
    printf(" ■");
    gotoxy(50,9);
    printf("■");
    gotoxy(50,8);
    printf(" ");
    gotoxy(54,6);
    printf("■");
    gotoxy(58,4);
    printf("■");
    gotoxy(58,5);
    printf("■");
    gotoxy(66,7);
    printf("■");
    gotoxy(66,8);
    printf("■");
    gotoxy(59,13);}
   else if(titime==15)
    {gotoxy(6,3);
    printf("  ■■■  ");
    gotoxy(24,6);
    printf("  ");
    gotoxy(24,7);
    printf("■");
    printf("  ");
    gotoxy(34,5);
    printf(" ■");
    gotoxy(34,6);
    printf(" ■");
    gotoxy(34,7);
    printf(" ■");
    gotoxy(48,9);
    printf("■");
    gotoxy(50,9);
    printf(" ");
    gotoxy(53,6);
    printf("■");
    gotoxy(58,4);
    printf(" ■");
    gotoxy(58,5);
    printf(" ■");
    gotoxy(65,7);
    printf("■");
    gotoxy(65,8);
    printf("■");
    gotoxy(59,13);}
   else if(titime==20)
    {gotoxy(6,3);
    printf("■■■■■");
    gotoxy(24,6);
    printf("■");
    gotoxy(24,7);
    printf("  ");
    printf("  ");
    gotoxy(34,5);
    printf("■");
    gotoxy(34,6);
    printf("■");
    gotoxy(34,7);
    printf("■");
    gotoxy(50,9);
    printf("■");
    gotoxy(48,9);
    printf(" ");
    gotoxy(54,6);
    printf("■");
    gotoxy(58,4);
    printf("■");
    gotoxy(58,5);
    printf("■");
    gotoxy(66,7);
    printf("■");
    gotoxy(66,8);
    printf("■");
    gotoxy(59,13);}}
 scanf("%d",&select);
 if(select ==1) return 0;
 else {
  gotoxy(25,14);printf("잘못 입력하셨습니다!");
  delay(3000);
  exit(0);
 }
}
void DrawScreen()
{
 int x, y;
 for(x=0;x<BW+2;x++){
  for (y=0;y<BH+2;y++){
   gotoxy(BX+x*2,BY+y);
   puts(arTile[ttype][board[x][y]]);
  }
 }
 gotoxy(50,3);puts("Tetris Ver 2.3.5");
 gotoxy(50,5);puts("좌우:이동, 위:회전, 아래:내림");
 gotoxy(50,6);puts("공백:전부 내림, ESC:종료");
 gotoxy(50,7);puts("P:정지, PgUp,PgDn: 모양바꿈");
 gotoxy(50,8);puts("S:랭킹확인");
 DrawNext();
  Color(15);
 PrintInfo();
}
BOOL ProcessKey()
{
 int ch,trot;
 if(kbhit()) {
  ch=getch();
  if(ch == 0xE0 || ch == 0){
   ch=getch();
  switch (ch){
  case LEFT:
   if(GetAround(nx-1,ny,brick,rot)==EMPTY){
    Color(15);
    PrintBrick(FALSE);
    nx--;
  Color(col);
    PrintBrick(TRUE);
   }
   break;
  case RIGHT:
   if(GetAround(nx+1,ny,brick,rot)==EMPTY){
    Color(15);
    PrintBrick(FALSE);
    nx++;
  Color(col);
    PrintBrick(TRUE);
   }
   break;
  case UP:
   trot=(rot == 3 ? 0:rot+1);
   if(GetAround(nx,ny,brick,trot) == EMPTY){
    Color(15);
    PrintBrick(FALSE);
    rot=trot;
  Color(col);
    PrintBrick(TRUE);
   }
   break;
  case DOWN:
   if(MoveDown()){
    return TRUE;
   }
   break;
  case PGDN:
   ttype++;
   if(ttype == sizeof(arTile)/sizeof(arTile[0])) ttype=0;
   Color(15);
   DrawScreen();
   Color(col);
   PrintBrick(TRUE);
   break;
  case PGUP:
   if(ttype == 0) ttype=sizeof(arTile)/sizeof(arTile[0]);
   ttype--;Color(15);
   DrawScreen();
    Color(col);
   PrintBrick(TRUE);
   break;
  }
 }else{
   switch (ch){
  case ESC:
    clrscr();
    setcursortype(NORMALCURSOR);
    gotoxy(30,12);
    printf("G");delay(500);
    gotoxy(32,12);
    printf("A");delay(500);
    gotoxy(34,12);
    printf("M");delay(500);
    gotoxy(36,12);
    printf("E");delay(500);
    gotoxy(40,12);
    printf("E");delay(500);
    gotoxy(42,12);
    printf("N");delay(500);
    gotoxy(44,12);
    printf("D");delay(500);
    
   
    delay(3000);
    exit(0);
   
  case' ': ;
   while(MoveDown()==FALSE){;}
   return TRUE;
  case 'p':Color(15);
   clrscr();
   gotoxy(15,10);
   puts("Tetris 잠시 중지. 다시 시작하려면 아무 키나 누르세요.");
   getch();
   clrscr();
   Color(15);
   DrawScreen();
    Color(col);
   PrintBrick(TRUE);
   break;
  case 's':Color(15);
   clrscr();
   DrawRank();
   break;
  }
 }
}
 return FALSE;
}
void PrintBrick(BOOL Show)
{
 int i;
 if(brick!=DE)
 {for(i=0;i<4;i++){
  gotoxy(BX+(Shape[brick][rot][i].x+nx)*2, BY+Shape[brick][rot][i].y+ny);
  puts(arTile[ttype][Show ? BRICK:EMPTY]);
 }}
 else {for(i=0;i<4;i++){
  gotoxy(BX+(Shape[brick][rot][i].x+nx)*2, BY+Shape[brick][rot][i].y+ny);
  puts(arTile[ttype][Show ? WALL:EMPTY]);
 }
 }
}
void TestFull()
{
 int i,x,y,ty;
 int count=0;
 static int arScoreInc[]={0,1,3,8,20};
 for(i=0;i<4;i++){
  board[nx+Shape[brick][rot][i].x][ny+Shape[brick][rot][i].y]=BRICK;
 }
  for(y=1;y<BH+1;y++){
   for(x=1;x<BW+1;x++){
    if(board[x][y]!=BRICK)break;
 }
 if(x==BW+1){
  
  count++;
  for(ty=y;ty>1;ty--){
   for(x=1;x<BW+1;x++){
    board[x][ty]=board[x][ty-1];
   }
   score+=arScoreInc[count];
   PrintInfo();
   
  }
  DrawBoard();
  delay(100);
 Beep(SOL,500);
 }
}
 }
int GetAround(int x,int y,int b, int r)
{
 int i,k=EMPTY;
 for(i=0;i<4;i++){
  k=max(k,board[x+Shape[b][r][i].x][y+Shape[b][r][i].y]);
 }
 return k;
}
BOOL MoveDown()
{
if(brick==DE){bomb();}
 if(GetAround(nx,ny+1,brick,rot)!= EMPTY){
  TestFull();
  return TRUE;
 }Color(15);
 PrintBrick(FALSE);
 ny++;
  Color(col);
 PrintBrick(TRUE);
 return FALSE;
}
void bomb()
{
 int y;
 if(GetAround(nx,ny+1,brick,rot)!= EMPTY)
 for(y=2;y<BH+2;y++){
  board[nx][y]=EMPTY; }
 score+=random(15);
 PrintInfo();
}
void DrawBoard()
{
 int x,y;
 for(x=1;x<BW+1;x++){
  for(y=1;y<BH+1;y++){
   gotoxy(BX+x*2,BY+y);
   puts(arTile[ttype][board[x][y]]);
  }
 }
}
void DrawNext()
{
 Color(15);
 int x,y,i;
 
 for(x=50;x<=70;x+=2){
  for(y=12;y<=18;y++){
   gotoxy(x,y);
   puts(arTile[ttype][(x==50 || x==70 || y==12 || y==18)? WALL:EMPTY]);
  }
 }
 for(i=0;i<4;i++)
 {Color(col);
  gotoxy(60+(Shape[nbrick][0][i].x)*2, 15+Shape[nbrick][0][i].y);
 puts(arTile[ttype][BRICK]);
 }
}
void PrintInfo()
{ Color(15);
 gotoxy(50,9);printf("점수 : %d     ",score);
 gotoxy(50,10);printf("벽돌 : %d 개 ", bricknum);
}
BOOL ReverseProcessKey()
{
 int ch,trot;
 if(kbhit()) {
  ch=getch();
  if(ch == 0xE0 || ch == 0){
   ch=getch();
  switch (ch){
  case RIGHT:
   if(GetAround(nx-1,ny,brick,rot)==EMPTY){
    Color(15);
    PrintBrick(FALSE);
    nx--;
  Color(col);
    PrintBrick(TRUE);
   }
   break;
  case LEFT:
   if(GetAround(nx+1,ny,brick,rot)==EMPTY){
    Color(15);
    PrintBrick(FALSE);
    nx++;
  Color(col);
    PrintBrick(TRUE);
   }
   break;
  case UP:
   trot=(rot == 3 ? 0:rot+1);
   if(GetAround(nx,ny,brick,trot) == EMPTY){
    Color(15);
    PrintBrick(FALSE);
    rot=trot;
  Color(col);
    PrintBrick(TRUE);
   }
   break;
  case DOWN:
   if(MoveDown()){
    return TRUE;
   }
   break;
  case PGDN:
   ttype++;
   if(ttype == sizeof(arTile)/sizeof(arTile[0])) ttype=0;
   DrawScreen();
    Color(col);
   PrintBrick(TRUE);
   break;
  case PGUP:
   if(ttype == 0) ttype=sizeof(arTile)/sizeof(arTile[0]);
   ttype--;
   DrawScreen();
   Color(col);
   PrintBrick(TRUE);
   break;
  }
 }else{
   switch (ch){
  case ESC:
    clrscr();
    setcursortype(NORMALCURSOR);
    gotoxy(30,12);
    printf("G");delay(500);
    gotoxy(32,12);
    printf("A");delay(500);
    gotoxy(34,12);
    printf("M");delay(500);
    gotoxy(36,12);
    printf("E");delay(500);
    gotoxy(40,12);
    printf("E");delay(500);
    gotoxy(42,12);
    printf("N");delay(500);
    gotoxy(44,12);
    printf("D");delay(500);
    
   
    delay(3000);
    exit(0);
   
  case' ':
   while(MoveDown()==FALSE){;}
   return TRUE;
  case 'p':
   clrscr();
   gotoxy(15,10);
   puts("Tetris 잠시 중지. 다시 시작하려면 아무 키나 누르세요.");
   getch();
   clrscr();
   DrawScreen();
    Color(col);
   PrintBrick(TRUE);
   break;
  case 's':
  clrscr();
  DrawRank();
  break;
  }
 }
}
 return FALSE;
}
void Rank()     //랭크저장함수
{
 FILE *out;
 char name[9];
 gotoxy(28,9);
 printf("수고하셧습니다.");
 printf("당신의 점수는 %d입니다.", score);
 gotoxy(28,10);
 printf("이름을 입력하세요:");
 scanf("%s", name);
 out=fopen("Rank.txt", "at");
 gotoxy(25,18);
    printf("이름과 점수는 out.txt 파일에 저장되어 집니다.\n");
 
 if(out==NULL) puts("파일을 열 수 없습니다.\n"), exit(1);
 fprintf(out,"%s %d\n", name, score);
 fclose(out);
 if(tolower(getch())!='y')exit(0);
}
void DrawRank()    //랭크 출력하는 함수
{
 FILE *in;
 ST temp;
 int i=0, m, n;
 char back;
 ST people[100];
 in=fopen("Rank.txt", "rt");
 if(in==NULL) puts("파일을 열 수 없습니다."), exit(1);
 
 while(1){
  if(fscanf(in,"%s %d", people[i].name, &people[i].Score)==EOF) break;
  i++;
 }
 fclose(in);
 for(n=0;n<i-1;n++){
  for(m=n+1;m<i;m++){
   if(people[n].Score<people[m].Score)
   {
    temp=people[n];
    people[n]=people[m];
    people[m]=temp;
   }
  }
 }
 
  setcursortype(NORMALCURSOR);
 gotoxy(20,5);
 puts("☆Consol Tetris Ranking☆");
 for(n=0;n<i;n++){
  if(n==10) break;
  gotoxy(20,n+6);
  printf("No.%d:%s 점수:%d\n", n+1, people[n].name, people[n].Score);
 }
 printf("\n");
 gotoxy(40,15);
 puts("다시 게임을 시작하려면 b를 누르세요");
 for(;4;){
  back=getch();
 if(back=='b' || back=='B'){
  clrscr();Color(15);
  DrawScreen();
   Color(col);
  PrintBrick(TRUE);
   setcursortype(NOCURSOR);
   break;}
 else continue;
 }
} 