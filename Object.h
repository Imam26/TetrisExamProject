#pragma once
#include <iostream>
#include <map>
#include <vector>
#include "Function.h"
#include "Field.h"
#include <algorithm>
using namespace std;

class Object {
private:
	map<int, vector<int>> cheking_coord;
	map<int, vector<int>> next_coord;
protected:
	int top;
	POINT coord, prev_coord;
	int * texture, *prev_texture;
	DIR direct;
	time_t speed, last_time;
	int height, width, color;
public:
	Object()
	{
		speed = 300;
	};
	virtual ~Object()
	{
		if (texture)delete[]texture;
	}
	virtual void Draw()
	{
		int * temp_texture = texture;
		int * temp_prev_texture = prev_texture;
		for (int i = 0; i<height; i++)
		{
			for (int j = 0; j<width; j++)
			{
				if (*prev_texture++ == 1 && prev_coord.y + i>top + 1)
				{
					map<int, vector<int>> ::iterator it = cheking_coord.find(prev_coord.y + i);
					if (it == cheking_coord.end())
					{
						SetPos(prev_coord.x + j, prev_coord.y + i);
						SetColor(0, 0);
						cout << " ";
					}
					else
					{
						vector<int> ::iterator it_v = find(it->second.begin(), it->second.end(), prev_coord.x + j);
						if (it_v == it->second.end())
						{
							SetPos(prev_coord.x + j, prev_coord.y + i);
							SetColor(0, 0);
							cout << " ";
						}
					}
				}
				if (*texture++ == 1 && coord.y + i>top + 1)
				{
					SetPos(coord.x + j, coord.y + i);
					SetColor(color, 0);
					cout << " ";
				}
			}
		}
		texture = temp_texture;
		prev_texture = temp_prev_texture;
	}
	virtual bool Move(Field & f)
	{
		prev_texture = texture;
		prev_coord = coord;
		time_t now = clock();
		if (now - last_time >= speed)
		{
			if (!IsCollide(f, coord.x, coord.y + 1)) coord.y++;
			else
			{
				SetChekingCoord(coord.x, coord.y);
				f.SetCdOfWall(cheking_coord);
				SetCoord(f.begin_coord.x + f.width / 2, f.begin_coord.y);
				return true;
			}
			last_time = now;
		}
		switch (direct) {
		case DIR::LEFT:
			if (!IsCollide(f, coord.x - 1, coord.y)) coord.x--; break;
		case DIR::RIGHT:
			if (!IsCollide(f, coord.x + 1, coord.y)) coord.x++; break;
		case DIR::DOWN:
			if (!IsCollide(f, coord.x, coord.y + 1)) coord.y++; break;
		case DIR::CHANGE:
			ChangeTexture();
			if (IsCollide(f, coord.x, coord.y))SetTexture(prev_texture);
			break;
		}
		return false;
	}
	virtual bool IsCollide(Field f, int coord_x, int coord_y)
	{
		SetChekingCoord(coord_x, coord_y);
		return !f.IsFree(cheking_coord);
	}
	virtual void ChangeTexture() = 0;
	virtual void SetTexture(int * text) { texture = text; }
	virtual void SetPrevTexture(int * text) { prev_texture = text; }
	virtual void SetDirect(DIR dir) { direct = dir; }
	virtual void SetCoord(int coord_x, int coord_y) { coord = { coord_x, coord_y }; }
	virtual int* GetTexture()const { return texture; }
	virtual int GetColor()const { return color; };
protected:
	virtual void SetChekingCoord(int coord_x, int coord_y)
	{
		if (!cheking_coord.empty())cheking_coord.clear();
		vector<int> temp_v;
		int * temp_texture = texture;
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				if (*texture++ == 1)temp_v.push_back(coord_x + j);
			}
			cheking_coord.emplace(coord_y + i, temp_v);
			temp_v.clear();
		}
		texture = temp_texture;
	}
};

class First_obj : public Object {
private:
	int textOne[3][3] = { { 0,0,0 },{ 0,1,0 },{ 1,1,1 } };
	int textTwo[3][3] = { { 1,0,0 },{ 1,1,0 },{ 1,0,0 } };
	int textThree[3][3] = { { 0,0,0 },{ 1,1,1 },{ 0,1,0 } };
	int textFour[3][3] = { { 0,0,1 },{ 0,1,1 },{ 0,0,1 } };
public:
	First_obj(POINT cd)
	{
		coord = { cd.x, cd.y };
		top = cd.y;
		height = width = 3;
		color = 5;
		SetTexture((int*)textOne);
		SetPrevTexture((int*)textOne);
	}
	virtual void ChangeTexture() override
	{
		if (texture == (int*)textOne)texture = (int*)textTwo;
		else if (texture == (int*)textTwo)texture = (int*)textThree;
		else if (texture == (int*)textThree)texture = (int*)textFour;
		else if (texture == (int*)textFour)texture = (int*)textOne;
	}
};

class Second_obj : public Object {
private:
	int textOne[3][3] = { { 1,0,0 },{ 1,1,0 },{ 0,1,0 } };
	int textTwo[3][3] = { { 0,1,1 },{ 1,1,0 },{ 0,0,0 } };
public:
	Second_obj(POINT cd)
	{
		coord = { cd.x, cd.y };
		top = cd.y;
		height = width = 3;
		color = 3;
		SetTexture((int*)textOne);
		SetPrevTexture((int*)textOne);
	}
	virtual void ChangeTexture() override
	{
		if (texture == (int*)textOne)texture = (int*)textTwo;
		else if (texture == (int*)textTwo)texture = (int*)textOne;
	}
};

class Third_obj : public Object {
private:
	int textOne[3][3] = { { 1,1,0 },{ 1,1,0 },{ 0,0,0 } };
public:
	Third_obj(POINT cd)
	{
		coord = { cd.x, cd.y };
		top = cd.y;
		height = 3;
		width = 3;
		color = 8;
		SetTexture((int*)textOne);
		SetPrevTexture((int*)textOne);
	}
	virtual void ChangeTexture() override {}
};

class Forth_obj : public Object {
private:
	int textOne[3][3] = { { 0,0,0 },{ 1,1,1 },{ 0,0,0 } };
	int textTwo[3][3] = { { 0,1,0 },{ 0,1,0 },{ 0,1,0 } };
public:
	Forth_obj(POINT cd)
	{
		coord = { cd.x, cd.y };
		top = cd.y;
		height = 3;
		width = 3;
		color = 9;
		SetTexture((int*)textOne);
		SetPrevTexture((int*)textOne);
	}
	virtual void ChangeTexture() override
	{
		if (texture == (int*)textOne)texture = (int*)textTwo;
		else if (texture == (int*)textTwo)texture = (int*)textOne;
	}
};


class Fifth_obj : public Object {
private:
	int textOne[3][3] = { { 0,1,0 },{ 1,1,0 },{ 1,0,0 } };
	int textTwo[3][3] = { { 1,1,0 },{ 0,1,1 },{ 0,0,0 } };
public:
	Fifth_obj(POINT cd)
	{
		coord = { cd.x, cd.y };
		top = cd.y;
		height = width = 3;
		color = 10;
		SetTexture((int*)textOne);
		SetPrevTexture((int*)textOne);
	}
	virtual void ChangeTexture() override
	{
		if (texture == (int*)textOne)texture = (int*)textTwo;
		else if (texture == (int*)textTwo)texture = (int*)textOne;
	}
};

/*class Sixth_obj : public Object {
private:
int textOne[3][3] = { { 1,1,0},{ 1,0,0 },{ 1,0,0 }};
int textTwo[3][3] = { { 0,1,1 },{ 0,0,1 },{ 0,0,1 }, };
int textThree[3][3] = { { 0,0,1 },{ 0,0,1 },{ 0,1,1 } };
int textFour[3][3] = { { 0,0,0 },{ 1,0,0 },{ 1,1,1 } };
public:
Sixth_obj(POINT cd)
{
coord = { cd.x, cd.y };
height = width = 3;
color = 11;
SetTexture((int*)textOne);
SetPrevTexture((int*)textOne);
}
virtual void ChangeTexture() override
{
if (texture == (int*)textOne)texture = (int*)textTwo;
else if (texture == (int*)textTwo)texture = (int*)textThree;
else if (texture == (int*)textThree)texture = (int*)textFour;
else if (texture == (int*)textFour)texture = (int*)textOne;
}
};*/