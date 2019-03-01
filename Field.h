#pragma once
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include "Function.h"
using namespace std;

class Field
{
protected:
	typedef map<int, vector<int>> container;
	container cd_of_wall;
public:
	int height, width, color, point;
	POINT begin_coord;
	Field()
	{
		begin_coord = { 2,2 };
		height = 20;
		width = 16;
		color = 2;
		point = 0;
		SetStartCdOfWall();
	};
	void DrawField()
	{
		int pos_y;
		SetPos(begin_coord.x + width + 2, begin_coord.y + 5);
		SetColor(0, 8);
		cout << "point: " << point;

		for (auto it = cd_of_wall.begin(); it != cd_of_wall.end(); ++it)
		{
			for (int i = 0; i < it->second.size(); i++)
			{
				(it->first == begin_coord.y) ? pos_y = it->first + 1 : pos_y = it->first;
				SetPos(it->second[i], pos_y);
				SetColor(color, 0);
				cout << " ";
			}
		}
		ClearWall();
	}
	void DrawNextObject(int *texture, int sz, int color)
	{
		int pos_x = begin_coord.x + width + 2;
		int pos_y = begin_coord.y + height - 6;
		SetPos(pos_x, pos_y - 2);
		SetColor(0, 8);
		cout << "next figure:";
		for (int i = 0; i < sz; i++)
		{
			for (int j = 0; j < sz; j++)
			{
				SetPos(pos_x + j, pos_y + i);
				(*texture++ == 1) ? SetColor(color, 0) : SetColor(0, 0);
				cout << " ";
			}
		}
	}
	bool GameOver()
	{
		if (GetTopOfWall() == height - 1 || GetTopOfWall() == -1)
		{
			cd_of_wall.clear();
			SetStartCdOfWall();
			DrawGameOver();
			point = 0;
			return true;
		}
		return false;
	}
	void SetCdOfWall(container cd)
	{
		container::iterator it = cd.begin();
		for (; it != cd.end(); ++it)
		{
			container::iterator f_it = cd_of_wall.find(it->first);
			if (f_it != cd_of_wall.end())
			{
				for (int i = 0; i < it->second.size(); i++)
				{
					f_it->second.push_back(it->second[i]);
				}
			}
			else {
				cd_of_wall.emplace(it->first, it->second);
			}
		}
	}
	bool IsFree(container cd)
	{
		container::iterator it = cd.begin();
		for (; it != cd.end(); ++it)
		{
			container::iterator f_it = cd_of_wall.find(it->first);
			if (f_it != cd_of_wall.end())
			{
				for (int i = 0; i < it->second.size(); i++)
				{
					if (find(f_it->second.begin(), f_it->second.end(), it->second[i]) != f_it->second.end())
						return false;
				}
			}
		}
		return true;
	}
	void ClearField()
	{
		for (int j = begin_coord.y + 2; j < begin_coord.y + height; j++)
		{
			for (int i = begin_coord.x + 1; i < begin_coord.x + width; i++)
			{
				SetPos(i, j);
				SetColor(0, 0);
				cout << " ";
			}
		}
	}
private:
	void ClearFieldSlow()
	{
		for (int j = begin_coord.y + height - 1; j > begin_coord.y + 1; j--)
		{
			for (int i = begin_coord.x + 1; i < begin_coord.x + width; i++)
			{
				Sleep(1);
				SetPos(i, j);
				SetColor(0, 0);
				cout << " ";
			}
		}
	}
	void DrawGameOver()
	{
		ClearFieldSlow();
		SetPos(begin_coord.x + width / 2 - 4, begin_coord.y + height / 2);
		SetColor(0, 8);
		cout << "GAME OVER!";
		SetPos(begin_coord.x + width / 2 - 6, begin_coord.y + height / 2 + 1);
		SetColor(0, 8);
		cout << "Press 'SPACE'";
	}
	void ClearWall() // Очищение фрагментов стены
	{
		container::iterator it = cd_of_wall.find(begin_coord.y + height - 1);
		int top = GetTopOfWall();
		if (it->second.size() > width)
		{
			ShiftCdOfWall(it, top);
			ClearField();
			point += 10;
		}
		else
		{
			int count = 3;
			it = cd_of_wall.find(begin_coord.y + height - top);
			for (; it != cd_of_wall.end(); ++it)
			{
				if (it->second.size() > width)
				{
					count = 3;
					for (; count > 0 && it != --cd_of_wall.end(); count--)
					{
						if (it->second.size() <= width) { break; }
						++it;
					}
				}
				if (count == 0)break;
			}
			if (count == 0)
			{
				container::iterator tmp;
				for (auto i = --it; count < 3; count++)
				{
					i->second.erase(i->second.begin() + 2, i->second.begin() + i->second.size());
					tmp = --i;
				}
				for (; tmp->second.size()>2; --tmp, --it)
				{
					it->second.insert(it->second.end(), tmp->second.begin() + 2, tmp->second.begin() + tmp->second.size());
					tmp->second.erase(tmp->second.begin() + 2, tmp->second.begin() + tmp->second.size());
				}
				ClearField();
				point += 10;
			}
		}
	}
	void ShiftCdOfWall(container::iterator it, int count)
	{
		for (int i = 0; i < count - 1; i++)
		{
			it->second.erase(it->second.begin() + 2, it->second.begin() + it->second.size());
			container::iterator it_n = cd_of_wall.find(it->first - 1);
			it->second.insert(it->second.end(), it_n->second.begin() + 2, it_n->second.begin() + it_n->second.size());
			--it;
		}
	}
	int GetTopOfWall()
	{
		for (auto it = --cd_of_wall.end(); it != cd_of_wall.begin(); --it)
		{
			if (it->second.size() == 2)return begin_coord.y + height - it->first;
		}
		return -1;
	}
	void SetStartCdOfWall()
	{
		vector<int> temp;
		for (int i = 0; i <= width; i++)
		{
			temp.push_back(begin_coord.x + i);
		}
		cd_of_wall.emplace(begin_coord.y, temp);
		cd_of_wall.emplace(begin_coord.y + height, temp);
		temp.clear();
		temp.push_back(begin_coord.x);
		temp.push_back(begin_coord.x + width);
		for (int i = 0; i < height; i++)
		{
			cd_of_wall.emplace(begin_coord.y + i, temp);
		}
	}
};

