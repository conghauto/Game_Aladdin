#pragma once
#include "GameObject.h"
#include "define.h"
#include <math.h>
#include <fstream>
#include <sstream>
#include <algorithm>

class Cell
{
private:
	int id;
public:
	vector<LPGAMEOBJECT> listObject;
	vector<int>listIdObject;

	Cell() {

	}
	Cell(int id)
	{
		this->id = id;
	}

	void SetId(int id);
	vector<LPGAMEOBJECT> GetListObject();
	int getId() {
		return this->id;
	}

	void AddObject(LPGAMEOBJECT object);
	void RemoveObject(int i);
};

class Grid
{
	static Grid *instance;

	int cellCountInRow;
	int cellCountInColumn;
public:
	vector<Cell*> listCells;

	void InitList(float mapWidth, float mapHeight);
	void ReleaseList();

	void RemoveCell(int i);

	void AddObject(LPGAMEOBJECT object);
	void AddIdAtCell(int idCell,int idObject){
		listCells[idCell]->listIdObject.push_back(idObject);
	}
	void RemoveObject(LPGAMEOBJECT object);
	bool RemoveObjectInCell(LPGAMEOBJECT object);
	void InitObjectsAtCell(LPCSTR fileSource);

	static Grid *GetInstance();
	vector<Cell*> GetCurrentCells(float x, float y);
	void UpdateObjectInCell(LPGAMEOBJECT object);
};