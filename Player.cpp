#include "StdAfx.h"
#include "Player.h"
#include "PlayableGameState.h"
#include "TextRenderer.h"

void Player::KeyPress(int key, int state){/*
	if (state == GLFW_PRESS){
		switch(key){
			case ' ':
				if (onGround) velocity.y += jumpHeight;
				glide = true;
				break;
			case 'A': moveDir = -1; break;
			case 'D': moveDir = 1; break;
		}
	} else {
		switch(key){
			case ' ':
				glide = false;
				if (velocity.y > nonGlidingVelocity) velocity.y = nonGlidingVelocity;
				break;
			case 'A':
				if (moveDir  == -1) moveDir = 0;
				break;
			case 'D':
				if (moveDir == 1) moveDir = 0;
				break;
		}
	}*/
	if (state == GLFW_PRESS){
		if (key == ' ' || key == 283 || key == 87){
			if (onGround) velocity.y += jumpHeight;
			glide = true;
		} /*else if (key == 'x' || key == 'X'){
			position.y = stageTop;
			velocity.x = velocity.y = position.x = 0;
			falls++;
		}*/
		if (key == 287 || key == 288){
			phaseEnabled = true;
		}
	} else {
		if (key == ' ' || key == 283 || key == 87){
			if (velocity.y > nonGlidingVelocity) velocity.y = nonGlidingVelocity;
			glide = false;
		} else if (key == 287 || key == 288){
			phaseEnabled = false;
			phaseCollision = true;
		}
	}
}

void Player::Render(glm::mat4 pv){
	shader->UseShader();
	mesh->LoadMesh();
	glm::vec4 newColour = colour;
	if (phaseEnabled) newColour.a = .5;
	shader->UploadUniforms(&(pv*modelMatrix), &newColour);
	mesh->Draw();
	glPushMatrix();
	glLoadIdentity();
	glScalef(2,2,1);
	glTranslatef(-.5f,-.5f,0);
	glUseProgram(0);
	glDepthFunc(GL_ALWAYS);
	glColor4fv((float*)&colours[DARKGREY]);
	const float barLeft = .75f;
	const float barWidth = .2f;
	const float barBottom = .05f;
	const float barHeight = .05f;
	glBegin(GL_QUADS);
	glVertex2f(barLeft,barBottom);
	glVertex2f(barLeft+barWidth,barBottom);
	glVertex2f(barLeft+barWidth,barBottom+barHeight);
	glVertex2f(barLeft,barBottom+barHeight);
	glEnd();
	glColor3fv((float*)&colours[GREY]);
	glBegin(GL_QUADS);
	glVertex2f(barLeft,barBottom);
	glVertex2f(barLeft+barWidth*phaseTime,barBottom);
	glVertex2f(barLeft+barWidth*phaseTime,barBottom+barHeight);
	glVertex2f(barLeft,barBottom+barHeight);
	glEnd();
	glPopMatrix();
	const float textSize = barHeight*3/5;
	TextRenderer::RenderText("PHASE POWER",&colours[BLUE],barLeft+barWidth/2-(textSize/2*11)/2,1-(barBottom+barHeight-textSize/2),textSize,true);
	TextRenderer::CleanupRenderText();
	glDepthFunc(GL_LESS);
}

bool Player::IsColliding(float x, float y, float left, float right, float top, float bottom){
	return left < x+size.x && right > x-size.x && y+size.y > bottom && y-size.y < top;
}

void Player::Update(double currentTime, double deltaTime, vector<NoteBlock*>::iterator& visibleNoteIterator, vector<NoteBlock*>::iterator& endIterator, double songSpeed, double songPos, double songDelta, double leftBoundary){
	if (phaseEnabled){
		if (phaseTime > 0){
			phaseTime -= (float)deltaTime*phaseDrainRate;
			if (phaseTime < 0){
				phaseTime = 0;
				phaseEnabled = false;
				phaseCollision = true;
			}
		}
	} else if (phaseTime < 1){
		phaseTime += (float)deltaTime*phaseRechargeRate;
		if (phaseTime > 1) phaseTime = 1;
	}
	float lastPosX = (float)(position.x - 2*songDelta/songSpeed);
	velocity.x = ((position.x<-maxXVelocity)?maxXVelocity:-position.x);
	velocity.y += gravity*(float)deltaTime;
	if (glide && velocity.y < 0) velocity.y += (float)(glideAmount*deltaTime);
	if (abs(velocity.y) > terminalVelocity){
		velocity.y = ((velocity.y<0) ? -terminalVelocity : terminalVelocity);
	}
	float newPosX = (float)(position.x + velocity.x*deltaTime);
	float newPosY = (float)(position.y + velocity.y*deltaTime);

	onGround = false;
	vector<NoteBlock*>::iterator noteIterator(visibleNoteIterator);
	
	bool correctCollision = true;
	while (noteIterator != endIterator){
		NoteBlock* block = *noteIterator;
		float left = (float)(block->position.x -2*songPos/songSpeed);
		if (left > newPosX+size.x) break;
		float right = (float)(left + block->length*2);
		float top = block->position.y + block->size.y;
		float bottom = block->position.y - block->size.y;

		if (block->hasCoin){
			glm::vec2 coinPos((left+right)/2,top+2);
			if (coinPos.x <= position.x+size.x && coinPos.x >= position.x-size.x && coinPos.y <= position.y + size.y && coinPos.y >= position.y - size.y){
				((PlayableGameState*)parentState)->score += block->coinScore;
				block->hasCoin = false;
			}
		}
		if (!phaseEnabled && IsColliding(newPosX, newPosY, left, right, top, bottom)) {
			if (phaseCollision && IsColliding(lastPosX, position.y, left, right, top, bottom)){
				correctCollision = false;
			} else {
				if (position.y + size.y > bottom && position.y - size.y < top){
					newPosX = left-size.x;
				} else {
					if (position.y < block->position.y)
						newPosY = bottom - size.y;
					else {
						newPosY = top + size.y;
						onGround = true;
					}
					velocity.y = 0;
				}
			}
		}
		noteIterator++;
	}
	if (correctCollision) phaseCollision = false;
	if (onGround) glide = false;

	if (newPosY < stageBottom || newPosX <= -1.6*leftBoundary){
		newPosY = stageTop;
		newPosX = 0;
		falls++;
	}

	position.x = (float)newPosX;
	position.y = (float)newPosY;

	modelMatrix = glm::scale(glm::translate(glm::mat4(1),position),size);
	colour = glm::vec4(GetColourFromHue(currentTime*2),1);
}

Player::Player(State* state, double stageBottom, double stageTop):RenderableObject(state){
	mesh = LoadMesh("models/beveled_cube_base.scm");
	shader = DiffuseShader::GetShader();
	size = glm::vec3(1,2,1);
	this->stageBottom = (float)stageBottom - size.y*8;
	this->stageTop = (float)stageTop+size.y*8;
	position = glm::vec3(0,stageTop,0);
	glide = false;
	moveDir = 0;
	falls = 0;
	phaseTime = 1;
	phaseEnabled = false;
	phaseCollision = false;
}

Player::~Player(){
	UnregisterMesh(mesh);
}

const float Player::gravity = -128;
const float Player::jumpHeight = 96;
const float Player::glideAmount = 48;
const float Player::horizontalSpeed = 32;
const float Player::nonGlidingVelocity = 32;
const float Player::terminalVelocity = 256;
const float Player::maxXVelocity = 24;
const float Player::phaseDrainRate = .75f;
const float Player::phaseRechargeRate = .25f;