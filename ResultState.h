#pragma once
#include "StdAfx.h"
#include "ScoreState.h"

struct ResultStruct{
	string songName;
	unsigned short treasure;
	unsigned short falls;
};

class ResultState:public ScoreState{
protected:
	unsigned short score, treasure, falls;
	string currentName;
	string songName;
	bool cursorOn;
	bool overButton;
	void SubmitScore();
public:
	void Render() override;
	void CharPress(int c, int pressed) override;
	void KeyPress(int c, int pressed) override;
	void MouseMove(float x, float y) override;
	void MouseButton(int button, int press) override;
	void Update(double,double) override;
	void Begin(double currentTime) override;
	ResultState(ResultStruct& score);
	~ResultState();
};