#pragma once
#include "StdAfx.h"

class MountainGen{
protected:
	vector<float> data;
	unsigned int size;

	void Build(unsigned int start, unsigned int end, float roughness=1);
	void DrawData(unsigned int i, glm::vec3* bottomColour, glm::vec3* topColour, bool inverted);
public:
	void Render(glm::vec3* bottomColour, glm::vec3* topColour, bool floor);
	MountainGen(unsigned int size, float initial=.5f);
};