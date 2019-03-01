#pragma once
#include <iostream>
#include <windows.h>
#include <sstream>
#include <time.h>
#include "Function.h"
#include "Pole.h"
using namespace std;

class Figure {
protected:
	POINT coord;
	time_t speed;
	time_t last_move;
	int color_bkgr;
	DIR dir;
	position pos;
public:
	Figure() : coord({0,0}), speed(), last_move(), color_bkgr(), dir(FALL) {};
	virtual void draw() = 0;
	virtual void clear() = 0;
	virtual bool move(Pole &p) = 0;
	virtual bool IsCollide(Pole p) = 0;
	virtual void FillPole(Pole &p) = 0; 
	virtual void setDir(DIR direct) { dir = direct; };
	virtual void setColor(int color) { color_bkgr = color; }
	virtual void setPosition(position p) { pos = p; }
	virtual void setCoord(POINT p) { coord = p; }
	virtual POINT getCoord()const { return coord; }
	virtual position getPosition()const { return pos; }
};

class Figure_one:virtual public Figure {	
public:
	Figure_one(int coord_x, int coord_y) {
		coord = { coord_x, coord_y };
		speed = 300;
		pos = first;
		color_bkgr = 2;
	}
	virtual void draw() override {
		if (pos == first) {
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 2; j++) {
					if (!((i == 0&&j == 1)||(i ==2&&j==0))) { 
						SetPos(coord.x+j, coord.y+i);
						SetColor(color_bkgr, 0);
						cout << " "; 
					}
				}
			}
		}
		else if (pos == second) {
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 2; j++) {
					if (!((i == 0 && j == 0) || (i == 2 && j == 1))) {
						SetPos(coord.x+i, coord.y+j);
						SetColor(color_bkgr, 0);
						cout << " ";
					}
				}
			}
		}
	}
	virtual void clear() override {
		if (pos == first) {
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 2; j++) {
					if (!((i == 0 && j == 1) || (i == 2 && j == 0))) {
						SetPos(coord.x + j, coord.y + i);
						SetColor(0, 0);
						cout << " ";
					}
				}
			}
		}
		else if (pos == second) {
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 2; j++) {
					if (!((i == 0 && j == 0) || (i == 2 && j == 1))) {
						SetPos(coord.x + i, coord.y + j);
						SetColor(0, 0);
						cout << " ";
					}
				}
			}
		}
	}
	virtual bool move(Pole &p) override {
		time_t now = clock();
		if (now - last_move >= speed) {
			setDir(DIR::FALL);
			if (IsCollide(p)) {
				FillPole(p);
				draw();
				color_bkgr = rand() % 10 + 5;
				coord.x = 10; coord.y = 4;
				//return false;
			}
			coord.y++;
			last_move = now;
		}
		if (!IsCollide(p)) {
			switch (dir) {
			case DIR::RIGHT:
					coord.x++;
				break;
			case DIR::LEFT:
					coord.x--;
				break;
			case DIR::CHANGE:
				if (pos == first&&coord.x + 2 == p.getRightWall())coord.x -= 1;
				(pos == first) ? pos = second : pos = first;
				break;
			case DIR::DOWN:
				coord.y++;
			}
		}
		return true;
	}	
	virtual bool IsCollide(Pole p) override {
		if (pos == first) {
			switch (dir) {
			case DIR::RIGHT:
				if (p.findWall(coord.x + 1, coord.y)|| p.findWall(coord.x + 2, coord.y + 1)||p.findWall(coord.x + 2, coord.y + 2))
					return true;
				break;
			case DIR::LEFT:
				if (p.findWall(coord.x - 1, coord.y) || p.findWall(coord.x - 1, coord.y + 1) || p.findWall(coord.x, coord.y + 2))
					return true;
				break;
			case DIR::CHANGE:
				if (p.findWall(coord.x + 2, coord.y) || p.findWall(coord.x + 2, coord.y+1))
					return true;
				break;
			case DIR::FALL:case DIR::DOWN:
				if (p.findWall(coord.x, coord.y + 2) || p.findWall(coord.x + 1, coord.y + 3))
					return true;
			}
		}
		else if (pos == second) {
			switch (dir) {
			case DIR::RIGHT:
				if (p.findWall(coord.x + 3, coord.y) || p.findWall(coord.x + 2, coord.y + 1))
				return true;
				break;
			case DIR::LEFT:
				if (p.findWall(coord.x, coord.y) || p.findWall(coord.x - 1, coord.y + 1))
				return true;
				break;
			case DIR::CHANGE:
				if (p.findWall(coord.x, coord.y) || p.findWall(coord.x + 1, coord.y + 2))
					return true;
				break;
			case DIR::FALL:case DIR::DOWN:
				if (p.findWall(coord.x, coord.y + 2) || p.findWall(coord.x + 1, coord.y + 2)||p.findWall(coord.x+2, coord.y+1))
					return true;
			}
		} return false;
		}
	virtual void FillPole(Pole & p) override {
		if (pos == first) {
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 2; j++) {
					if (!((i == 0 && j == 1) || (i == 2 && j == 0))) {
						p(coord.x + j, coord.y + i);
					}
				}
			}
		}
		else if (pos == second) {
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 2; j++) {
					if (!((i == 0 && j == 0) || (i == 2 && j == 1))) {
						p(coord.x + i, coord.y + j);
					}
				}
			}
		}
	}
};

