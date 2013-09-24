#pragma once
#include "stdafx.h"

class State{
public:
	float
		mousePosX,
		mousePosY;

	virtual void Render();
	virtual void Begin(double currentTime)=0;
	virtual void Update(double currentTime, double deltaTime);
	virtual void KeyPress(int key, int pressed);
	virtual void CharPress(int c, int pressed);
	virtual void MouseButton(int button, int pressed);
	virtual void MouseMove(float x, float y);
	State();
	virtual ~State();
};