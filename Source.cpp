#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <conio.h>
#include <time.h>
#include <vector>
#include "Object.h"
#include "Function.h"
#include "Field.h"
#include <time.h>
using namespace std;
#pragma warning(disable:4996)
class LC { public: LC() { system("chcp 1251 > null");  srand(time(0)); } ~LC() { cin.get(); cin.get(); } }l;


DIR get_event() {
	char key;
	if (_kbhit()) {
		key = _getch();
		/*if (key == KEYS::CURSOR1 || key == KEYS::CURSOR2) {
		key = _getch();
		}  */
		switch (key) {
		case KEYS::_UP: return DIR::UP;
		case KEYS::_DOWN: return DIR::DOWN;
		case KEYS::_LEFT: return DIR::LEFT;
		case KEYS::_RIGHT: return DIR::RIGHT;
		case KEYS::ESC: return DIR::QUIT;
		case KEYS::ENTER: return DIR::CHANGE;
		case KEYS::X: case KEYS::_X: return DIR::FALL;
		}
	}
	return DIR::FALL;
}

int main() {

	Field f;
	DIR dir;
	POINT p = { f.begin_coord.x + f.width / 2, f.begin_coord.y };
	Object *obj = new Third_obj(p);
	Object *next_obj = new Second_obj(p);
	CursorHide();
	start(f.begin_coord.y + f.height + 2, f.begin_coord.x + f.width + f.width);
	while (true) {
		f.DrawField();
		dir = get_event();
		switch (dir) {
		case DIR::LEFT:case DIR::DOWN:case DIR::UP:case DIR::RIGHT:case DIR::CHANGE:
			obj->SetDirect(dir); break;
		case DIR::FALL:
			obj->SetDirect(dir); break;
		}
		obj->Draw();
		if (obj->Move(f)) {
			obj = next_obj;
			int i = rand() % 5;
			if (i == 0)next_obj = new First_obj(p);
			else if (i == 1) next_obj = new Second_obj(p);
			else if (i == 2) next_obj = new Third_obj(p);
			else if (i == 3) next_obj = new Forth_obj(p);
			else if (i == 4) next_obj = new Fifth_obj(p);
		};
		f.DrawNextObject(next_obj->GetTexture(), 3, next_obj->GetColor());
		if (f.GameOver())
		{
			bool b = true;
			while (b)
			{
				char d = _getch();
				switch (d)
				{
				case KEYS::SPACE: f.ClearField(); b = false;
				}
			}
		}
	}
}



