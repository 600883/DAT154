#include "Car.h"
#include <Windows.h>
#include <string>
using namespace std;

Car::Car(string direction)
{
	colorR = rand() % 255;
	colorG = rand() % 255;
	colorB = rand() % 255;
	hCarBrush = CreateSolidBrush(RGB(colorR, colorG, colorB));

	if (direction == "left") {
		
		posLeft = 0;
		posTop = 240;
		posRight = 40;
		posBottom = 40;
	}
	else if (direction == "top") {

		posLeft = 540;
		posTop = 0;
		posRight = 560;
		posBottom = 40;
	}

}
