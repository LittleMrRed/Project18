//夜夜爱消除
#include <graphics.h>
#include <conio.h>
#include <time.h>
#include <stdio.h>
#include <string.h>


//宏定义
#define Width 1100
#define High 850
#define Key_Down(VK_c) GetAsyncKeyState(VK_c) & 0x8000
#define Num 180										//ballArr数组的元素数量


//全局变量
COLORREF color[6] = { LIGHTRED, LIGHTBLUE, WHITE, YELLOW,LIGHTGREEN, LIGHTMAGENTA };//定义颜色数组					
int x, y;														//光标中心位置
typedef	struct position								//光标位置结构体
{
	int x;
	int y;
}typepos;
typepos cur;												//光标当前位置
typepos ballArr[Num];								//将相邻同颜色小球保存在数组中		
int index;													//记录相邻同颜色小球个数
int score;													//记录得分


//函数声明
void startup();														//数据初始化
void show();															//画面显示
void updateWithoutInput();									//与用户输入无关的更新
void updateWithInput();										//与用户输入有关的更新
void gameover();													//游戏结束
void drawcursor(typepos, COLORREF);					//绘制光标函数
void drawtime(int sec);											//倒计时函数
void drawscore(int score);										//计分函数
int isVaild(typepos, COLORREF);							//检验是否合法
void getsamecolorballs(typepos, COLORREF);		//获取相邻同颜色小球函数
void turn();															//数组元素排序(y坐标由小到大)
void balldown();													//小球下落



//主函数
int main()
{
	startup();															//数据初始化
	show();																//显示画面
	updateWithoutInput();										//与用户输入无关的更新·
	updateWithInput();											//与用户输入有关的更新
	gameover();														//游戏结束
	return 0;
}


//数据初始化
void startup()
{
	initgraph(Width, High);							//初始化界面
	srand((unsigned)time(NULL));				//时间种子

	cur.x = 500;
	cur.y = 400;

	index = 0;												//初始化
	score = 0;
}



//画面显示
void show()
{
	//调用函数绘制光标
	drawcursor(cur, WHITE);

	//绘制灰色边框
	setlinecolor(LIGHTGRAY);
	setlinestyle(PS_SOLID, 10);
	rectangle(220, 20, 880, 830);

	//绘制小圆
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


//与用户输入无关的更新
void updateWithoutInput()
{
	//调用倒计时函数
	drawtime(30);

	//调用计分函数
	drawscore(0);
}


//与用户输入有关的更新
void updateWithInput()
{
	for (int i = 2999; i > -1; i--)
	{
		if (i % 100 == 0)
		{
			drawtime(i / 100);
		}
		//移动光标
		char input;
		if (_kbhit())
		{
			input = _getch();
			if (input == 'a' && cur.x - 25 >= 250)
			{
				drawcursor(cur, BLACK);				//擦去原光标
				cur.x -= 50;
				drawcursor(cur, WHITE);;				//绘制新光标
			}
			else if (input == 'd' && cur.x + 25 <= 850)
			{
				drawcursor(cur, BLACK);				//擦去原光标
				cur.x += 50;
				drawcursor(cur, WHITE);				//绘制新光标
			}
			else if (input == 'w' && cur.y - 25 >= 50)
			{
				drawcursor(cur, BLACK);				//擦去原光标
				cur.y -= 50;

				drawcursor(cur, WHITE);				//绘制新光标
			}
			else if (input == 's' && cur.y + 25 <= 800)
			{
				drawcursor(cur, BLACK);				//擦去原光标
				cur.y += 50;
				drawcursor(cur, WHITE);				//绘制新光标
			}
			else if (Key_Down(VK_RETURN) || Key_Down(VK_SPACE))
			{
				//获取光标底下以及相邻同色小球的坐标，并存入数组
				getsamecolorballs(cur, getpixel(cur.x, cur.y));
				//将数组的元素置黑一段时间
				if (index > 1)
				{
					for (int k = 0; k < index; k++)
					{
						setlinecolor(BLACK);
						setfillcolor(BLACK);
						fillcircle(ballArr[k].x, ballArr[k].y, 23);
					}
					//使下落前有个停顿
					Sleep(300);
					//上方小球下落
					balldown();
					//得分
					score += index;
					drawscore(score);
				}
				index = 0;						//重置为0
			}
		}	Sleep(10);
	}	
}


//绘制光标函数
void drawcursor(typepos pos, COLORREF cl)
{
	setlinecolor(cl);
	rectangle(pos.x - 25, pos.y - 25, pos.x + 25, pos.y + 25);
}


//倒计时函数
void drawtime(int sec)
{
	//输出“剩余时间”
	settextcolor(YELLOW);								//设置字体颜色
	settextstyle(25, 0, _T("黑体"));						//设置字体大小风格
	TCHAR s[30];
	_stprintf_s(s, _T("剩余时间：%2ds"), sec);		// 高版本 VC 推荐使用 _stprintf_s 函数
	outtextxy(920, 30, s);
}


//计分函数
void drawscore(int score)
{
	settextcolor(RED);
	settextstyle(30, 0, _T("黑体"));						//设置字体大小风格
	TCHAR f[30];
	_stprintf_s(f, _T("得分：%d"), score);
	outtextxy(920, 730, f);
}



//检验是否合法
int isVaild(typepos cur, COLORREF cl)
{
	if (getpixel(cur.x, cur.y) != cl)						//返回假
		return 0;
	else
	{
		for (int k = 0; k < index; k++)
		{
			if (ballArr[k].x == cur.x && ballArr[k].y == cur.y)
				return 0;										//返回假
		}
		return 1;												//返回真
	}
}



//获取相邻同颜色小球函数
void getsamecolorballs(typepos cur, COLORREF cl)
{
	ballArr[index].x = cur.x;								//将光标下的小球存入数组
	ballArr[index].y = cur.y;
	index++;

	typepos tmpPos;
	for (int i = 0; i < 4; i++)								//遍历相邻小球
	{
		switch (i)
		{
		case 0:tmpPos.x = cur.x; tmpPos.y = cur.y - 50; break;
		case 1:tmpPos.x = cur.x; tmpPos.y = cur.y + 50; break;
		case 2:tmpPos.x = cur.x - 50; tmpPos.y = cur.y; break;
		case 3:tmpPos.x = cur.x + 50; tmpPos.y = cur.y; break;
		}
		if(isVaild(tmpPos, cl))								//如果颜色相同，即返回真，存放入数组
		{
			getsamecolorballs(tmpPos, cl);			//自己调用自己
		}
	}
}



//数组元素排序(y坐标由小到大)
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




//小球下落
void balldown()
{
	//数组元素排序(y坐标由小到大)
	turn();
	for (int k = 0; k < index; k++)								//遍历数组中每一个元素
	{
		for (int j = ballArr[k].y; j > 50; j -= 50)
		{
			COLORREF cl = getpixel(ballArr[k].x, j - 50);	//用变量保存获取的颜色
			setfillcolor(cl);
			fillcircle(ballArr[k].x,  j, 23);								//重绘
		}
		COLORREF cl = color[rand() % 6];						//随机第一个圆的颜色
		setfillcolor(cl);
		fillcircle(ballArr[k].x, 50, 23);
	}
}



//游戏结束
void gameover()
{
	//GAME OVER
	cleardevice();
	settextcolor(RED);
	settextstyle(100, 0, _T("黑体"));						//设置字体大小风格
	TCHAR f[30];
	_stprintf_s(f, _T("GAME OVER"));					//显示gameover
	outtextxy(350, 360, f);
	//分数
	drawscore(score);
	
	system("pause");
	Sleep(2000);
	exit (0);
	closegraph();
}


