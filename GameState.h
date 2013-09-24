#pragma once
#include "stdafx.h"
#include "State.h"
#include "MIDI_Objects.h"
#include "NoteBlock.h"
#include "MountainGen.h"

#ifdef _DEBUG
#define DEBUGCAMERA
#endif

class GameState:public State{
protected:
	MIDI::Song* song;
	vector<NoteBlock*> noteBlocks;
	vector<NoteBlock*>::iterator visibleNoteIterator;
	double
		songPos,
		songSpeed,
		songDelta,
		leftBoundary,
		rightBoundary;
#ifdef DEBUGCAMERA
	float cameraSpeed;
	unsigned char moveDir;
	bool debugCamera;
#endif
	bool
		running,
		paused,
		loop,
		done;
	glm::mat4 projectionMatrix;
	glm::vec2 cameraLook;
	glm::vec3 cameraLookVector;
	glm::vec3 cameraPosition;
	glm::mat4 pv;
	MountainGen mountainLayer1, mountainLayer2;

	void RebuildMatrix();
	virtual void SongDone(double currentTime);
public:
	virtual void Pause();
	virtual void UnPause();
	virtual void Begin(double currentTime);
	virtual void MouseMove(float x, float y);
	virtual void KeyPress(int button, int pressed);
	virtual void Render() override;
	virtual void Update(double currentTime, double deltaTime) override;
	GameState(string songFilename, bool loop = false);
	virtual ~GameState();
};