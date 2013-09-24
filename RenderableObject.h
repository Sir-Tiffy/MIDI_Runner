#pragma once
#include "stdafx.h"
#include "State.h"

class RenderableObject{
protected:
	State* parentState;

public:
	bool visible;
	virtual void Render();
	virtual void Update(double currentTime, double deltaTime);
	RenderableObject(State* state);
	virtual ~RenderableObject();
};
