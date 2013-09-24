#include "StdAfx.h"
#include "MountainGen.h"

void MountainGen::Build(unsigned int start, unsigned int end, float roughness){
	//if (end-start <= 0) return;
	int mid = (start+end)/2;
	if (mid == start) return;
	float average = (data[start]+data[end])/2.0f;
	float highest = max(data[start],data[end]);
	//float diff = highest-average;
	data[mid] = abs(average+ ((float)rand()/RAND_MAX-.5f)*roughness*2);
	roughness /= 2;
	Build(start,mid,roughness);
	Build(mid,end,roughness);
}

void MountainGen::DrawData(unsigned int i, glm::vec3* bottomColour, glm::vec3* topColour, bool inverted){
	glColor3fv((float*)(inverted?topColour:bottomColour));
	glVertex3f((float)cos(2*PI*i/size),inverted?data[i]:0,(float)sin(2*PI*i/size));
	glColor3fv((float*)(inverted?bottomColour:topColour));
	glVertex3f((float)cos(2*PI*i/size),inverted?0:data[i],(float)sin(2*PI*i/size));
}

void MountainGen::Render(glm::vec3* bottomColour, glm::vec3* topColour, bool floor){
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);

	DrawData(0,bottomColour,topColour, false);
	for (unsigned int i = 1; i < size; i++){
		DrawData(i,bottomColour,topColour, true);
		DrawData(i,bottomColour,topColour, false);
	}
	DrawData(0,bottomColour,topColour, true);

	if (floor){
		//floor
		glColor3fv((float*)bottomColour);
		glVertex3f(-1,0,-1);
		glVertex3f(1,0,-1);
		glVertex3f(1,0,1);
		glVertex3f(-1,0,1);
	}

	glEnd();
	CheckError("Failed to render mountains!");
}

MountainGen::MountainGen(unsigned int size, float initial){
	this->size = size;
	data.resize(size,initial);
	Build(0,size-1);
}