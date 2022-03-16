/*
������ܿ�����־
1.������Ŀ
2.�����ز�
3.������Ϸ����
	ʵ�ʵĿ�������
	���ڳ�ѧ�߽�����û���������
	ѡ��ͼ�ο������������
	������ܣ��ǻ��ڡ�easyx"ͼ�ο�
	1)������Ϸ����
	2�������Ϸ����
		a.3�ر�����ͬ�ٶ�ͬʱ�ƶ�
		b.ѭ������������ʵ��
	3��ʵ����Ϸ����
		a.���ر�����Դ
		b.��Ⱦ������֪ʶ�����꣩
		�������⣺1.����ͼƬ��png��ʽͼƬ���ֺ�ɫ ʹ�ÿ�Դ���µĽӿ�����͸������
				  2.ͼƬ��������������� ʹ���������ƴ��� BeginBatchDraw()  EndBatchDraw();
 4.ʵ����ҵı���
 5.ʵ����ҵ���Ծ
 6.ʵ�����С�ڹ�
 7.�����ϰ���ṹ����������
 8.ʹ���ϰ���ṹ������³�ʼ��
 9.��װ�����ϰ������ʾ
 10.ʵ����ҵ��¶׼���
 11.ʵ�������ϰ���
 12.ʵ����ײ���
	�ж����������Ƿ��ཻ
 13.�������
 14.��ʾ����
 15.�жϽ�������
*/

#define win_width 1012
#define win_height 396
#include<stdio.h>
#include <graphics.h>
#include "tools.h"
#include <conio.h>
#include<vector>
#define OBSTACLE_COUNT 100
#define WIN_SCORE 50

using namespace std;//���������ռ�
IMAGE imgBgs[3];//����ͼƬ
int bgX[3];//����ͼƬ��x����
int bgSpeed[3] = { 1,2,4 };
IMAGE imgHeros[12];
int heroX;//��ҵ�X����
int heroY;//��ҵ�Y����
int heroIndex;//��ұ���ʱ��ͼƬ֡���
bool heroJump;//��ʾ���������Ծ
int jumpHeight_max;//��Ծ����߸߶�
int heroJumpOff;
bool update;//�Ƿ���Ҫ����ˢ��
int heroBlood;
int score;//����
IMAGE imgSZ[10];
typedef enum {
	TORTOISE,//�ڹ� 0
	LION,//ʨ�� 1
	HOOK1,
	HOOK2,
	HOOK3,
	HOOK4,
	OBSTACLE_TYPE_COUNT//2
}obstacle_type;

vector<vector<IMAGE>> obstacleImgs;//��������ϰ����ͼƬ
IMAGE imgHeroDown[2];
bool heroDown;//����Ƿ������¶�
typedef struct obstacle {
	int type;//�ϰ��������
	int imgIndex;
	int x, y;//�ϰ��������
	int speed;
	int power;
	bool exist;
	bool hited;
	bool passed;

}obstacle_t;

obstacle_t obstacles[OBSTACLE_COUNT];
int lastObsIndex;
//IMAGE imgTortoise;//С�ڹ�
//int torToiseX;//С�ڹ��ˮƽ����
//int torToiseY;//С�ڹ��Y����
//bool tortoiseExist;//��ǰ�����Ƿ���С�ڹ�
void restart() {
	heroBlood = 100;
	score = 0;
	mciSendString("play res/bg.mp3 repeat", 0, 0, 0);
}
void creatObstacle()
{
	int i;
	for (i = 0; i < OBSTACLE_COUNT; i++)
	{
		if (obstacles[i].exist == false)
		{
			break;
		}
	}
	if (i >= OBSTACLE_COUNT) {
		return;
	}
	obstacles[i].exist = true;
	obstacles[i].imgIndex = 0;
	obstacles[i].hited = false;
	obstacles[i].passed = false;
	//obstacles[i].type = (obstacle_type)(rand() % OBSTACLE_TYPE_COUNT);
	obstacles[i].type = (obstacle_type)(rand() % 3);
	if (obstacles[i].type == HOOK1) {
		obstacles[i].type += rand() % 4;
	}
	if (lastObsIndex >= 0 &&
		obstacles[lastObsIndex].type >= HOOK1 &&
		obstacles[lastObsIndex].type <= HOOK4 &&
		obstacles[i].type == LION && obstacles[lastObsIndex].x > (win_width - 500))
	{
		obstacles[i].type = TORTOISE;
	}
	lastObsIndex = i;
	obstacles[i].x = win_width;
	obstacles[i].y = 345 + 5 - obstacleImgs[obstacles[i].type][0].getheight();
	if (obstacles[i].type == TORTOISE)
	{
		obstacles[i].speed = 0;
		obstacles[i].power = 5;//�˺�ֵ
	}
	else if (obstacles[i].type == LION)
	{
		obstacles[i].speed = 4;
		obstacles[i].power = 20;
	}
	else if (obstacles[i].type >=HOOK1 && obstacles[i].type<=HOOK4)
	{
		obstacles[i].speed = 0;
		obstacles[i].power = 20;
		obstacles[i].y = 0;
	}
}
void checkHit()
{
	for (int i = 0; i < OBSTACLE_COUNT; i++)
	{
		if (obstacles[i].exist && obstacles[i].hited == false )
		{
			int a1x, a1y, a2x, a2y;
			int off = 30;
			if (!heroDown)//���¶ף����Ǳ��ܣ���Ծ��
			{
				a1x = heroX + off;
				a1y = heroY + off;
				a2x = heroX + imgHeros[heroIndex].getwidth() - off;
				a2y = heroY + imgHeros[heroIndex].getheight();

			}
			else {
				a1x = heroX + off;
				a1y = 345 - imgHeroDown[heroIndex].getheight();
				a2x = heroX + imgHeroDown[heroIndex].getwidth() - off;
				a2y = 345;
			}
			IMAGE img = obstacleImgs[obstacles[i].type][obstacles[i].imgIndex];
			int b1x = obstacles[i].x + off;
			int b1y = obstacles[i].y + off;
			int b2x = obstacles[i].x + img.getwidth() - off;
			int b2y = obstacles[i].y + img.getheight() - 10;

			//�ж��ཻ
			if (rectIntersect(a1x, a1y, a2x, a2y, b1x, b2y, b2x, b2y))
			{
				heroBlood -= obstacles[i].power;
				playSound("res/hit.mp3");
				obstacles[i].hited = true;
			}
		}
	}
}
//ѭ������Ч��
void fly()
{
	for (int i = 0; i < 3; i++)
	{
		bgX[i] -= bgSpeed[i];
		if (bgX[i] < -win_width) {
			bgX[i] = 0;
		}
	}
	//ʵ����Ծ
	if (heroJump) {
		if (heroY < jumpHeight_max) {
			heroJumpOff = 4;
		}
		heroY += heroJumpOff;
		if (heroY > 345 - imgHeros[0].getheight()) {
			heroJump = false;
			heroJumpOff = -4;
		}
	}
	else if (heroDown)
	{
		static int count = 0;
		int delay[2] = { 8,30};
		count++;
		if (count >= delay[heroIndex]) {
			count = 0;
			heroIndex++;
			if (heroIndex >= 2)
			{
				heroIndex = 0;
				heroDown = false;
			}
		}
	}
	else
	{  //����Ծ
		heroIndex = (heroIndex + 1) % 12;
	}
	//����С�ڹ�
	static int framecount = 0;
	static int enemyFre = 50;//�ϰ���ˢ��Ƶ��
	framecount++;
	if (framecount > enemyFre)
	{
		framecount = 0;
		enemyFre = 50 + rand() % 50;//50-99
		creatObstacle();
		/*if (!tortoiseExist) {
			tortoiseExist = true;
			torToiseX = win_width;
			enemyFre = 100 + rand() % 300;
		}*/
	}
	//if (tortoiseExist)
	//{
	//	torToiseX -= bgSpeed[2];
	//	if (torToiseX < -imgTortoise.getwidth())
	//	{
	//		tortoiseExist = false;
	//	}
	//}
	//���������ϰ�������
	for(int i = 0; i < OBSTACLE_COUNT; i++)
	{
		if (obstacles[i].exist)
		{
			obstacles[i].x -= obstacles[i].speed + bgSpeed[2];
			if (obstacles[i].x < -obstacleImgs[obstacles[i].type][0].getwidth() * 2)
			{
				obstacles[i].exist = false;
			}
			int len = obstacleImgs[obstacles[i].type].size();
			obstacles[i].imgIndex = (obstacles[i].imgIndex + 1) % len;
		}
	}
	//��Һ��ϰ���ġ���ײ��⡱
	checkHit();

}

//��Ⱦ��Ϸ����
void updateBg()
{
	putimagePNG2(bgX[0],0,&imgBgs[0]);
	putimagePNG2(bgX[1], 119, &imgBgs[1]);
	putimagePNG2(bgX[2], 330, &imgBgs[2]);
}
void init()
{
	//������Ϸ����
	initgraph(win_width, win_height);
	//������Ϸ��Դ
	char name[64];
	for (int i = 0; i < 3; i++)
	{
		//"res/bg001.png"  "res/bg002.png"  "res/bg003.png" 
		sprintf(name, "res/bg00%d.png", i + 1);//sprintf  ��·�����뵽name�ַ�����
		loadimage(&imgBgs[i], name);
		bgX[i] = 0;
	}

	//����Hero��ͼƬ֡�ز�
	for (int i = 0; i < 12; i++)
	{
		//"res/hero1.png".... "res/hero2.png"
		sprintf(name, "res/hero%d.png", i + 1);
		loadimage(&imgHeros[i], name);
	}
	//������ҵĳ�ʼλ��
	heroX = win_width * 0.5 - imgHeros[0].getwidth() * 0.5;
	heroY = 345 - imgHeros[0].getheight();
	heroIndex = 0;
	heroJump = false;
	jumpHeight_max= 345 - imgHeros[0].getheight()-120;
	heroJumpOff = -4;
	update = true;
	//����С�ڹ��ز�
	/*loadimage(&imgTortoise, "res/t1.png");
	tortoiseExist = false;
	torToiseY = 350 - imgTortoise.getheight();*/
	IMAGE imgtort;
	loadimage(&imgtort, "res/t1.png");
	vector<IMAGE>imgTortArray;
	imgTortArray.push_back(imgtort);
	obstacleImgs.push_back(imgTortArray);

	IMAGE imgLion;
	vector<IMAGE>imgLionArray;
	for (int i = 0; i < 6; i++)
	{
		sprintf(name, "res/p%0d.png", i + 1);
		loadimage(&imgLion,name);
		imgLionArray.push_back(imgLion);
	}
	obstacleImgs.push_back(imgLionArray);
	//��ʼ���ϰ���
	for (int i = 0; i < OBSTACLE_COUNT; i++)
	{
		obstacles[i].exist = false;
	}
	//�����¶��ز�
	loadimage(&imgHeroDown[0], "res/d1.png");
	loadimage(&imgHeroDown[1], "res/d2.png");
	heroDown = false;
	//���������ϰ���
	IMAGE imgH;
	for (int i = 0; i < 4; i++)
	{	
		vector<IMAGE> imgHookArray;
		sprintf(name, "res/h%d.png", i + 1);
		loadimage(&imgH, name,63,260,true);
		imgHookArray.push_back(imgH);
		obstacleImgs.push_back(imgHookArray);
	}
	//Ӣ��Ѫ��
	heroBlood = 100;
	//Ԥ������Ч
	preLoadSound("res/hit.mp3");

	mciSendString("play res/bg.mp3 repeat", 0, 0, 0);//���ű�������
	lastObsIndex = -1;
	score = 0;
	//��������ͼƬ
	for (int i = 0; i < 10; i++)
	{
		sprintf(name, "res/sz/%d.png",i);
		loadimage(&imgSZ[i], name);
	}
}
void jump()
{
	heroJump = true;
	update = true;
}
void down() {
	heroDown = true;
	update = true;
	heroIndex = 0;
}
void keyEvent()
{	
	char ch;
	if (kbhit())//����а�����kbhit()����true
	{
		ch=getch();//getch()����Ҫ���»س�����ֱ�Ӷ�ȡ
		if (ch == ' ')
		{
			jump();
		}
		else if (ch == 'a') {
			down();
		}

	}
}
void updateEnemy()
{
	////��ȾС�ڹ�
	//if (tortoiseExist)
	//{
	//	putimagePNG2(torToiseX, torToiseY, win_width, &imgTortoise);
	//}
	for (int i = 0; i < OBSTACLE_COUNT; i++)
	{
		if (obstacles[i].exist)
		{
			putimagePNG2(obstacles[i].x, obstacles[i].y, win_width, &obstacleImgs[obstacles[i].type][obstacles[i].imgIndex]);
		}
	}
}

void updateHero() {
	if (!heroDown)
	{
		putimagePNG2(heroX, heroY, &imgHeros[heroIndex]);
	}else
	{
		int y = 345 - imgHeroDown[heroIndex].getheight();
		putimagePNG2(heroX, y, &imgHeroDown[heroIndex]);
	}
}
void updateBloodbar()
{
	drawBloodBar(10, 10, 200, 10, 2, BLUE,DARKGRAY, RED, heroBlood / 100.0);
}
void checkOver()
{
	if (heroBlood <= 0)
	{
		loadimage(0, "res/over.png");
		FlushBatchDraw();
		mciSendString("stop res/bg/mp3", 0, 0, 0);
		system("pause");
		//��֮ͣ�󣬳�Ҹ������ֱ�ӿ�ʼ��һ��
		restart();
	}
}
void checkScore() {
	for (int i = 0;i< OBSTACLE_COUNT; i++)
	{
		if (obstacles[i].exist && obstacles[i].passed == false && obstacles[i].hited==false
			&& obstacles[i].x + obstacleImgs[obstacles[i].type][0].getwidth() < heroX) {
			score++;
			obstacles[i].passed = true;
		}
	}
}
void updateScore()
{
	char str[8];
	int x = 20;
	int y = 25;
	sprintf(str, "%d", score);
	for (int i = 0; str[i]; i++)
	{
		int sz = str[i] - '0';
		putimagePNG(x, y, &imgSZ[sz]);
		x += imgSZ[sz].getwidth() + 5;
	}
}

void checkWin() {
	if (score >= WIN_SCORE)
	{
		FlushBatchDraw();
		mciSendString("play res/win.mp3", 0, 0, 0);
		Sleep(2000);
		loadimage(0, "res/win.png");
		FlushBatchDraw();
		mciSendString("stop res/bg.mp3", 0, 0, 0);
		system("pause");
		restart();//���³�ʼ��

	}
}
int main()
{
	init();
	//��Ϸ�����ʼ��
	loadimage(0, "res/over.png");
	system("pause");
	int timer=0;
	while (1)
	{	
		keyEvent();
		timer += getDelay();
		if (timer > 30) 
		{
			timer = 0;
			update = true;
		}
		if (update)
		{	
			update = false;
			BeginBatchDraw();
			updateBg();
			//putimagePNG2(heroX, heroY, &imgHeros[heroIndex]);
			updateHero();
			updateEnemy();
			updateBloodbar();
			updateScore();
			checkWin();
			EndBatchDraw();

			checkOver();
			checkScore();
			fly();
		}

	}
	system("pause");
	return 0;
}