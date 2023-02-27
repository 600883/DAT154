#pragma once
#include <Windows.h>
#include <string>
using namespace std;

class Car
{
private:
	string direction;

public:
	int posLeft;
	int posTop;
	int posRight;
	int posBottom;

	int colorR;
	int colorG;
	int colorB;
	HBRUSH hCarBrush;

	Car(string direction);
	
};

