#include <d3dx9.h>
#include <d3d9.h>
#include <vector>
#pragma once
#include <fstream>
#include <sstream>
#include <d3dx9.h>
#include <string.h>
#include "Sprites.h"
#include"Game.h"
#include "GameObject.h"
#include"define.h"
using namespace std;

#pragma once
class ObjectTile
{
public:
	int id;
	string name;
	float x, y;
	int width, height;

	float getX() { return x; }
	float getY() { return y; }

	ObjectTile();
	~ObjectTile();
};

class ListObject
{
public:
	vector<ObjectTile*>listObjects;

	vector<ObjectTile*> getObjectsInFile(LPCSTR fileSource);

	ListObject() {};
	~ListObject() {};
};


