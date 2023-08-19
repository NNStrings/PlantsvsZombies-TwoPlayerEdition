#include "pvz.h"

#pragma warning (disable:4996)
#pragma comment( lib, "MSIMG32.LIB")
#pragma comment( lib, "winmm.lib")

size_t clik = 0;
// constexpr int millisecond = 20;
constexpr int millisecond = 10;

inline int sliding_speed1(int x) {
	double k = -1.0 / 7290;
	int speed = k * x * (x + 540) + 1;
	// int speed = k * x * (x + 540) + 30;
	return speed;
}
inline int sliding_speed2(int x) {
	double k = -1.0 / 4000;
	int speed = k * (x + 210) * (x + 540) + 1;
	// int speed = k * (x + 210) * (x + 540) + 30;
	return speed;
}
void sliding_window()
{
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

void put_processbar()
{
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
	/*
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
	*/
	sliding_processbar();
	PrintPlants();
	PrintZombie();
	if (clik % 100 == 0) {
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
	initgraph(860, 600);
	loading();
	sliding_window();
	put_processbar();
	std::fill(&plants_state[0][0], &plants_state[0][0] + sizeof(plants_state) / sizeof(int), -1);
	init_pot_coordinate();
	init_else_plants_coordinate();
	mciSendString("open ./resource/music/background.mp3 alias BGM1", 0, 0, 0);
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
			GameEngine();
			updataWindows();
		}
	}
	mciSendString("close BGM1", 0, 0, 0);
	closegraph();
	return 0;
}