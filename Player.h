#pragma once
#include "StdAfx.h"
#include "RenderableObject.h"
#include "NoteBlock.h"

class Player:public RenderableObject{
protected:
	glm::vec2 velocity;
	glm::vec4 colour;
	glm::vec3 size;
	glm::mat4 modelMatrix;
	float
		stageBottom,
		stageTop,
		phaseTime;
	bool
		onGround,
		glide,
		phaseEnabled,
		phaseCollision;
	char
		moveDir;
	const static float
		gravity,
		jumpHeight,
		glideAmount,
		horizontalSpeed,
		nonGlidingVelocity,
		terminalVelocity,
		maxXVelocity,
		phaseDrainRate,
		phaseRechargeRate;
	/*const static float jumpHeight;
	const static float glideAmount;
	const static float horizontalSpeed;
	const static float nonGlidingVelocity;
	const static float terminalVelocity;
	const static float maxXVelocity;*/
	Mesh* mesh;
	DiffuseShader* shader;

	bool IsColliding(float x, float y, float left, float right, float top, float bottom);
public:
	unsigned int falls;
	glm::vec3 position;
	void KeyPress(int key, int state);
	virtual void Render(glm::mat4 pv);
	virtual void Update(double deltaTime, double currentTime, vector<NoteBlock*>::iterator& visibleNoteIterator, vector<NoteBlock*>::iterator& end, double songSpeed, double songPos, double songDelta, double leftBoundary);
	Player(State* state, double stateBottom, double stageTop);
	virtual ~Player();
};