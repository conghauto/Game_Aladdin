﻿#include "Grid.h"

void Cell::GetPoint(float &left, float &right)
{
	left = this->left;
	right = this->right;
}

vector<LPGAMEOBJECT> Cell::GetListObject()
{
	return listObject;
}

void Cell::AddObject(LPGAMEOBJECT object)
{
	listObject.push_back(object);
}

void Cell::RemoveObject(int i)
{
	listObject.erase(listObject.begin() + i);
}

Grid *Grid::instance = NULL;



void Grid::InitList(float mapWidth,float mapHeight)
{
	Cell *cell;
	int cellX_Count = ceil(mapWidth / CELL_SIZE);
	int cellY_Count = ceil(mapHeight / CELL_SIZE);
	
	this->cellCountInRow = cellX_Count;

	for (int i = 0; i < cellY_Count; i++) {
		for (int j = 0; j < cellX_Count; j++)
		{
			float left, right;
			left = ( i + j )* CELL_SIZE;
			right = ( i + j + 1)* CELL_SIZE;
			cell = new Cell(left, right);
			listCells.push_back(cell);
		}
	}
}

void Grid::RemoveCell(int i)
{
	listCells.erase(listCells.begin() + i);
}

void Grid::AddObject(LPGAMEOBJECT object)
{
	// Dựa vào vị trí x, y của object để đặt vật vào cell tương ứng
	// Tránh trường hợp vật ra khỏi map: x<0, y<0
	float objectX = object->x < 0 ? 0 : object->x;
	float objectY = object->y < 0 ? 0 : object->y;

	int cellX_Number = floor(objectX / CELL_SIZE);
	int cellY_Number = floor(objectY / CELL_SIZE);

	// Tránh trường hợp vật ra khỏi map
	cellX_Number = (cellX_Number == this->listCells.size())? (this->listCells.size() - 1): cellX_Number;

	object->cellNumber = cellY_Number>0?(cellX_Number+(cellCountInRow-1)*cellY_Number): cellX_Number;
	int cellNumber = object->cellNumber;

	listCells[cellNumber]->AddObject(object);
}

void Grid::RemoveObject(LPGAMEOBJECT object)
{
	Cell *cell = listCells[object->cellNumber];
	int listObjectSize = cell->listObject.size();
	int objectNumber = -1;

	// Duyệt qua từng phần tử trong gridObjects để lấy chỉ số của object
	for (int i = 0; i < listObjectSize; i++)
	{
		if (cell->listObject[i] == object)
		{
			objectNumber = i;
			break;
		}
	}

	// Nếu tồn tại object trong gridObjects thì chỉ số sẽ khác -1
	if (objectNumber != -1)
	{
		cell->RemoveObject(objectNumber);
	}
}

Grid * Grid::GetInstance()
{
	if (instance == NULL) instance = new Grid();
	return instance;
}

vector<Cell*> Grid::GetCurrentCells(float x,float y)
{
	vector<Cell*> result;
	int cellX_Number = floor(x / CELL_SIZE);
	int cellY_Number = floor(y / CELL_SIZE);

	int cellCurrent = cellY_Number > 0 ? (cellX_Number + (cellCountInRow - 1)*cellY_Number) : cellX_Number;

	// Trường hợp màn hình game nằm giữa 2 Cell
	result.push_back(listCells[cellCurrent]);
	if (cellCurrent > 0)
		result.push_back(listCells[cellCurrent - 1]);
	if (cellCurrent + 1 < listCells.size())
		result.push_back(listCells[cellCurrent + 1]);

	return result;
}

void Grid::UpdateObjectInCell(LPGAMEOBJECT object)
{
	// Dựa vào vị trí x, y của object để đặt vật vào grid tương ứng
	int cellX_Number = floor(object->x / CELL_SIZE);
	int cellY_Number = floor(object->y / CELL_SIZE);

	int cellCurrent = cellY_Number > 0 ? (cellX_Number + (cellCountInRow - 1)*cellY_Number) : cellX_Number;

	if (cellCurrent < 0)
		cellCurrent = 0;
	else if (cellCurrent >= listCells.size())
		cellCurrent = listCells.size() - 1;

	if (cellCurrent != object->cellNumber)
	{
		// Xóa object khỏi grid cũ
		RemoveObject(object);

		// Đổi cellCurrent hiện tại của object và thêm object vào cell mới
		object->cellNumber = cellCurrent;
		listCells[cellCurrent]->AddObject(object);
	}
}

void Grid::ReleaseList()
{
	vector<LPGAMEOBJECT> listRemoveObjects;

	int GridSize = listCells.size();
	for (int i = 0; i < GridSize; i++)
	{
		int listObjectSize = listCells[i]->listObject.size();
		Cell *cell = listCells[i];
		for (int j = 0; j < listObjectSize; j++)
		{
			listRemoveObjects.push_back(cell->listObject[j]);
		}
	}

	for (int i = 0; i < listRemoveObjects.size(); i++)
	{
		RemoveObject(listRemoveObjects[i]);
		delete listRemoveObjects[i];
	}

	for (int i = 0; i < listCells.size(); )
	{
		Cell *cell = listCells[i];
		RemoveCell(0);
		delete cell;
	}
}