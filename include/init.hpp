#ifndef _INIT_HPP_
#define _INIT_HPP_

#include <graphics.h>
#include <algorithm>

#include "pvz.h"

class InitGame {
public:
	static InitGame& initGame() {
		static InitGame instance;
		return instance;
	}
private:
	InitGame() {
		initgraph(860, 600);
		loading();
		sliding_window();
		put_processbar();
		std::fill(&plants_state[0][0], &plants_state[0][0] + sizeof(plants_state) / sizeof(int), -1);
		init_pot_coordinate();
		init_else_plants_coordinate();
	}
	InitGame(InitGame const&) = delete;
	InitGame& operator=(InitGame const&) = delete;

	void loadImage(IMAGE pic[], int n, const char path[]) {
		char tmpPath[200], frameNo[4];
		for (int i = 0; i < n; i++) {
			strcpy_s(tmpPath, 200, path);
			strcat(strcat(tmpPath, itoa(i + 1, frameNo, 10)), ".png");
			loadimage(&pic[i], tmpPath);
		}
	}
	void loading() {
		loadimage(&background, "../../../resource/background/background6boss.jpg");
		loadimage(&progressbar[0], "../../../resource/progressbar/11.png");
		loadimage(&progressbar[1], "../../../resource/progressbar/22.png");
		loadimage(&progressbar[2], "../../../resource/progressbar/33.png");
		loadimage(&progressbar[3], "../../../resource/progressbar/44.png");
		loadimage(&progressbar[4], "../../../resource/progressbar/55.png");
		loadimage(&progressbar[5], "../../../resource/progressbar/66.png");
		loadimage(&pointer, "../../../resource/background/pointer.gif");

		loadimage(&card[0], "../../../resource/card/flowerpot_card.jpg");
		loadimage(&card[1], "../../../resource/card/cabbage_card.jpg");
		loadimage(&card[2], "../../../resource/card/corn_card.jpg");
		loadimage(&card[3], "../../../resource/card/watermelon_card.jpg");
		loadimage(&card[4], "../../../resource/card/hotpepper_card.jpg");
		loadimage(&card[5], "../../../resource/card/icemushroom_card.jpg");

		loadimage(&plants[0], "../../../resource/plants/flowerpot.png");
		loadimage(&plants[1], "../../../resource/plants/cabbage.png");
		loadimage(&plants[2], "../../../resource/plants/corn.png");
		loadimage(&plants[3], "../../../resource/plants/watermelon.png");
		loadimage(&plants[4], "../../../resource/plants/hotpepper.png");
		loadimage(&plants[5], "../../../resource/plants/icemushroom.png");
		loadimage(&corn_bomb, "../../../resource/plants/corn/corn_bomb.png");
		loadimage(&corn_crater, "../../../resource/plants/corn/corn_crater.png");
		loadimage(&fire[0], "../../../resource/plants/hotpepper/fire1.png");
		loadimage(&fire[1], "../../../resource/plants/hotpepper/fire2.png");
		loadimage(&fire[2], "../../../resource/plants/hotpepper/fire3.png");
		loadimage(&snow[0], "../../../resource/plants/icemushroom/snow1.png");
		loadimage(&snow[1], "../../../resource/plants/icemushroom/snow2.png");
		loadimage(&snow[2], "../../../resource/plants/icemushroom/snow3.png");

		loadImage(cabbage, 45, "../../../resource/plants/cabbage/");
		loadImage(watermolen, 47, "../../../resource/plants/watermelon/");
		loadImage(hotpepper, 29, "../../../resource/plants/hotpepper/");
		loadImage(normalwalk, 47, "../../../resource/zombie/normal_zomblie/walk/");
		loadImage(normaleat, 11, "../../../resource/zombie/normal_zomblie/eat/");
		loadImage(barricadewalk, 47, "../../../resource/zombie/barricade_zombie/walk/");
		loadImage(barricadeeat, 11, "../../../resource/zombie/barricade_zombie/eat/");
		loadImage(bucketwalk, 47, "../../../resource/zombie/buckethead_zombie/walk/");
		loadImage(bucketeat, 11, "../../../resource/zombie/buckethead_zombie/eat/");
		loadImage(footballwalk, 28, "../../../resource/zombie/football_zombie/walk/");
		loadImage(footballeat, 30, "../../../resource/zombie/football_zombie/eat/");
		loadImage(catapult, 45, "../../../resource/zombie/catapult_zombie/");
		loadImage(jokerwalk, 21, "../../../resource/zombie/joker_zombie/walk/");
		loadImage(jokereat, 24, "../../../resource/zombie/joker_zombie/eat/");
		loadImage(jokerbomb, 31, "../../../resource/zombie/joker_zombie/bomb/");
	}
	int sliding_speed1(int x) {
		double k = -1.0 / 7290;
		int speed = k * x * (x + 540) + 1;
		return speed;
	}
	int sliding_speed2(int x) {
		double k = -1.0 / 4000;
		int speed = k * (x + 210) * (x + 540) + 1;
		return speed;
	}
	void sliding_window() {
		cleardevice();
		int x = 0, y = 0;
		bool flag = 1;
		putimage(x, y, &background);
		Sleep(1000);
		cleardevice();
		x -= sliding_speed1(x);
		cleardevice();
		while (flag) {
			x -= sliding_speed1(x);
			putimage(x, y, &background);
			Sleep(20);
			if (x < -430) {
				putimage(x, y, &background);
				Sleep(1000);
				flag = 0;
			}
		}
		while (!flag) {
			x += sliding_speed2(x);
			putimage(x, y, &background);
			Sleep(20);
			if (x > -210) {
				putimage(x, y, &background);
				flag = 1;
			}
		}
	}
	void put_processbar() {
		int x = 150, y = -85, speed = 5;
		while (y != 5) {
			putimage(x, y, &progressbar[0]);
			HRGN rgn = CreateRectRgn(x + 8, y + 64, x + 508, y + 80);
			setcliprgn(rgn);
			DeleteObject(rgn);
			putimage(x + 8, y + 64, &whiteline);
			setcliprgn(NULL);
			y += speed;
			Sleep(20);
		}
	}
};

#endif
