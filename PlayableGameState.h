#pragma once
#include "StdAfx.h"
#include "GameState.h"
#include "Player.h"
#include "MountainGen.h"

class PlayableGameState:public GameState{
protected:
	Player player;
	float countdown;
	bool started;
	string songName;
	GLuint texture;
	bool mouseOverExitButton;
	void SongDone(double currentTime) override;
public:
	unsigned short score;
	virtual void KeyPress(int key, int state) override;
	virtual void MouseMove(float mouseX, float mouseY) override;
	virtual void MouseButton(int button, int state) override;
	virtual void Render() override;
	virtual void Update(double currentTime, double deltaTime) override;
	virtual void Begin(double currentTime) override;

	PlayableGameState(string filename);
	virtual ~PlayableGameState();
};