#pragma once
#include<graphics.h>
#include<conio.h>
#include<cmath>
#include<mmsystem.h>
#include<Windows.h>
#include<iostream>
#include<ctime>
#include<iostream>
#include<cstdlib>
#include <chrono>
#include <thread>
#include <algorithm>
#include<array>
#include<random>

using namespace std;

#define FLOWERPOT 0
#define CABBAGE 1
#define CORN 2
#define WATERMELON 3
#define HOTPEPPER 4
#define ICEMUSHROOM 5

#define NORMAL 0
#define BARRICADE 1
#define BUCKET 2
#define FOOTBALL 3
#define CATAPULT 4
#define ICECAR 5
#define JOKER 6

extern int plantsNum;
extern int zombieNum;
extern int plants_state[5][8];
extern int cur_x, cur_y;			// 当前光标位置
extern int cur_plants;				// 光标当前植物
extern int pointer_row;

extern IMAGE background;
extern IMAGE progressbar[6];
extern IMAGE whiteline;
extern IMAGE pointer;

extern IMAGE plants[6];

extern IMAGE card[6];
extern IMAGE cabbage[45];
extern IMAGE watermolen[47];
extern IMAGE hotpepper[29];
extern IMAGE corn_bomb;
extern IMAGE corn_crater;
extern IMAGE fire[3];
extern IMAGE snow[3];
extern IMAGE normalwalk[47];
extern IMAGE normaleat[11];
extern IMAGE barricadewalk[47];
extern IMAGE barricade[11];
extern IMAGE bucketwalk[47];
extern IMAGE bucketeat[11];
extern IMAGE footballwalk[30];
extern IMAGE footballeat[30];
extern IMAGE catapult[45];
extern IMAGE jokerwalk[21];
extern IMAGE jokereat[24];
extern IMAGE jokerbomb[31];

extern IMAGE tmpImg1;
extern IMAGE tmpImg2;

extern ExMessage mousemsg;

struct coordinate {
	int x;
	int y;
};

extern coordinate xy_pot[32][32];
extern coordinate xy_else_plants[32][32];
// extern int y_zombie[5];

class Conveycard {
public:
	void generate_card();
	void move_card();
	void delete_card(int x);
	void click_card();
	Conveycard() {
		std::fill_n(card_type, 10, -1);
		for (int i = 0; i < 10; i++) {
			card_pos[i].x = 690;
			card_pos[i].y = 5;
		}
	}
private:
	coordinate card_pos[10];			// 跟踪坐标
	int convey_length = 0;				// 当前卡片数目
	int card_type[16];					// 指定坐标的植物类型
};

extern Conveycard convey_card;

class Plants {
public:
	int hp;
	int hit;
	int type;
	int x;
	int y;
	int No;
	int frameNo;
	int eaten;
	bool hitting;
	int hurt;
	Plants() {
		No = plantsNum;
		plantsNum++;
		hp = 6;
		frameNo = 0;
		eaten = 0;
		hitting = 0;
		hurt = 0;
	}
};
class Flowerpot : public Plants {
public:
	Flowerpot() {
		type = FLOWERPOT;
		hit = 0;
	}
};
class Cabbage : public Plants {
public:
	Cabbage() {
		type = CABBAGE;
		hit = 40;
	}
};
class Corn : public Plants {
public:
	Corn() {
		hp = 1800;
		type = CORN;
		hit = 1800;
	}
};
class Watermelon : public Plants {
public:
	unsigned sputtering;
	Watermelon() {
		type = WATERMELON;
		hit = 80;
		sputtering = 26;
	}
};
class Hotpepper : public Plants {
public:
	Hotpepper() {
		frameNo = 0;
		type = HOTPEPPER;
		hp = 10000;
		hit = 1800;
	}
};
class Icemushroom : public Plants {
public:
	Icemushroom() {
		type = ICEMUSHROOM;
		hp = 10000;
		hit = 20;
	}
};
class Zombie {
public:
	int type;
	int hp;
	int state;				//walk:0 eat:1 bomb:2
	int speed;
	int No;
	int row;
	int location;
	int frameNo;
	int maxFrameW;
	int maxFrameE;
	int FrameBomb;
	int isFrozen;
	int xspan;
	int ishit;				//被攻击
	int hurt;
	void jokerBomb(int frame);
	Zombie() {
		No = zombieNum;
		zombieNum++;
		state = 0;
		isFrozen = 0;
		frameNo = 0;
		ishit = 0;
		xspan = 120;
		hurt = 0;
		FrameBomb = 0;
	}
};
class Normalzombie : public Zombie {
public:
	Normalzombie() {
		hp = 200;
		type = NORMAL;
		speed = 1;
		maxFrameW = 46;
		maxFrameE = 10;
	}
};
class Barricadezombie : public Zombie {
public:
	Barricadezombie() {
		hp = 400;
		type = BARRICADE;
		speed = 1;
		maxFrameW = 46;
		maxFrameE = 10;
	}
};
class Bucketzombie : public Zombie {
public:
	Bucketzombie() {
		hp = 1000;
		type = BUCKET;
		speed = 1;
		maxFrameW = 46;
		maxFrameE = 10;
	}
};
class Footballzombie : public Zombie {
public:
	Footballzombie() {
		hp = 1800;
		type = FOOTBALL;
		speed = 1;
		maxFrameW = 27;
		maxFrameE = 29;
	}
};
class Catapultzombie : public Zombie {
public:
	Catapultzombie() {
		hp = 1200;
		type = CATAPULT;
		speed = 0;
		maxFrameW = 44;
		maxFrameE = 0;
	}
};
class Jokerzombie : public Zombie {
public:
	Jokerzombie() {
		hp = 400;
		type = JOKER;
		speed = 1;
		maxFrameW = 20;
		maxFrameE = 23;
	}
};

template<class T>
class Node {
public:
	T* context;
	Node* next = NULL;
	Node(T* t) {
		context = t;
	}
};

template<class T>
class LinkList {
public:
	Node<T>* head;
	Node<T>* tail;
	LinkList() {
		head = NULL;
		tail = NULL;
	}
	LinkList(Node<T> node) {
		head = node;
		tail = node;
	}
	~LinkList() {
		DeleteAllNode();
	}
	void InsertNode(T* t) {
		Node<T>* node = new Node<T>(t);
		if (head == NULL) {
			head = node;
			tail = node;
		}
		else {
			tail->next = node;
			tail = node;
		}
	}
	void DeleteNode(int No) {
		Node<T>* cur = head;
		Node<T>* pre = NULL;
		while (cur != NULL && cur->context->No != No) {
			pre = cur;
			cur = cur->next;
		}
		if (cur == NULL) {
			return;
		}
		else if (pre == NULL) {
			head = cur->next;
		}
		else {
			pre->next = cur->next;
		}
		if (cur == tail) {
			tail = pre;
		}
		delete cur;
	}
	void DeleteAllNode() {
		Node<T>* cur = head;
		Node<T>* pre = NULL;
		while (tail != NULL) {
			pre = cur;
			cur = cur->next;
			DeleteNode(pre->context->No);
		}
	}
};

extern LinkList<Plants> plants_link;

void loading();
void preciseSleep(int microseconds);
int getDelay();
void transImage(IMAGE* dstimg, int x, int y, IMAGE* srcimg);
void init_pot_coordinate();
void init_else_plants_coordinate();
int getRandom();
coordinate put_place();
void put_flowerpot(coordinate xy);
void cabbage_hit(coordinate xy, int frameNo);
void put_cabbage(coordinate xy);
void watermelon_hit(coordinate xy, int frameNo);
void put_watermelon(coordinate xy);
void put_hotpepper(coordinate xy, int frameNo);
void put_fire(int row);
void put_icemushroom(coordinate xy);
void put_icebk(int p[43][30]);
void put_corn(coordinate xy, int frameNo);
void put_crater(coordinate xy);
void get_keyboard();
void gerenateNormal();
void gerenateBarricade();
void gerenateBucket();
void gerenateFootball();
void gerenateCatapult();
void gerenateJoker();
void zombieFrozen(IMAGE* targetImage, IMAGE* srcImage, int addRed, int addGreen, int addBlue);
void PrintPlants();
void PrintZombie();
void GameEngine();