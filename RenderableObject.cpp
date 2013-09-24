#include "stdafx.h"
#include "RenderableObject.h"

void RenderableObject::Render(){}
void RenderableObject::Update(double currentTime, double deltaTime){}

RenderableObject::RenderableObject(State* state){
	parentState = state;
}
RenderableObject::~RenderableObject(){}