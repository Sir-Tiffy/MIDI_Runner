#include "stdafx.h"
#include "State.h"

void State::Render(){}
void State::MouseButton(int button, int pressed){}

void State::MouseMove(float x, float y){
	mousePosX = x;
	mousePosY = y;
}

void State::Update(double currentTime, double deltaTime){}
void State::KeyPress(int button, int pressed){}
void State::CharPress(int c, int pressed){}

State::State(){}
State::~State(){}