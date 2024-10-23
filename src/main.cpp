#include <future>

#include "pvz.h"
#include "music.hpp"
#include "init.hpp"

#pragma warning (disable:4996)
#pragma comment( lib, "MSIMG32.LIB")
#pragma comment( lib, "winmm.lib")

size_t clik = 0;
constexpr int millisecond = 10;

void sliding_processbar()
{
	int x = 150, y = 0;
	static int bar_frame = 0;
	putimage(x, y, &progressbar[bar_frame++]);
	HRGN rgn = CreateRectRgn(157, 6, 660, 80);
	setcliprgn(rgn);
	DeleteObject(rgn);
	convey_card.move_card();
	setcliprgn(NULL);
	if (bar_frame == 6) {
		bar_frame = 0;
	}
}
void updataWindows()
{
	BeginBatchDraw();
	putimage(-195, 0, &background);
#ifndef NDEBUG
	setlinecolor(GREEN);

	line(0, 293, 470, 180);
	line(0, 210, 470, 97);
	line(0, 380, 470, 265);
	line(0, 463, 470, 350);
	line(0, 550, 470, 437);
	line(0, 635, 470, 522);

	line(42, 0, 42, 600);
	line(142, 0, 142, 600);
	line(226, 0, 226, 600);
	line(307, 0, 307, 600);
	line(385, 0, 385, 600);
	line(470, 0, 470, 600);

	line(0, 93, 860, 93);
	line(0, 173, 860, 173);
	line(0, 260, 860, 260);
	line(0, 345, 860, 345);
	line(0, 425, 860, 425);
	line(0, 515, 860, 515);

	line(550, 0, 550, 600);
	line(624, 0, 624, 600);
	line(710, 0, 710, 600);
#endif
	sliding_processbar();
	PrintPlants();
	PrintZombie();
	if (clik % 200 == 0) {
		convey_card.generate_card();
	}
	if (cur_plants >= 0) {
		transImage(NULL, cur_x, cur_y, &plants[cur_plants]);
	}
	++clik;
	EndBatchDraw();
}

int main()
{
	srand((unsigned)time(NULL));
	InitGame::initGame();
	mciSendString("open ../../../resource/music/background.mp3 alias BGM1", 0, 0, 0);
	mciSendString("play BGM1 repeat", 0, 0, 0);
	int timer = 0;
	bool flag = true;
	while (1) {
		convey_card.click_card();
		get_keyboard();
		timer += getDelay();
		if (timer > millisecond) {
			flag = true;
			timer = 0;
		}
		if (flag) {
			flag = false;
			auto gameTask = std::async(std::launch::async, GameEngine);
			auto updataTask = std::async(std::launch::async, updataWindows);
			gameTask.get();
			updataTask.get();
		}
	}
	return 0;
}