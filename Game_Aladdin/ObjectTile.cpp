#include "ObjectTile.h"



ObjectTile::ObjectTile()
{
}

vector<ObjectTile*>ListObject::getObjectsInFile(LPCSTR fileSource)
{
	vector<ObjectTile*>result;
	fstream pFile;
	pFile.open(fileSource, fstream::in);

	string lineString;
	int * posSpace = NULL;
	string subString;
	int countLine = 0;
	int i = 0;

	/*LPGAMEOBJECT object;*/

	while (pFile.good())
	{
		ObjectTile*object = new ObjectTile();
		getline(pFile, lineString);
		std::istringstream iss(lineString);

		int id;
		float x, y;
		string name = "";
		string col1 = "", col2 = "", col3 = "", col4 = "", col5 = "", col6 = "";

		if (!(iss >> col1 >> col2 >> col3 >> col4 >> col5 >> col6)) { break; }

		object->id = atoi(col1.c_str());
		object->name = col2;
		object->x = (float)(::atof(col3.c_str()));
		object->y = 1024 - (float)(::atof(col4.c_str()));
		if (object->name != "") {
			result.push_back(object);
		}
	}
	return result;
}


ObjectTile::~ObjectTile()
{
}
