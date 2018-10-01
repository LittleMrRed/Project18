//ҹҹ������
#include <graphics.h>
#include <conio.h>
#include <time.h>
#include <stdio.h>
#include <string.h>


//�궨��
#define Width 1100
#define High 850
#define Key_Down(VK_c) GetAsyncKeyState(VK_c) & 0x8000
#define Num 180										//ballArr�����Ԫ������


//ȫ�ֱ���
COLORREF color[6] = { LIGHTRED, LIGHTBLUE, WHITE, YELLOW,LIGHTGREEN, LIGHTMAGENTA };//������ɫ����					
int x, y;														//�������λ��
typedef	struct position								//���λ�ýṹ��
{
	int x;
	int y;
}typepos;
typepos cur;												//��굱ǰλ��
typepos ballArr[Num];								//������ͬ��ɫС�򱣴���������		
int index;													//��¼����ͬ��ɫС�����
int score;													//��¼�÷�


//��������
void startup();														//���ݳ�ʼ��
void show();															//������ʾ
void updateWithoutInput();									//���û������޹صĸ���
void updateWithInput();										//���û������йصĸ���
void gameover();													//��Ϸ����
void drawcursor(typepos, COLORREF);					//���ƹ�꺯��
void drawtime(int sec);											//����ʱ����
void drawscore(int score);										//�Ʒֺ���
int isVaild(typepos, COLORREF);							//�����Ƿ�Ϸ�
void getsamecolorballs(typepos, COLORREF);		//��ȡ����ͬ��ɫС����
void turn();															//����Ԫ������(y������С����)
void balldown();													//С������



//������
int main()
{
	startup();															//���ݳ�ʼ��
	show();																//��ʾ����
	updateWithoutInput();										//���û������޹صĸ��¡�
	updateWithInput();											//���û������йصĸ���
	gameover();														//��Ϸ����
	return 0;
}


//���ݳ�ʼ��
void startup()
{
	initgraph(Width, High);							//��ʼ������
	srand((unsigned)time(NULL));				//ʱ������

	cur.x = 500;
	cur.y = 400;

	index = 0;												//��ʼ��
	score = 0;
}



//������ʾ
void show()
{
	//���ú������ƹ��
	drawcursor(cur, WHITE);

	//���ƻ�ɫ�߿�
	setlinecolor(LIGHTGRAY);
	setlinestyle(PS_SOLID, 10);
	rectangle(220, 20, 880, 830);

	//����СԲ
	int i, j;
	for (j = 50; j <= 800; j += 50)
		for (i = 250; i <= 850; i += 50)
		{
			COLORREF cl = color[rand() % 6];
			setlinestyle(PS_SOLID, 1);
			setfillcolor(cl);
			fillcircle(i, j, 23);
		}
}


//���û������޹صĸ���
void updateWithoutInput()
{
	//���õ���ʱ����
	drawtime(30);

	//���üƷֺ���
	drawscore(0);
}


//���û������йصĸ���
void updateWithInput()
{
	for (int i = 2999; i > -1; i--)
	{
		if (i % 100 == 0)
		{
			drawtime(i / 100);
		}
		//�ƶ����
		char input;
		if (_kbhit())
		{
			input = _getch();
			if (input == 'a' && cur.x - 25 >= 250)
			{
				drawcursor(cur, BLACK);				//��ȥԭ���
				cur.x -= 50;
				drawcursor(cur, WHITE);;				//�����¹��
			}
			else if (input == 'd' && cur.x + 25 <= 850)
			{
				drawcursor(cur, BLACK);				//��ȥԭ���
				cur.x += 50;
				drawcursor(cur, WHITE);				//�����¹��
			}
			else if (input == 'w' && cur.y - 25 >= 50)
			{
				drawcursor(cur, BLACK);				//��ȥԭ���
				cur.y -= 50;

				drawcursor(cur, WHITE);				//�����¹��
			}
			else if (input == 's' && cur.y + 25 <= 800)
			{
				drawcursor(cur, BLACK);				//��ȥԭ���
				cur.y += 50;
				drawcursor(cur, WHITE);				//�����¹��
			}
			else if (Key_Down(VK_RETURN) || Key_Down(VK_SPACE))
			{
				//��ȡ�������Լ�����ͬɫС������꣬����������
				getsamecolorballs(cur, getpixel(cur.x, cur.y));
				//�������Ԫ���ú�һ��ʱ��
				if (index > 1)
				{
					for (int k = 0; k < index; k++)
					{
						setlinecolor(BLACK);
						setfillcolor(BLACK);
						fillcircle(ballArr[k].x, ballArr[k].y, 23);
					}
					//ʹ����ǰ�и�ͣ��
					Sleep(300);
					//�Ϸ�С������
					balldown();
					//�÷�
					score += index;
					drawscore(score);
				}
				index = 0;						//����Ϊ0
			}
		}	Sleep(10);
	}	
}


//���ƹ�꺯��
void drawcursor(typepos pos, COLORREF cl)
{
	setlinecolor(cl);
	rectangle(pos.x - 25, pos.y - 25, pos.x + 25, pos.y + 25);
}


//����ʱ����
void drawtime(int sec)
{
	//�����ʣ��ʱ�䡱
	settextcolor(YELLOW);								//����������ɫ
	settextstyle(25, 0, _T("����"));						//���������С���
	TCHAR s[30];
	_stprintf_s(s, _T("ʣ��ʱ�䣺%2ds"), sec);		// �߰汾 VC �Ƽ�ʹ�� _stprintf_s ����
	outtextxy(920, 30, s);
}


//�Ʒֺ���
void drawscore(int score)
{
	settextcolor(RED);
	settextstyle(30, 0, _T("����"));						//���������С���
	TCHAR f[30];
	_stprintf_s(f, _T("�÷֣�%d"), score);
	outtextxy(920, 730, f);
}



//�����Ƿ�Ϸ�
int isVaild(typepos cur, COLORREF cl)
{
	if (getpixel(cur.x, cur.y) != cl)						//���ؼ�
		return 0;
	else
	{
		for (int k = 0; k < index; k++)
		{
			if (ballArr[k].x == cur.x && ballArr[k].y == cur.y)
				return 0;										//���ؼ�
		}
		return 1;												//������
	}
}



//��ȡ����ͬ��ɫС����
void getsamecolorballs(typepos cur, COLORREF cl)
{
	ballArr[index].x = cur.x;								//������µ�С���������
	ballArr[index].y = cur.y;
	index++;

	typepos tmpPos;
	for (int i = 0; i < 4; i++)								//��������С��
	{
		switch (i)
		{
		case 0:tmpPos.x = cur.x; tmpPos.y = cur.y - 50; break;
		case 1:tmpPos.x = cur.x; tmpPos.y = cur.y + 50; break;
		case 2:tmpPos.x = cur.x - 50; tmpPos.y = cur.y; break;
		case 3:tmpPos.x = cur.x + 50; tmpPos.y = cur.y; break;
		}
		if(isVaild(tmpPos, cl))								//�����ɫ��ͬ���������棬���������
		{
			getsamecolorballs(tmpPos, cl);			//�Լ������Լ�
		}
	}
}



//����Ԫ������(y������С����)
void turn()
{
	typepos temp;
	for (int j = 0; j < index - 1; j++)
		for (int i = 0; i < index -1 - j; i++)
		{
			if (ballArr[i].x > ballArr[i + 1].x)
			{
				temp = ballArr[i];
				ballArr[i] = ballArr[i + 1];
				ballArr[i + 1] = temp;
			}
			else if (ballArr[i].y > ballArr[i + 1].y)
			{
				temp = ballArr[i];
				ballArr[i] = ballArr[i + 1];
				ballArr[i + 1] = temp;
			}
		}
}




//С������
void balldown()
{
	//����Ԫ������(y������С����)
	turn();
	for (int k = 0; k < index; k++)								//����������ÿһ��Ԫ��
	{
		for (int j = ballArr[k].y; j > 50; j -= 50)
		{
			COLORREF cl = getpixel(ballArr[k].x, j - 50);	//�ñ��������ȡ����ɫ
			setfillcolor(cl);
			fillcircle(ballArr[k].x,  j, 23);								//�ػ�
		}
		COLORREF cl = color[rand() % 6];						//�����һ��Բ����ɫ
		setfillcolor(cl);
		fillcircle(ballArr[k].x, 50, 23);
	}
}



//��Ϸ����
void gameover()
{
	//GAME OVER
	cleardevice();
	settextcolor(RED);
	settextstyle(100, 0, _T("����"));						//���������С���
	TCHAR f[30];
	_stprintf_s(f, _T("GAME OVER"));					//��ʾgameover
	outtextxy(350, 360, f);
	//����
	drawscore(score);
	
	system("pause");
	Sleep(2000);
	exit (0);
	closegraph();
}


