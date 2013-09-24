#pragma once
#include "StdAfx.h"
#include "State.h"
#include "MountainGen.h"
#include "ScoreClass.h"

class ScoreState:public State{
protected:
	GLuint texture;
	void Render(bool ignore);
private:
	//vector<ScoreStruct> currentSongScores;
	string currentSongName;
	glm::mat4 projectionMatrix;
	short selectedSongIndex;
	unsigned char selectedButton;
	MountainGen mountainLayer1, mountainLayer2;
public:
	void Render() override;
	void MouseButton(int button, int state) override;\
	void MouseMove(float mousePosX, float mousePosY) override;
	void Update(double,double) override;
	void Begin(double currentTime) override;
	ScoreState(string songName = "", ScoreStruct s = ScoreStruct(), bool newScore = false);
	~ScoreState();
};