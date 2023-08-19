#include "pvz.h"
#pragma warning (disable:4996)

LinkList<Plants> plants_link;
LinkList<Zombie> zombie_link;

int plantsNum = 0;
int zombieNum = 0;
int plants_state[5][8];
int cur_x, cur_y;					// current cursor position
int cur_plants = -1;				// cursor current plant
int pointer_row = 0;

IMAGE background;
IMAGE progressbar[6];
IMAGE whiteline;
IMAGE pointer;

IMAGE plants[6];

IMAGE card[6];
IMAGE cabbage[45];
IMAGE watermolen[47];
IMAGE hotpepper[29];
IMAGE corn_bomb;
IMAGE corn_crater;
IMAGE fire[3];
IMAGE snow[3];
IMAGE normalwalk[47];
IMAGE normaleat[11];
IMAGE barricadewalk[47];
IMAGE barricadeeat[11];
IMAGE bucketwalk[47];
IMAGE bucketeat[11];
IMAGE footballwalk[30];
IMAGE footballeat[30];
IMAGE catapult[45];
IMAGE jokerwalk[21];
IMAGE jokereat[24];
IMAGE jokerbomb[31];

IMAGE tmpImg1;
IMAGE tmpImg2;

ExMessage mousemsg;

coordinate xy_pot[32][32];
coordinate xy_else_plants[32][32];
// int y_zombie[5] = { 100,185,270,355,440 };
Conveycard convey_card;

void loadImage(IMAGE pic[], int n, const char path[])
{
	char tmpPath[200], frameNo[4];
	for (int i = 0; i < n; i++) {
		strcpy_s(tmpPath, 200, path);
		strcat(strcat(tmpPath, itoa(i + 1, frameNo, 10)), ".png");
		loadimage(&pic[i], tmpPath);
	}
}

void loading()
{
	loadimage(&background, "./resource/background/background6boss.jpg");
	loadimage(&progressbar[0], "./resource/progressbar/11.png");
	loadimage(&progressbar[1], "./resource/progressbar/22.png");
	loadimage(&progressbar[2], "./resource/progressbar/33.png");
	loadimage(&progressbar[3], "./resource/progressbar/44.png");
	loadimage(&progressbar[4], "./resource/progressbar/55.png");
	loadimage(&progressbar[5], "./resource/progressbar/66.png");
	loadimage(&pointer, "./resource/background/pointer.gif");

	loadimage(&card[0], "./resource/card/flowerpot_card.jpg");
	loadimage(&card[1], "./resource/card/cabbage_card.jpg");
	loadimage(&card[2], "./resource/card/corn_card.jpg");
	loadimage(&card[3], "./resource/card/watermelon_card.jpg");
	loadimage(&card[4], "./resource/card/hotpepper_card.jpg");
	loadimage(&card[5], "./resource/card/icemushroom_card.jpg");

	loadimage(&plants[0], "./resource/plants/flowerpot.png");
	loadimage(&plants[1], "./resource/plants/cabbage.png");
	loadimage(&plants[2], "./resource/plants/corn.png");
	loadimage(&plants[3], "./resource/plants/watermelon.png");
	loadimage(&plants[4], "./resource/plants/hotpepper.png");
	loadimage(&plants[5], "./resource/plants/icemushroom.png");
	loadimage(&corn_bomb, "./resource/plants/corn/corn_bomb.png");
	loadimage(&corn_crater, "./resource/plants/corn/corn_crater.png");
	loadimage(&fire[0], "./resource/plants/hotpepper/fire1.png");
	loadimage(&fire[1], "./resource/plants/hotpepper/fire2.png");
	loadimage(&fire[2], "./resource/plants/hotpepper/fire3.png");
	loadimage(&snow[0], "./resource/plants/icemushroom/snow1.png");
	loadimage(&snow[1], "./resource/plants/icemushroom/snow2.png");
	loadimage(&snow[2], "./resource/plants/icemushroom/snow3.png");

	loadImage(cabbage, 45, "./resource/plants/cabbage/");
	loadImage(watermolen, 47, "./resource/plants/watermelon/");
	loadImage(hotpepper, 29, "./resource/plants/hotpepper/");
	loadImage(normalwalk, 47, "./resource/zombie/normal_zomblie/walk/");
	loadImage(normaleat, 11, "./resource/zombie/normal_zomblie/eat/");
	loadImage(barricadewalk, 47, "./resource/zombie/barricade_zombie/walk/");
	loadImage(barricadeeat, 11, "./resource/zombie/barricade_zombie/eat/");
	loadImage(bucketwalk, 47, "./resource/zombie/buckethead_zombie/walk/");
	loadImage(bucketeat, 11, "./resource/zombie/buckethead_zombie/eat/");
	loadImage(footballwalk, 28, "./resource/zombie/football_zombie/walk/");
	loadImage(footballeat, 30, "./resource/zombie/football_zombie/eat/");
	loadImage(catapult, 45, "./resource/zombie/catapult_zombie/");
	loadImage(jokerwalk, 21, "./resource/zombie/joker_zombie/walk/");
	loadImage(jokereat, 24, "./resource/zombie/joker_zombie/eat/");
	loadImage(jokerbomb, 31, "./resource/zombie/joker_zombie/bomb/");
}

void preciseSleep(int microseconds)
{
	auto start = std::chrono::high_resolution_clock::now();
	auto end = start + std::chrono::microseconds(microseconds);
	while (std::chrono::high_resolution_clock::now() < end)
	{
		std::this_thread::sleep_for(std::chrono::microseconds(1));
	}
}
int getDelay() {
	static unsigned long long lastTime = 0;
	unsigned long long currentTime = GetTickCount();
	if (lastTime == 0) {
		lastTime = currentTime;
		return 0;
	}
	else {
		int ret = currentTime - lastTime;
		lastTime = currentTime;
		return ret;
	}
}

void transImage(IMAGE* dstimg, int x, int y, IMAGE* srcimg)
{
	HDC dstDC = GetImageHDC(dstimg);
	HDC srcDC = GetImageHDC(srcimg);
	int w = srcimg->getwidth();
	int h = srcimg->getheight();
	BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
	AlphaBlend(dstDC, x, y, w, h, srcDC, 0, 0, w, h, bf);
}
void init_pot_coordinate()
{
	int xspan1 = 83, yspan1 = -21;			//oblique x-axis
	int xspan2 = 0, yspan2 = 83;			//oblique y-axis
	int xspan3 = 80, yspan3 = 0;			//vertical x-axis
	int xspan4 = 0, yspan4 = 85;			//vertical y-axis
	xy_pot[0][0].x = 45;
	xy_pot[0][0].y = 248;
	for (int i = 1; i < 5; i++) {
		xy_pot[0][i].x = xy_pot[0][i - 1].x + xspan1;
		xy_pot[0][i].y = xy_pot[0][i - 1].y + yspan1;
	}
	for (int i = 1; i < 5; i++) {
		xy_pot[i][0].x = xy_pot[0][0].x;
		xy_pot[i][0].y = xy_pot[i - 1][0].y + yspan2;
	}
	for (int i = 1; i < 5; i++) {
		for (int j = 1; j < 5; j++) {
			xy_pot[i][j].x = xy_pot[0][j].x;
			xy_pot[i][j].y = xy_pot[i][j - 1].y + yspan1;
		}
	}
	xy_pot[0][5].x = 455;
	xy_pot[0][5].y = 148;

	for (int i = 6; i < 8; i++) {
		xy_pot[0][i].x = xy_pot[0][i - 1].x + xspan3;
		xy_pot[0][i].y = xy_pot[0][5].y;
	}
	for (int i = 1; i < 5; i++) {
		xy_pot[i][5].x = xy_pot[0][5].x;
		xy_pot[i][5].y = xy_pot[i - 1][5].y + yspan4;
	}
	for (int i = 0; i < 5; i++) {
		for (int j = 6; j < 8; j++) {
			xy_pot[i][j].x = xy_pot[0][j].x;
			xy_pot[i][j].y = xy_pot[i][5].y;
		}
	}
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 3; j++) {
			plants_state[i][j] = FLOWERPOT;
			Flowerpot* flowerpot_node = new Flowerpot();
			flowerpot_node->x = i;
			flowerpot_node->y = j;
			plants_link.InsertNode(flowerpot_node);
		}
	}
}
void init_else_plants_coordinate()
{
	int xspan1 = 83, yspan1 = -21;			//oblique x-axis
	int xspan2 = 0, yspan2 = 83;			//oblique y-axis
	int xspan3 = 80, yspan3 = 0;			//vertical x-axis
	int xspan4 = 0, yspan4 = 85;			//vertical y-axis
	xy_else_plants[0][0].x = 45;
	xy_else_plants[0][0].y = 248;
	for (int i = 1; i < 5; i++) {
		xy_else_plants[0][i].x = xy_else_plants[0][i - 1].x + xspan1;
		xy_else_plants[0][i].y = xy_else_plants[0][i - 1].y + yspan1;
	}
	for (int i = 1; i < 5; i++) {
		xy_else_plants[i][0].x = xy_else_plants[0][0].x;
		xy_else_plants[i][0].y = xy_else_plants[i - 1][0].y + yspan2;
	}
	for (int i = 1; i < 5; i++) {
		for (int j = 1; j < 5; j++) {
			xy_else_plants[i][j].x = xy_else_plants[0][j].x;
			xy_else_plants[i][j].y = xy_else_plants[i][j - 1].y + yspan1;
		}
	}
	xy_else_plants[0][5].x = 455;
	xy_else_plants[0][5].y = 148;

	for (int i = 6; i < 9; i++) {
		xy_else_plants[0][i].x = xy_else_plants[0][i - 1].x + xspan3;
		xy_else_plants[0][i].y = xy_else_plants[0][5].y;
	}
	for (int i = 1; i < 5; i++) {
		xy_else_plants[i][5].x = xy_else_plants[0][5].x;
		xy_else_plants[i][5].y = xy_else_plants[i - 1][5].y + yspan4;
	}
	for (int i = 0; i < 5; i++) {
		for (int j = 6; j < 9; j++) {
			xy_else_plants[i][j].x = xy_else_plants[0][j].x;
			xy_else_plants[i][j].y = xy_else_plants[i][5].y;
		}
	}
}

coordinate put_place()
{
	/*
	y=-0.240426x+210.000000
	y=-0.240426x+293.000000
	y=-0.244681x+380.000000
	y=-0.240426x+463.000000
	y=-0.240426x+550.000000
	y=-0.240426x+635.000000
	*/
	int pl1[] = { 210,293,380,463,550,635 };
	int pl2[] = { 93,173,260,345,425,515 };
	double k = -0.240426;
	coordinate xy;
	xy.x = 10, xy.y = 10;
	plants_state[xy.x][xy.y] = FLOWERPOT;
	// cout << mousemsg.x * k + 210 << "\t" << mousemsg.y << "\t" << mousemsg.x * k + 293 << endl;
	for (int i = 0; i < 5; i++) {
		if (mousemsg.x * k + pl1[i] < mousemsg.y && mousemsg.x * k + pl1[i + 1] > mousemsg.y) {
			if (mousemsg.x <= 142 && mousemsg.x > 42) {
				xy.x = i;
				xy.y = 0;
				return xy;
			}
			else if (mousemsg.x <= 226 && mousemsg.x > 142) {
				xy.x = i;
				xy.y = 1;
				return xy;
			}
			else if (mousemsg.x <= 307 && mousemsg.x > 226) {
				xy.x = i;
				xy.y = 2;
				return xy;
			}
			else if (mousemsg.x <= 385 && mousemsg.x > 307) {
				xy.x = i;
				xy.y = 3;
				return xy;
			}
			else if (mousemsg.x <= 470 && mousemsg.x > 385) {
				xy.x = i;
				xy.y = 4;
				return xy;
			}
		}
		if (mousemsg.y > pl2[i] && mousemsg.y < pl2[i + 1]) {
			if (mousemsg.x <= 550 && mousemsg.x > 470) {
				xy.x = i;
				xy.y = 5;
				return xy;
			}
			else if (mousemsg.x <= 624 && mousemsg.x > 550) {
				xy.x = i;
				xy.y = 6;
				return xy;
			}
			else if (mousemsg.x <= 710 && mousemsg.x > 624) {
				xy.x = i;
				xy.y = 7;
				return xy;
			}
		}
	}
	return xy;
}

void put_flowerpot(coordinate xy)
{
	int x = xy.x - 205, y = xy.y - 405;
	transImage(NULL, x, y, &plants[0]);
}
void put_cabbage(coordinate xy)
{
	int x = xy.x - 35, y = xy.y - 537;
	transImage(NULL, x, y, &plants[1]);
}
void cabbage_hit(coordinate xy, int frameNo)
{
	int x = xy.x - 35, y = xy.y - 537;
	transImage(NULL, x, y, &cabbage[frameNo]);
}
void put_watermelon(coordinate xy)
{
	int x = xy.x - 121, y = xy.y - 240;
	transImage(NULL, x, y, &plants[3]);
}
void watermelon_hit(coordinate xy, int frameNo)
{
	int x = xy.x - 121, y = xy.y - 240;
	transImage(NULL, x, y, &watermolen[frameNo]);
}
void put_hotpepper(coordinate xy, int frameNo)
{
	int x = xy.x - 200, y = xy.y - 412;
	transImage(NULL, x, y, &hotpepper[frameNo]);
}
void put_fire(int row)
{
	int r;
	for (int i = 0; i < 8; i++) {
		r = rand() % 3;
		transImage(NULL, xy_else_plants[row][i].x, xy_else_plants[row][i].y - 70, &fire[r]);
	}
}
void put_icemushroom(coordinate xy)
{
	int x = xy.x - 290, y = xy.y - 476;
	transImage(NULL, x, y, &plants[5]);
}
void put_icebk(int p[43][30])
{
	for (int i = 0; i < 43; i++) {
		for (int j = 0; j < 30; j++) {
			switch (p[i][j]) {
			case 0:
				transImage(NULL, i * 20, j * 20, &snow[0]);
				break;
			case 1:
				transImage(NULL, i * 20, j * 20, &snow[1]);
				break;
			case 2:
				transImage(NULL, i * 20, j * 20, &snow[2]);
				break;
			}
		}
	}
}
void put_corn(coordinate xy, int frameNo)
{
	// int x = xy.x - 540, y = xy.y - 190;
	int x = xy.x - 540, y = xy.y - 560 + frameNo * 10;
	transImage(NULL, x, y, &corn_bomb);
}
void put_crater(coordinate xy)
{
	int x = xy.x - 50, y = xy.y - 110;
	transImage(NULL, x, y, &corn_crater);
}
int getRandom()
{
	static std::array<double, 6> probabilities = { 0.3, 0.2, 0.2, 0.1, 0.1, 0.1 };
	static std::mt19937 gen(std::random_device{}());
	std::discrete_distribution<> d(probabilities.begin(), probabilities.end());
	int result = d(gen);
	if (result) {
		if (probabilities[result] >= 0.05) {
			probabilities[result] -= 0.05;
			for (int i = 1; i < 6; ++i) {
				if (i != result) {
					probabilities[i] += 0.01;
				}
			}
		}
		else {
			probabilities[1] = 0.2;
			probabilities[2] = 0.2;
			probabilities[3] = 0.1;
			probabilities[4] = 0.1;
			probabilities[5] = 0.1;
		}
	}
	/*
	if (result && probabilities[result] >= 0.05) {
		probabilities[result] -= 0.05;
		for (int i = 1; i < 6; ++i) {
			if (i != result) {
				probabilities[i] += 0.01;
			}
		}
	}
	*/
	return result;
}
void Conveycard::generate_card()
{
	if (convey_length == 9) {
		return;
	}
	int choose_card = getRandom();
	card_type[convey_length++] = choose_card;
}
void Conveycard::delete_card(int x)
{
	if (x == convey_length) {
		card_type[x - 1] = -1;
		card_pos[x - 1].x = 690;
		card_pos[x - 1].y = 5;
		convey_length--;
		return;
	}
	for (int i = x; i < convey_length - 1; i++) {
		card_type[i] = card_type[i + 1];
		card_pos[i].x = card_pos[i + 1].x;
		card_pos[i].y = card_pos[i + 1].y;
	}
	card_type[convey_length - 1] = -1;
	card_pos[convey_length - 1].x = 690;
	card_pos[convey_length - 1].y = 5;
	convey_length--;
}
void Conveycard::move_card()
{
	constexpr int speed = 1;
	for (int i = 0; i < convey_length; i++) {
		if (card_pos[i].x != 156 + 53 * i) {
			card_pos[i].x -= speed;
		}
	}
	for (int i = 0; i < convey_length; i++) {
		putimage(card_pos[i].x, card_pos[i].y, &card[card_type[i]]);
	}
}
void Conveycard::click_card()
{
	static int status = 0;
	static int current_num = 0;
	coordinate xy;
	if (peekmessage(&mousemsg, EX_MOUSE)) {
		if (mousemsg.message == WM_LBUTTONDOWN && status == 0) {
			for (int i = 0; i < convey_length; ++i) {
				if (mousemsg.x >= card_pos[i].x && mousemsg.x < card_pos[i].x + 53 && mousemsg.y >= 5 && mousemsg.y <= 80) {
					status = 1;
					cur_plants = card_type[i];
					current_num = i;
					switch (cur_plants) {
					case 0:
						cur_x = mousemsg.x - 255;
						cur_y = mousemsg.y - 400;
						break;
					case 1:
						cur_x = mousemsg.x - 80;
						cur_y = mousemsg.y - 500;
						break;
					case 2:
						cur_x = mousemsg.x - 35;
						cur_y = mousemsg.y - 35;
						break;
					case 3:
						cur_x = mousemsg.x - 170;
						cur_y = mousemsg.y - 200;
						break;
					case 4:
						cur_x = mousemsg.x - 265;
						cur_y = mousemsg.y - 350;
						break;
					case 5:
						cur_x = mousemsg.x - 342;
						cur_y = mousemsg.y - 430;
						break;
					}
				}
			}
		}
		else if (mousemsg.message == WM_MOUSEMOVE && status == 1) {
			switch (cur_plants) {
			case 0:
				cur_x = mousemsg.x - 255;
				cur_y = mousemsg.y - 400;
				break;
			case 1:
				cur_x = mousemsg.x - 80;
				cur_y = mousemsg.y - 500;
				break;
			case 2:
				cur_x = mousemsg.x - 35;
				cur_y = mousemsg.y - 35;
				break;
			case 3:
				cur_x = mousemsg.x - 170;
				cur_y = mousemsg.y - 200;
				break;
			case 4:
				cur_x = mousemsg.x - 265;
				cur_y = mousemsg.y - 350;
				break;
			case 5:
				cur_x = mousemsg.x - 342;
				cur_y = mousemsg.y - 430;
				break;
			}
		}
		else if (mousemsg.message == WM_LBUTTONDOWN && status == 1 && !cur_plants) {
			mciSendString("play ./resource/music/plant.mp3 from 0 ", 0, 0, 0);
			xy = put_place();
			/*
			cout << xy.x << "  " << xy.y << endl;
			for (int i = 0; i < 5; i++) {
				for (int j = 0; j < 8; j++) {
					printf("%d ", plants_state[i][j]);
				}
				printf("\n");
			}
			*/
			plants_state[xy.x][xy.y] = FLOWERPOT;
			Flowerpot* flowerpot_node = new Flowerpot();
			flowerpot_node->x = xy.x;
			flowerpot_node->y = xy.y;
			plants_link.InsertNode(flowerpot_node);
			status = 0;
			cur_plants = -1;
			delete_card(current_num);
		}
		else if (mousemsg.message == WM_LBUTTONDOWN && status == 1 && cur_plants > 0) {
			mciSendString("play ./resource/music/plant.mp3 from 0 ", 0, 0, 0);
			xy = put_place();
			if (plants_state[xy.x][xy.y] == FLOWERPOT) {
				switch (cur_plants) {
				case CABBAGE:
				{
					plants_state[xy.x][xy.y] = CABBAGE;
					Cabbage* cabbage_node = new Cabbage();
					cabbage_node->x = xy.x;
					cabbage_node->y = xy.y;
					plants_link.InsertNode(cabbage_node);
					status = 0;
					cur_plants = -1;
					delete_card(current_num);
					break;
				}
				case CORN:
				{
					plants_state[xy.x][xy.y] = CORN;
					Corn* corn_node = new Corn();
					corn_node->x = xy.x;
					corn_node->y = xy.y;
					plants_link.InsertNode(corn_node);
					status = 0;
					cur_plants = -1;
					delete_card(current_num);
					break;
				}
				case WATERMELON:
				{
					plants_state[xy.x][xy.y] = WATERMELON;
					Watermelon* watermelon_node = new Watermelon();
					watermelon_node->x = xy.x;
					watermelon_node->y = xy.y;
					plants_link.InsertNode(watermelon_node);
					status = 0;
					cur_plants = -1;
					delete_card(current_num);
					break;
				}
				case HOTPEPPER:
				{
					plants_state[xy.x][xy.y] = HOTPEPPER;
					Hotpepper* hotpepper_node = new Hotpepper();
					hotpepper_node->x = xy.x;
					hotpepper_node->y = xy.y;
					plants_link.InsertNode(hotpepper_node);
					status = 0;
					cur_plants = -1;
					delete_card(current_num);
					break;
				}
				case ICEMUSHROOM:
				{
					plants_state[xy.x][xy.y] = ICEMUSHROOM;
					Icemushroom* icemushroom_node = new Icemushroom();
					icemushroom_node->x = xy.x;
					icemushroom_node->y = xy.y;
					plants_link.InsertNode(icemushroom_node);
					status = 0;
					cur_plants = -1;
					delete_card(current_num);
					break;
				}
				}
			}
		}

	}
}
void PrintPlants()
{
	int current_num = 0;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 8; j++) {
			switch (plants_state[i][j]) {
			case FLOWERPOT:
			{
				put_flowerpot(xy_pot[i][j]);
				break;
			}
			case CABBAGE:
			{
				put_flowerpot(xy_pot[i][j]);
				Node<Plants>* cur = plants_link.head;
				while (cur != NULL) {
					if (cur->context->x == i && cur->context->y == j && cur->context->type == CABBAGE) {
						if (cur->context->hitting) {
							if (cur->context->frameNo == 45) {
								cur->context->frameNo = 0;
							}
							cabbage_hit(xy_else_plants[i][j], cur->context->frameNo++);
						}
						else {
							put_cabbage(xy_else_plants[i][j]);
						}
					}
					cur = cur->next;
				}
				break;
			}
			case CORN:
			{
				put_flowerpot(xy_pot[i][j]);
				Node<Plants>* cur = plants_link.head;
				while (cur != NULL) {
					if (cur->context->x == i && cur->context->y == j && cur->context->type == CORN) {
						break;
					}
					else {
						cur = cur->next;
					}
				}
				if (cur != NULL) {
					if (cur->context->frameNo < 38) {
						put_corn(xy_pot[i][j], cur->context->frameNo++);
					}
					else if (cur->context->frameNo == 38) {
						mciSendString("play ./resource/music/cornbomb.mp3 from 0", 0, 0, 0);
						put_crater(xy_pot[i][j]);
						Node<Zombie>* curZ = zombie_link.head;
						while (curZ != NULL) {
							Zombie* zombieptr = curZ->context;
							if (abs(zombieptr->row - i) < 2 && abs(zombieptr->location - xy_pot[i][j].x + 30) < 100) {
								zombieptr->hp -= 1800;
							}
							curZ = curZ->next;
						}
						cur->context->frameNo++;
					}
					else if (cur->context->frameNo > 38 && cur->context->frameNo < 80) {
						put_crater(xy_pot[i][j]);
						cur->context->frameNo++;
					}
					else {
						plants_link.DeleteNode(cur->context->No);
						plants_state[i][j] = FLOWERPOT;
					}
				}
				break;
			}
			case WATERMELON:
			{
				put_flowerpot(xy_pot[i][j]);
				Node<Plants>* cur = plants_link.head;
				while (cur != NULL) {
					if (cur->context->x == i && cur->context->y == j && cur->context->type == WATERMELON) {
						if (cur->context->hitting) {
							if (cur->context->frameNo == 47) {
								cur->context->frameNo = 0;
							}
							watermelon_hit(xy_else_plants[i][j], cur->context->frameNo++);
						}
						else {
							put_watermelon(xy_else_plants[i][j]);
						}
					}
					cur = cur->next;
				}
				break;
			}
			case HOTPEPPER:
			{
				put_flowerpot(xy_pot[i][j]);
				Node<Plants>* cur = plants_link.head;
				while (cur != NULL) {
					if (cur->context->x == i && cur->context->y == j && cur->context->type == HOTPEPPER) {
						break;
					}
					else {
						cur = cur->next;
					}
				}
				if (cur != NULL) {
					if (cur->context->frameNo < 30) {
						put_hotpepper(xy_else_plants[i][j], cur->context->frameNo++);
					}
					else if (cur->context->frameNo == 30) {
						put_fire(i);
						Node<Zombie>* curZ = zombie_link.head;
						while (curZ != NULL) {
							Zombie* zombieptr = curZ->context;
							if (zombieptr->row == i) {
								zombieptr->hp -= 1800;
							}
							curZ = curZ->next;
						}
						++cur->context->frameNo;
					}
					else if (cur->context->frameNo > 30 && cur->context->frameNo < 50) {
						put_fire(i);
						cur->context->frameNo++;
					}
					else {
						plants_link.DeleteNode(cur->context->No);
						plants_state[i][j] = FLOWERPOT;
					}
				}
				break;
			}
			case ICEMUSHROOM:
			{
				static int snow_p[43][30];
				put_flowerpot(xy_pot[i][j]);
				Node<Plants>* cur = plants_link.head;
				while (cur != NULL) {
					if (cur->context->x == i && cur->context->y == j && cur->context->type == ICEMUSHROOM) {
						break;
					}
					else {
						cur = cur->next;
					}
				}
				if (cur != NULL) {
					if (cur->context->frameNo < 30) {
						put_icemushroom(xy_else_plants[i][j]);
						++cur->context->frameNo;
					}
					else if (cur->context->frameNo == 30) {
						for (int i = 0; i < 43; i++) {
							for (int j = 0; j < 30; j++) {
								int r = rand() % 40;
								snow_p[i][j] = r;
							}
						}
						Node<Zombie>* curZ = zombie_link.head;
						while (curZ != NULL) {
							Zombie* zombieptr = curZ->context;
							zombieptr->hp -= 100;
							zombieptr->isFrozen = 200;
							curZ = curZ->next;
						}
						++cur->context->frameNo;
					}
					else if (cur->context->frameNo > 30 && cur->context->frameNo < 40) {
						put_icebk(snow_p);
						++cur->context->frameNo;
					}
					else {
						plants_link.DeleteNode(cur->context->No);
						plants_state[i][j] = FLOWERPOT;
					}
				}
			}
			}
		}
	}
}
void gerenateNormal()
{
	Normalzombie* normal_node = new Normalzombie();
	normal_node->row = pointer_row;
	normal_node->location = 660;
	zombie_link.InsertNode(normal_node);
}
void gerenateBarricade()
{
	Barricadezombie* barricade_node = new Barricadezombie();
	barricade_node->row = pointer_row;
	barricade_node->location = 660;
	zombie_link.InsertNode(barricade_node);
}
void gerenateBucket()
{
	Bucketzombie* bucket_node = new Bucketzombie();
	bucket_node->row = pointer_row;
	bucket_node->location = 660;
	zombie_link.InsertNode(bucket_node);
}
void gerenateFootball()
{
	Footballzombie* football_node = new Footballzombie();
	football_node->row = pointer_row;
	football_node->location = 660;
	zombie_link.InsertNode(football_node);
}
void gerenateCatapult()
{
	Catapultzombie* catapult_node = new Catapultzombie();
	catapult_node->row = pointer_row;
	catapult_node->location = 660;
	zombie_link.InsertNode(catapult_node);
}
void gerenateJoker()
{
	Jokerzombie* joker_node = new Jokerzombie();
	joker_node->row = pointer_row;
	joker_node->location = 660;
	zombie_link.InsertNode(joker_node);
}
void Zombie::jokerBomb(int frame)
{
	if (frame < 31) {
		transImage(NULL, location - 550 + frame, xy_else_plants[row][8].y - xspan - 400, &jokerbomb[frame]);
	}
	else if (frame == 31) {
		Node<Plants>* curP = plants_link.head;
		while (curP != NULL) {
			Plants* plantsptr = curP->context;
			curP = curP->next;
		}
	}
	else if (frame > 31 && frame < 50) {
		transImage(NULL, location - 550 + frame, xy_else_plants[row][8].y - xspan - 400, &jokerbomb[30]);
	}
	else {
		type = -1;
	}
}
void get_keyboard()
{
	int singal = '\0';
	if (_kbhit()) {
		singal = _getch();
		switch (singal) {
		case 224:
			singal = _getch();
			if (singal == 72 && pointer_row != 0)--pointer_row;
			if (singal == 80 && pointer_row != 4)++pointer_row;
			break;
		case VK_ESCAPE:
			mciSendString("close BGM1", 0, 0, 0);
			closegraph();
			exit(0);
		case 'z':
		case 'Z':
			gerenateNormal();
			break;
		case 'x':
		case 'X':
			gerenateBarricade();
			break;
		case 'c':
		case 'C':
			gerenateBucket();
			break;
		case 'v':
		case 'V':
			gerenateFootball();
			break;
		case 'b':
		case 'B':
			gerenateCatapult();
			break;
		case 'n':
		case 'N':
			gerenateJoker();
			break;
		}
	}
}
void zombieFrozen(IMAGE* targetImage, IMAGE* srcImage, int addRed = 0, int addGreen = 0, int addBlue = 50)
{
	int srcImgWidth = srcImage->getwidth(), srcImgHeight = srcImage->getheight();
	targetImage->Resize(srcImgWidth, srcImgHeight);
	DWORD* pTargetBuffer = GetImageBuffer(targetImage);
	DWORD* pSrcBuffer = GetImageBuffer(srcImage);
	int allPixel = srcImgHeight * srcImgWidth;

#define RGBA(r, g, b, a) ((b) + (g << 8) + (r << 16) + (a << 24))
	for (int i = 0; i < allPixel; ++i)
	{
		UCHAR r = (UCHAR)GetRValue(pSrcBuffer[i]);
		UCHAR g = (UCHAR)GetGValue(pSrcBuffer[i]);
		UCHAR b = (UCHAR)GetBValue(pSrcBuffer[i]);
		r = r + addRed;
		r = r > 255 ? 255 : r;
		g = g + addGreen;
		g = g > 255 ? 255 : g;
		b = b + addBlue;
		b = b > 255 ? 255 : b;
		pTargetBuffer[i] = (DWORD)RGBA(r, g, b, pSrcBuffer[i] >> 24);
	}
}
void PrintZombie()
{
	static clock_t clo = 0;
	int football_bais = 0, joker_bais = 0;
	transImage(NULL, xy_else_plants[pointer_row][8].x + 30, xy_else_plants[pointer_row][8].y - 40, &pointer);
	Node<Zombie>* cur = zombie_link.head, * next = NULL;
	while (cur != NULL) {
		Zombie* zombieptr = cur->context;
		if (zombieptr->location < -150) {
			zombie_link.DeleteNode(zombieptr->No);
		}
		if (zombieptr->state == 0 && zombieptr->hp > 0) {
			switch (zombieptr->type) {
			case NORMAL:
				if (zombieptr->isFrozen) {
					tmpImg1 = normalwalk[zombieptr->frameNo];
					zombieFrozen(&tmpImg2, &tmpImg1);
					transImage(NULL, zombieptr->location, xy_else_plants[zombieptr->row][8].y - zombieptr->xspan, &tmpImg2);
				}
				else transImage(NULL, zombieptr->location, xy_else_plants[zombieptr->row][8].y - zombieptr->xspan, &normalwalk[zombieptr->frameNo]);
				break;
			case BARRICADE:
				if (zombieptr->isFrozen) {
					tmpImg1 = barricadewalk[zombieptr->frameNo];
					zombieFrozen(&tmpImg2, &tmpImg1);
					transImage(NULL, zombieptr->location, xy_else_plants[zombieptr->row][8].y - zombieptr->xspan, &tmpImg2);
				}
				else transImage(NULL, zombieptr->location, xy_else_plants[zombieptr->row][8].y - zombieptr->xspan, &barricadewalk[zombieptr->frameNo]);
				break;
			case BUCKET:
				if (zombieptr->isFrozen) {
					tmpImg1 = bucketwalk[zombieptr->frameNo];
					zombieFrozen(&tmpImg2, &tmpImg1);
					transImage(NULL, zombieptr->location, xy_else_plants[zombieptr->row][8].y - zombieptr->xspan, &tmpImg2);
				}
				else transImage(NULL, zombieptr->location, xy_else_plants[zombieptr->row][8].y - zombieptr->xspan, &bucketwalk[zombieptr->frameNo]);
				break;
			case FOOTBALL:
				football_bais = zombieptr->frameNo;
				if (zombieptr->isFrozen) {
					tmpImg1 = footballwalk[zombieptr->frameNo];
					zombieFrozen(&tmpImg2, &tmpImg1);
					transImage(NULL, zombieptr->location - 550 + football_bais, xy_else_plants[zombieptr->row][8].y - zombieptr->xspan - 400, &tmpImg2);
				}
				else transImage(NULL, zombieptr->location - 550 + football_bais, xy_else_plants[zombieptr->row][8].y - zombieptr->xspan - 400, &footballwalk[zombieptr->frameNo]);
				break;
			case CATAPULT:
				if (zombieptr->isFrozen) {
					tmpImg1 = catapult[zombieptr->frameNo];
					zombieFrozen(&tmpImg2, &tmpImg1);
					transImage(NULL, zombieptr->location - 510, xy_else_plants[zombieptr->row][8].y - 330, &tmpImg2);
				}
				else transImage(NULL, zombieptr->location - 510, xy_else_plants[zombieptr->row][8].y - 330, &catapult[zombieptr->frameNo]);
				break;
			case JOKER:
				joker_bais = zombieptr->frameNo;
				if (zombieptr->isFrozen) {
					tmpImg1 = jokerwalk[zombieptr->frameNo];
					zombieFrozen(&tmpImg2, &tmpImg1);
					transImage(NULL, zombieptr->location - 550 + joker_bais, xy_else_plants[zombieptr->row][8].y - zombieptr->xspan - 400, &tmpImg2);
				}
				else transImage(NULL, zombieptr->location - 550 + joker_bais, xy_else_plants[zombieptr->row][8].y - zombieptr->xspan - 400, &jokerwalk[zombieptr->frameNo]);
				break;
			}
			if (zombieptr->type >= NORMAL && zombieptr->type <= BUCKET) {
				if (zombieptr->isFrozen) {
					--zombieptr->isFrozen;
				}
				else {
					zombieptr->frameNo++;
					if (zombieptr->location > 350) {
						if (clo % 2 == 1)zombieptr->location -= zombieptr->speed;
					}
					else {
						if (clo % 2 == 1) {
							zombieptr->location -= zombieptr->speed;
							if (zombieptr->location % 4 == 0) {
								zombieptr->xspan -= zombieptr->speed;
							}
						}
					}
					if (zombieptr->frameNo > zombieptr->maxFrameW) {
						zombieptr->frameNo = 0;
					}
				}
			}
			else if (zombieptr->type == FOOTBALL) {
				if (zombieptr->isFrozen) {
					--zombieptr->isFrozen;
				}
				else {
					if (clo % 2 == 1)zombieptr->frameNo++;
					if (zombieptr->location > 350) {
						zombieptr->location -= zombieptr->speed;
						if (clo % 2 == 1)zombieptr->location -= zombieptr->speed;
					}
					else {
						zombieptr->location -= zombieptr->speed;
						if (zombieptr->location % 4 == 0) {
							zombieptr->xspan -= zombieptr->speed;
						}
					}
					if (zombieptr->frameNo > zombieptr->maxFrameW) {
						zombieptr->frameNo = 0;
					}
				}
			}
			else if (zombieptr->type == CATAPULT) {
				if (zombieptr->isFrozen) {
					--zombieptr->isFrozen;
				}
				else {
					if (clo % 2 == 1)zombieptr->frameNo++;
					if (zombieptr->frameNo > zombieptr->maxFrameW) {
						zombieptr->frameNo = 0;
					}
				}
			}
			else if (zombieptr->type == JOKER) {
				if (zombieptr->isFrozen) {
					--zombieptr->isFrozen;
				}
				else {
					if (clo % 2 == 1)zombieptr->frameNo++;
					if (zombieptr->location > 350) {
						if (clo % 2 == 1)zombieptr->location -= zombieptr->speed;
					}
					else {
						if (clo % 2 == 1) {
							zombieptr->location -= zombieptr->speed;
							if (zombieptr->location % 4 == 0) {
								zombieptr->xspan -= zombieptr->speed;
							}
						}
					}
					if (zombieptr->frameNo > zombieptr->maxFrameW) {
						zombieptr->frameNo = 0;
					}
				}
			}
		}
		else if (zombieptr->state == 1 && zombieptr->hp > 0) {
			switch (zombieptr->type) {
			case NORMAL:
				if (zombieptr->isFrozen) {
					tmpImg1 = normaleat[zombieptr->frameNo];
					zombieFrozen(&tmpImg2, &tmpImg1);
					transImage(NULL, zombieptr->location, xy_else_plants[zombieptr->row][8].y - zombieptr->xspan, &tmpImg2);
				}
				else {
					transImage(NULL, zombieptr->location, xy_else_plants[zombieptr->row][8].y - zombieptr->xspan, &normaleat[zombieptr->frameNo]);
				}
				break;
			case BARRICADE:
				if (zombieptr->isFrozen) {
					tmpImg1 = barricadeeat[zombieptr->frameNo];
					zombieFrozen(&tmpImg2, &tmpImg1);
					transImage(NULL, zombieptr->location, xy_else_plants[zombieptr->row][8].y - zombieptr->xspan, &tmpImg2);
				}
				else transImage(NULL, zombieptr->location, xy_else_plants[zombieptr->row][8].y - zombieptr->xspan, &barricadeeat[zombieptr->frameNo]);
				break;
			case BUCKET:
				if (zombieptr->isFrozen) {
					tmpImg1 = bucketeat[zombieptr->frameNo];
					zombieFrozen(&tmpImg2, &tmpImg1);
					transImage(NULL, zombieptr->location, xy_else_plants[zombieptr->row][8].y - zombieptr->xspan, &tmpImg2);
				}
				else transImage(NULL, zombieptr->location, xy_else_plants[zombieptr->row][8].y - zombieptr->xspan, &bucketeat[zombieptr->frameNo]);
				break;
			case FOOTBALL:
				if (zombieptr->isFrozen) {
					tmpImg1 = footballeat[zombieptr->frameNo];
					zombieFrozen(&tmpImg2, &tmpImg1);
					transImage(NULL, zombieptr->location - 450, xy_else_plants[zombieptr->row][8].y - zombieptr->xspan - 390, &tmpImg2);
				}
				else transImage(NULL, zombieptr->location - 450, xy_else_plants[zombieptr->row][8].y - zombieptr->xspan - 390, &footballeat[zombieptr->frameNo]);
				break;
			case JOKER:
				if (zombieptr->isFrozen) {
					tmpImg1 = jokereat[zombieptr->frameNo];
					zombieFrozen(&tmpImg2, &tmpImg1);
					transImage(NULL, zombieptr->location - 270, xy_else_plants[zombieptr->row][8].y - zombieptr->xspan - 420, &tmpImg2);
				}
				else transImage(NULL, zombieptr->location - 270, xy_else_plants[zombieptr->row][8].y - zombieptr->xspan - 420, &jokereat[zombieptr->frameNo]);
				break;
			}
			if (zombieptr->type >= NORMAL && zombieptr->type <= BUCKET) {
				if (zombieptr->isFrozen) {
					--zombieptr->isFrozen;
				}
				else {
					if (clo % 4 == 0)zombieptr->frameNo++;
					if (zombieptr->frameNo == zombieptr->maxFrameE) {
						mciSendString("play ./resource/music/eat.mp3 from 0", 0, 0, 0);
						zombieptr->frameNo = 0;
					}
				}
			}
			else if (zombieptr->type == FOOTBALL) {
				if (zombieptr->isFrozen) {
					--zombieptr->isFrozen;
				}
				else {
					if (clo % 2 == 0) {
						zombieptr->frameNo++;
					}
					if (zombieptr->frameNo == zombieptr->maxFrameE) {
						mciSendString("play ./resource/music/eat.mp3 from 0", 0, 0, 0);
						zombieptr->frameNo = 0;
					}
				}
			}
			else if (zombieptr->type == JOKER) {
				if (zombieptr->isFrozen) {
					--zombieptr->isFrozen;
				}
				else {
					if (clo % 2 == 0) {
						zombieptr->frameNo++;
					}
					if (zombieptr->frameNo == zombieptr->maxFrameE) {
						mciSendString("play ./resource/music/eat.mp3 from 0", 0, 0, 0);
						zombieptr->frameNo = 0;
					}
				}
			}
		}
		else if (zombieptr->type == JOKER && zombieptr->hp <= 0) {
			if (zombieptr->FrameBomb < 31) {
				transImage(NULL, zombieptr->location, xy_else_plants[zombieptr->row][8].y - zombieptr->xspan - 400, &jokerbomb[zombieptr->FrameBomb]);
				++zombieptr->FrameBomb;
			}
			else if (zombieptr->FrameBomb == 31) {
				transImage(NULL, zombieptr->location, xy_else_plants[zombieptr->row][8].y - zombieptr->xspan - 400, &jokerbomb[30]);
				++zombieptr->FrameBomb;
			}
			else if (zombieptr->FrameBomb > 31 && zombieptr->FrameBomb < 50) {
				transImage(NULL, zombieptr->location, xy_else_plants[zombieptr->row][8].y - zombieptr->xspan - 400, &jokerbomb[30]);
				++zombieptr->FrameBomb;
			}
			else {
				zombieptr->type = -1;
			}
		}
		cur = cur->next;
	}
	++clo;
}
void GameEngine()
{
	static clock_t clo = 0;
	Node<Zombie>* curZ = zombie_link.head, * nextZ = NULL;
	Node<Plants>* curP = plants_link.head, * nextP = NULL;
	while (curZ != NULL) {
		Zombie* zombieptr = curZ->context;
		zombieptr->hurt = 0;
		zombieptr->ishit = 0;
		curZ = curZ->next;
	}
	while (curP != NULL) {
		Plants* plantsptr = curP->context;
		plantsptr->eaten = 0;
		plantsptr->hurt = 0;
		curP = curP->next;
	}
	curZ = zombie_link.head;
	curP = plants_link.head;
	while (curP != NULL) {
		Plants* plantsptr = curP->context;
		if (plantsptr->hp <= 0) {
			if (plantsptr->type == 0) {
				Node<Plants>* tmp = plants_link.head;
				while (tmp != NULL) {
					if (tmp->context->x == plantsptr->x && tmp->context->y == plantsptr->y && tmp->context->type) {
						plants_link.DeleteNode(tmp->context->No);
						break;
					}
					tmp = tmp->next;
				}
				plants_state[plantsptr->x][plantsptr->y] = -1;
			}
			else if (plantsptr->type) {
				plants_state[plantsptr->x][plantsptr->y] = 0;
			}
			/*
			if (plantsptr->type) {
				plants_state[plantsptr->x][plantsptr->y] = 0;
			}
			else if (plantsptr->type == 0 && plants_state[plantsptr->x][plantsptr->y] == 0){
				plants_state[plantsptr->x][plantsptr->y] = -1;
			}
			*/
			plants_link.DeleteNode(plantsptr->No);
		}
		else {
			switch (plantsptr->type) {
			case CABBAGE:
			case WATERMELON:
			{
				curZ = zombie_link.head;
				Zombie* tmp = NULL;
				int minloc = 10000;
				while (curZ != NULL) {
					Zombie* zombieptr = curZ->context;
					if (zombieptr->row == plantsptr->x && zombieptr->location < minloc &&
						xy_pot[zombieptr->row][plantsptr->y].x < zombieptr->location) {
						tmp = zombieptr;
						minloc = tmp->location;
					}
					curZ = curZ->next;
				}
				if (tmp) {
					plantsptr->hitting = 1;
					tmp->ishit = 1;
					tmp->hurt += plantsptr->hit;
				}
				else {
					plantsptr->hitting = 0;
				}
				tmp = NULL;
			}
			}
		}
		curP = curP->next;
	}
	curZ = zombie_link.head;
	while (curZ != NULL) {
		Zombie* zombieptr = curZ->context;
		if (zombieptr->hp <= 0) {
			if (zombieptr->type == JOKER) {
				if (zombieptr->FrameBomb == 31) {
					curP = plants_link.head;
					while (curP != NULL) {
						Plants* plantsptr = curP->context;
						if (abs(zombieptr->location - xy_pot[zombieptr->row][plantsptr->y].x) <= 5 && abs(zombieptr->row - plantsptr->x) < 2) {
							plantsptr->hp = 0;
						}
						curP = curP->next;
					}
				}
			}
			else zombie_link.DeleteNode(zombieptr->No);
		}
		if (zombieptr->type == CATAPULT) {				// Dealing with Catapults Separately
			if (zombieptr->isFrozen) {
				--zombieptr->isFrozen;
				goto CATAPULT_END;
			}
			for (int j = 0; j < 8; j++) {
				if (plants_state[zombieptr->row][j] >= 0) {
					curP = plants_link.head;
					while (curP != NULL) {
						Plants* plantsptr = curP->context;
						if (plantsptr->x == zombieptr->row && plantsptr->y == j && plantsptr->type > 0) {
							plantsptr->eaten = 1;
							plantsptr->hurt++;
							goto CATAPULT_END;
						}
						/*
						else {
							plantsptr->eaten = 0;		// Catapult is too strong
							plantsptr->hurt = 0;		// Turn on weakening, same as below
						}
						*/
						curP = curP->next;
					}
					curP = plants_link.head;
					while (curP != NULL) {
						Plants* plantsptr = curP->context;
						if (plantsptr->x == zombieptr->row && plantsptr->y == j && plantsptr->type == 0) {
							plantsptr->eaten = 1;
							plantsptr->hurt++;
							goto CATAPULT_END;
						}
						/*
						else {
							plantsptr->eaten = 0;
							plantsptr->hurt = 0;		// Turn on weakening
						}
						*/
						curP = curP->next;
					}
				}
			}
		}
	CATAPULT_END:;
		for (int j = 0; j < 8; j++) {
			if (abs(zombieptr->location - xy_pot[zombieptr->row][j].x) <= 5 &&
				plants_state[zombieptr->row][j] >= 0) {
				goto JMP_RESET;
			}
		}
		zombieptr->state = 0;
	JMP_RESET:
		for (int j = 0; j < 8; j++) {
			if (abs(zombieptr->location - xy_pot[zombieptr->row][j].x) <= 5 &&
				plants_state[zombieptr->row][j] >= 0) {
				if (zombieptr->state == 0) {
					zombieptr->frameNo = 0;
				}
				zombieptr->state = 1;
				curP = plants_link.head;
				while (curP != NULL) {
					Plants* plantsptr = curP->context;
					if (plantsptr->x == zombieptr->row && abs(zombieptr->location - xy_pot[zombieptr->row][plantsptr->y].x) <= 5) {
						if (plantsptr->type == plants_state[plantsptr->x][plantsptr->y]) {
							plantsptr->eaten = 1;
							plantsptr->hurt++;
						}
						else {
							plantsptr->eaten = 0;				// invincible flowerpot bug
							plantsptr->hurt = 0;
						}
					}
					curP = curP->next;
				}
			}
		}
		curZ = curZ->next;
	}
	curZ = zombie_link.head;
	curP = plants_link.head;
	while (curZ != NULL) {
		Zombie* zombieptr = curZ->context;
		if (zombieptr->ishit && clo % 44 == 22) {
			zombieptr->hp -= zombieptr->hurt;
		}
		curZ = curZ->next;
	}
	while (curP != NULL) {
		Plants* plantsptr = curP->context;
		if (plantsptr->eaten && clo % 44 == 22) {
			plantsptr->hp -= plantsptr->hurt;
		}
		curP = curP->next;
	}
	++clo;
}