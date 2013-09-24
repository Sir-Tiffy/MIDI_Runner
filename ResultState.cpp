#include "StdAfx.h"
#include "ResultState.h"
#include "MenuState.h"
#include "TextureHandler.h"
#include "NumberRenderer.h"
#include "Game.h"
#include "TextRenderer.h"

void ResultState::Render(){
	__super::Render(true);
	glUseProgram(0);
	glPushMatrix();
	glLoadIdentity();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture);
	glColor3f(1,1,1);
	glTranslatef(-1,-1,0);
	glScalef(2,2,1);
	glBegin(GL_QUADS);
	//you win
	glTexCoord2f(0,80/128.0f); glVertex2f(283/800.0f,534/600.0f);
	glTexCoord2f(235/512.0f,80/128.0f); glVertex2f(518/800.0f,534/600.0f);
	glTexCoord2f(235/512.0f,1); glVertex2f(518/800.0f,582/600.0f);
	glTexCoord2f(0,1); glVertex2f(283/800.0f,582/600.0f);

	//ok button
	if (overButton) glColor4fv((float*)&colours[RED]);
	glTexCoord2f(0.75f,91/128.0f); glVertex2f(.5f-91/800.0f/2,.2f);
	glTexCoord2f(440/512.0f,91/128.0f); glVertex2f(.5f+91/800.0f/2,.2f);
	glTexCoord2f(440/512.0f,1); glVertex2f(.5f+91/800.0f/2,.2f+56/600.0f);
	glTexCoord2f(0.75f,1); glVertex2f(.5f-91/800.0f/2,.2f+56/600.0f);
	glEnd();
	glPopMatrix();


	float scoreSize = .075f;
	stringstream scoreText;
	scoreText<<"Treasure: "<<setw(4)<<setfill('0')<<treasure<<"\n   Falls: "<<setw(4)<<setfill('0')<<falls<<"\n   Score: "<<setw(4)<<setfill('0')<<score;
	TextRenderer::RenderText(scoreText.str(),&colours[WHITE],.5f-14*(scoreSize/2)/2,.125f,scoreSize,true);
	TextRenderer::RenderText("Enter your name:",&colours[WHITE],.2f,.5f,.08f);
	stringstream name;
	name<<currentName;
	if (cursorOn && currentName.length() < maxNameLength) name<<'_';
	TextRenderer::RenderText(name.str(),&colours[WHITE],.5f-(.08f/2*maxNameLength)/2.0f,.5f+.08f,.08f);
	TextRenderer::CleanupRenderText();
}

void ResultState::KeyPress(int key, int pressed){
	if (pressed == GLFW_PRESS){
		if (key == 295 && currentName.length() > 0){ //backspace
			currentName.pop_back();
		} else if (key == 294) SubmitScore();
	}
}

void ResultState::CharPress(int c, int pressed){
	if (pressed == GLFW_PRESS){
		if (currentName.length() < maxNameLength) currentName.push_back(c);
	}
}

void ResultState::MouseMove(float x, float y){
	//__super::MouseMove(x,y);
	overButton = x >= .5f-91/800.0f && x <= .5f+91/800.f && y >= .2f && y <= .2f + 56/600.0f;
		/*	glTexCoord2f(0.75f,91/128.0f); glVertex2f(.5-91/800.0f/2,.2f);
	glTexCoord2f(440/512.0f,91/128.0f); glVertex2f(.5+91/800.0f/2,.2f);
	glTexCoord2f(440/512.0f,1); glVertex2f(.5+91/800.0f/2,.2f+56/600.0f);
	glTexCoord2f(0.75f,1); glVertex2f(.5-91/800.0f/2,.2f+56/600.0f);
		*/
}

void ResultState::SubmitScore(){
	if (currentName == "") return;
	ScoreStruct s = {score,currentName};
	Game::ChangeState(new ScoreState(songName, s, true));
}

void ResultState::MouseButton(int button, int pressed){
	if (overButton && button == GLFW_MOUSE_BUTTON_LEFT && pressed == GLFW_PRESS) return SubmitScore();
	//else __super::MouseButton(button,pressed);
}

void ResultState::Update(double currentTime, double deltaTime){
	cursorOn = (int)floor(currentTime*2)%2==0;
}

void ResultState::Begin(double currentTime){
	glfwEnable(GLFW_MOUSE_CURSOR);
}

ResultState::ResultState(ResultStruct& data){
	treasure = data.treasure;
	falls = data.falls;
	songName = data.songName;
	score = falls >= treasure ? 0 : treasure-falls;
	currentName = "";
	cursorOn = false;
	overButton = false;
}

ResultState::~ResultState(){
	TextureHandler::UnloadTexture(string("textures/scoreTexture.tga"));
}