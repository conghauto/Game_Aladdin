#include "Grid.h"

void Cell::SetId(int id)
{
	this->id = id;
}

vector<LPGAMEOBJECT> Cell::GetListObject()
{
	return listObject;
}

void Cell::AddObject(LPGAMEOBJECT object)
{
	/*object->cellNumber = this->id;*/
	listObject.push_back(object);
}

void Cell::RemoveObject(int i)
{
	listObject.erase(listObject.begin() + i);
}

Grid *Grid::instance = NULL;



void Grid::InitList(float mapWidth,float mapHeight)
{
	
	int cellX_Count = ceil(mapWidth / CELL_SIZE);
	int cellY_Count = ceil(mapHeight / CELL_SIZE);
	
	this->cellCountInRow = cellX_Count;
	this->cellCountInColumn = cellY_Count;

	int count = 0;
	for (int i = 0; i < cellY_Count; i++) {
		for (int j = 0; j < cellX_Count; j++)
		{
			Cell *cell = new Cell(count);
			listCells.push_back(cell);
			count++;
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

/*	int cellX_Number = floor((x / CELL_SIZE));
	int cellY_Number = floor((y / CELL_SIZE));

	int cellCurrent = cellY_Number > 0 ? (cellX_Number + (cellCountInRow - 1)*cellY_Number) : cellX_Number*/;
	// Tránh trường hợp vật ra khỏi map
	cellX_Number = (cellX_Number == cellCountInRow)? (cellCountInRow - 1): cellX_Number;
	int cellCurrent = cellY_Number > 0 ? (cellX_Number + cellCountInRow * cellY_Number) : cellX_Number;

	//if (object->x+16 > (cellX_Number+1)*CELL_SIZE) {
	//	cellCurrent += 1;
	//}
	//else if(object->x -16 < (cellX_Number)*CELL_SIZE&&cellX_Number>0){
	//	cellCurrent -= 1;
	//}

	object->cellNumber = cellCurrent;

	listCells[cellCurrent]->AddObject(object);
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


void Grid::InitObjectsAtCell(LPCSTR fileSource)
{
	fstream pFile;
	pFile.open(fileSource, fstream::in);

	string lineString;
	int * posSpace = NULL;
	string subString;
	int countLine = 0;
	int i = 0;
	int idCell = 0;

	/*LPGAMEOBJECT object;*/

	while (pFile.good())
	{
		getline(pFile, lineString);
		size_t n = std::count(lineString.begin(), lineString.end(), ' ');
		
		posSpace = new int[n];
		for (int j = 0; j < n+1; j++)
		{
			if (j == 0)
			{
				posSpace[0] = lineString.find(" ");
				subString = lineString.substr(0, posSpace[0]);
				idCell = atoi(subString.c_str());
				//cell->SetId(idCell);
				//listCells.push_back(cell);
			}
			else
			{
				posSpace[j] = lineString.find(" ", posSpace[j - 1] + 1);
				subString = lineString.substr(posSpace[j - 1] + 1, posSpace[j] - (posSpace[j - 1] + 1));
				int id = atoi(subString.c_str());
				AddIdAtCell(idCell, id);
			}
		}
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
	int cellX_Number = floor((x / CELL_SIZE));
	int cellY_Number = floor((y / CELL_SIZE));

	int cellCurrent = cellY_Number > 0 ? (cellX_Number + cellCountInRow*cellY_Number) : cellX_Number;

	// Trường hợp màn hình game nằm giữa 2 Cell
	result.push_back(listCells[cellCurrent]);
	if (cellCurrent > 0){
		result.push_back(listCells[cellCurrent - 1]);
	}
	if (cellCurrent < cellCountInRow-1) {
		result.push_back(listCells[cellCurrent + 1]);
	}
	if (cellY_Number > 0&&cellY_Number< cellCountInColumn-1) {
		result.push_back(listCells[cellCurrent - cellCountInRow]);
	}

	if (cellY_Number==0 && cellY_Number < cellCountInColumn - 1) {
		result.push_back(listCells[cellCurrent + cellCountInRow]);
	}

	return result;
}

void Grid::UpdateObjectInCell(LPGAMEOBJECT object)
{
	// Dựa vào vị trí x, y của object để đặt vật vào grid tương ứng
	int cellX_Number = floor(object->x / CELL_SIZE);
	int cellY_Number = floor(object->y / CELL_SIZE);

	int cellCurrent = cellY_Number > 0 ? (cellX_Number + cellCountInRow * cellY_Number) : cellX_Number;

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

bool Grid::RemoveObjectInCell(LPGAMEOBJECT object) {

	if (object->cellNumber >= 0 && object->cellNumber <= 7) {
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
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
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
			if (cell->listObject[j]->name != "aladdin") {
				listRemoveObjects.push_back(cell->listObject[j]);
			}
			/*delete cell->listObject[j];*/
		}
	}

	for (int i = 0; i < listRemoveObjects.size(); i++)
	{
		if (RemoveObjectInCell(listRemoveObjects[i])) {

			delete listRemoveObjects[i];
		}
	}

	for (int i = 0; i < listCells.size(); )
	{
		Cell *cell = listCells[i];
		RemoveCell(0);
		delete cell;
	}
}
