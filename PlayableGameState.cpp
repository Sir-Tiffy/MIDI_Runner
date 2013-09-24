#include "StdAfx.h"
#include "PlayableGameState.h"
#include "texturehandler.h"
#include "NumberRenderer.h"
#include "Game.h"
#include "ResultState.h"
#include "MenuState.h"
#include "TextRenderer.h"

int index = 0;

void PlayableGameState::KeyPress(int key, int state){
	if (started) __super::KeyPress(key,state);
	if (!paused 
#ifdef DEBUGCAMERA
		&& !debugCamera
#endif
		) player.KeyPress(key,state);
	if (key == 257 && state == GLFW_PRESS) Game::ChangeState(new MenuState(Game::InitialSong));
}

void PlayableGameState::MouseMove(float mouseX, float mouseY){
	__super::MouseMove(mouseX, mouseY);
	if (paused) mouseOverExitButton = mouseX >= 8/800.0f && mouseY >= 556/600.0f && mouseX <= 112/800.0f && mouseY <= 593/600.0f;
}

void PlayableGameState::MouseButton(int button, int state){
	if (mouseOverExitButton && button == GLFW_MOUSE_BUTTON_LEFT && state == GLFW_PRESS){
		Game::ChangeState(new MenuState(Game::InitialSong),true);
	}
}

void PlayableGameState::Render(){
	__super::Render();
	player.Render(pv);
	/*
	if (!started){
		glPushMatrix();
		glLoadIdentity();
		glUseProgram(0);
		glDepthFunc(GL_ALWAYS);
		glTranslatef(-.5,-.5,0);
		glScalef(.1f,.1f,.1f);
		NumberRenderer::RenderDigit((unsigned char)floor(countdown+1));
		glDepthFunc(GL_LESS);
		glPopMatrix();
	}*/

	glUseProgram(0);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(-1,-1,0);
	glScalef(2,2,1);
	glBindTexture(GL_TEXTURE_2D, texture);
	glEnable(GL_TEXTURE_2D);
	glColor3f(1,1,1);

	glBegin(GL_QUADS);/*
	//score
	glTexCoord2f(0,75/128.0f), glVertex2f(557/800.0f,556/600.0f);
	glTexCoord2f(1,75/128.0f), glVertex2f(685/800.0f,556/600.0f);
	glTexCoord2f(1,112/128.0f), glVertex2f(685/800.0f,593/600.0f);
	glTexCoord2f(0,112/128.0f), glVertex2f(557/800.0f,593/600.0f);
	//falls
	glTexCoord2f(0,0), glVertex2f(574/800.0f,499/600.0f);
	glTexCoord2f(110/128.0f,0), glVertex2f(685/800.0f,499/600.0f);
	glTexCoord2f(110/128.0f,36.0f/128.0f), glVertex2f(685/800.0f,536/600.0f);
	glTexCoord2f(0,36.0f/128.0f), glVertex2f(574/800.0f,536/600.0f);*/
	//menu
	if (paused){
	glTexCoord2f(0,37/128.0f), glVertex2f(8/800.0f,556/600.0f);
	glTexCoord2f(104/128.0f,37/128.0f), glVertex2f(112/800.0f,556/600.0f);
	glTexCoord2f(104/128.0f,74/128.0f), glVertex2f(112/800.0f,593/600.0f);
	glTexCoord2f(0,74/128.0f), glVertex2f(8/800.0f,593/600.0f);
	}
	glEnd();
	glPushMatrix();
	glTranslatef(696/800.0f,561/600.0f,0);
	glScalef(24/800.0f,32/600.0f,1);
	NumberRenderer::RenderDigit(score/1000);
	glTranslatef(1,0,0);
	NumberRenderer::RenderDigit((score%1000)/100);
	glTranslatef(1,0,0);
	NumberRenderer::RenderDigit((score%100)/10);
	glTranslatef(1,0,0);
	NumberRenderer::RenderDigit(score%10);
	glPopMatrix();
	glTranslatef(696/800.0f,504/600.0f,0);
	glScalef(24/800.0f,32/600.0f,1);
	unsigned int falls = player.falls;
	NumberRenderer::RenderDigit(falls/1000);
	glTranslatef(1,0,0);
	NumberRenderer::RenderDigit((falls%1000)/100);
	glTranslatef(1,0,0);
	NumberRenderer::RenderDigit((falls%100)/10);
	glTranslatef(1,0,0);
	NumberRenderer::RenderDigit(falls%10);
	glPopMatrix();

	TextRenderer::RenderText("Treasure:\n   Falls:",&colours[WHITE],.525f,0.015f,.075f,true);
	TextRenderer::CleanupRenderText();

	CheckError("Failed to render playable game state!");
	if (done){
		ResultStruct d = {songName,score,player.falls};
		Game::ChangeState(new ResultState(d));
	}
}

void PlayableGameState::SongDone(double currentTime){
	__super::SongDone(currentTime);
	//Game::ChangeStateMainThread(new MenuState(Game::InitialSong), true);
}

void PlayableGameState::Update(double currentTime, double deltaTime){
	if (!started){
		countdown -= (float)deltaTime;
		if (countdown <= 0){
			started = true;
			__super::Begin(currentTime);
			UnPause();
			glfwDisable(GLFW_MOUSE_CURSOR);
		}
	}
	
	__super::Update(currentTime,deltaTime);
	if (!paused){
		player.Update(currentTime, deltaTime, visibleNoteIterator, noteBlocks.end(), songSpeed, songPos, songDelta, leftBoundary);
#ifdef DEBUGCAMERA
		if (!debugCamera){
#endif
		cameraPosition.y += (float)(deltaTime*(player.position.y - cameraPosition.y)*3.0/4);
		RebuildMatrix();
#ifdef DEBUGCAMERA
		}
#endif
	}
}

void PlayableGameState::Begin(double currentTime){
	countdown = 2;
	score = 0;
	glfwDisable(GLFW_MOUSE_CURSOR);
	//__super::Begin(currentTime);
}

PlayableGameState::PlayableGameState(string filename):GameState(filename), player(this, 2*song->lowestNote, 2*song->highestNote), texture(TextureHandler::LoadTexture(string("./textures/gameTexture.tga"),GLFW_BUILD_MIPMAPS_BIT)){
	started = false;
	mouseOverExitButton = false;
	songName.assign(filename.begin()+filename.find_last_of("\\")+1,filename.end()-4);
}

PlayableGameState::~PlayableGameState(){
	TextureHandler::UnloadTexture(string("./textures/gameTexture.tga"));
}