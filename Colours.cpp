#pragma once
#include "StdAfx.h"

glm::vec3 GetColourFromHue(double hue){
	hue = hue - 6*2*PI/6*floor(hue/6/(2*PI/6));
	return glm::vec3(
		abs(hue*2*PI/6-3)-1,
		-abs(hue*2*PI/6-2)+2,
		-abs(hue*2*PI/6-4)+2
		);
}