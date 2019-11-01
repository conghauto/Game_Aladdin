#pragma once
#include "GameObject.h"
#include "define.h"
#include <math.h>

class Cell
{
	float left, right;
public:
	vector<LPGAMEOBJECT> listObject;

	Cell(float left, float right)
	{
		this->left = left;
		this->right = right;
	}

	void GetPoint(float &left, float &right);
	vector<LPGAMEOBJECT> GetListObject();

	void AddObject(LPGAMEOBJECT object);
	void RemoveObject(int i);
};

class Grid
{
	static Grid *instance;
	vector<Cell*> listCells;
	int cellCountInRow;
public:
	void InitList(float mapWidth, float mapHeight);
	void ReleaseList();

	void RemoveCell(int i);

	void AddObject(LPGAMEOBJECT object);
	void RemoveObject(LPGAMEOBJECT object);

	static Grid *GetInstance();
	vector<Cell*> GetCurrentCells(float x, float y);
	void UpdateObjectInCell(LPGAMEOBJECT object);
};