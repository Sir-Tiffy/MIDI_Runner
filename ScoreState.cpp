#include "StdAfx.h"
#include "ScoreState.h"
#include "MenuState.h"
#include "TextureHandler.h"
#include "NumberRenderer.h"
#include "Game.h"
#include "TextRenderer.h"

void ScoreState::Render(){
	Render(false);
}

void ScoreState::Render(bool ignore){
	glPushMatrix();
	glLoadIdentity();
	glMultMatrixf((float*)&projectionMatrix);
	//mountains
	glScalef(512,512,512);
	glPushMatrix();
	glTranslatef(0,-.5f,0);
	mountainLayer1.Render(&glm::vec3(3/32.0f,3/16.0f,1/16.0f),&glm::vec3(1/16.0f,128.0f/255,0), true);
	glPopMatrix();
	glScalef(1.1f,1.1f,1.1f);
	glTranslatef(0,-.5f,0);
	mountainLayer2.Render(&glm::vec3(3/64.0f,3/32.0f,1/32.0f),&glm::vec3(1/32.0f,128.0f/512,0), false);
	
	//sky
	glLoadIdentity();
	glMultMatrixf((float*)&projectionMatrix);
	glScalef(768,768,768);
	glBegin(GL_QUADS);
	glColor3f(0,0,53.0f/255);
	glVertex3f(-1,-1,-1);
	glVertex3f(1,-1,-1);

	glColor3f(0,0,0);
	glVertex3f(1,1,-1);
	glVertex3f(-1,1,-1);
	glEnd();

	//stars
	glScalef(.9f,.9f,.9f);
	glTranslatef(0,0,0);
	glColor3f(1,1,1);
	const unsigned int NUM_STARS = 256;
	default_random_engine randGenerator;
	glBegin(GL_POINTS);
	for (unsigned int i = 0; i < NUM_STARS; i++){
		//float angle = (i/(NUM_STARS-1.0f)-.5f)*PI * 5/12 + PI;
		const float angle = (float)(((float)randGenerator()/randGenerator.max()-.5f)*PI * 5/12 + PI);
		glVertex3f((float)sin(angle),(float)randGenerator()/randGenerator.max(), (float)cos(angle));
	}
	glEnd();
	
	//gui
	glUseProgram(0);
	glLoadIdentity();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture);
	glColor3f(1,1,1);
	glTranslatef(-1,-1,0);
	glScalef(2,2,1);
	//you win
	/*glTexCoord2f(0,191/256.0f); glVertex2f(283/800.0f,534/600.0f);
	glTexCoord2f(256/256.0f,191/256.0f); glVertex2f(518/800.0f,534/600.0f);
	glTexCoord2f(256/256.0f,239/256.0f); glVertex2f(518/800.0f,582/600.0f);
	glTexCoord2f(0,239/256.0f); glVertex2f(283/800.0f,582/600.0f);*/
	//score
	/*glTexCoord2f(0,38/256.0f); glVertex2f(184/800.0f,243/600.0f);
	glTexCoord2f(216/256.0f,38/256.0f); glVertex2f(.5f,243/600.0f);
	glTexCoord2f(216/256.0f,190/256.0f); glVertex2f(.5f,395/600.0f);
	glTexCoord2f(0,190/256.0f); glVertex2f(184/800.0f,395/600.0f);*/
	
	if (!ignore){
	//menu button
		if (selectedButton == 1) glColor3f(1,0,0);
		glBegin(GL_QUADS);
		glTexCoord2f(0.5f,91/128.0f); glVertex2f(663/800.0f,32/600.0f);
		glTexCoord2f(361/512.0f,91/128.0f); glVertex2f(768/800.0f,32/600.0f);
		glTexCoord2f(361/512.0f,1); glVertex2f(768/800.0f,69/600.0f);
		glTexCoord2f(0.5f,1); glVertex2f(663/800.0f,69/600.0f);
		glEnd();
		if (selectedButton == 1) glColor3f(1,1,1);

	/*
	//fall numbers
	glTranslatef(.51f,246/600.0f,0);
	glScalef(24/800.0f,32/600.0f,1);
	glPushMatrix();

	NumberRenderer::RenderDigit(falls/1000);
	glTranslatef(1,0,0);
	NumberRenderer::RenderDigit((falls%1000)/100);
	glTranslatef(1,0,0);
	NumberRenderer::RenderDigit((falls%100)/10);
	glTranslatef(1,0,0);
	NumberRenderer::RenderDigit(falls%10);
	glPopMatrix();

	//score numbers
	glTranslatef(0,57/600.0f / (32/600.0f),0);
	glPushMatrix();
	
	NumberRenderer::RenderDigit(score/1000);
	glTranslatef(1,0,0);
	NumberRenderer::RenderDigit((score%1000)/100);
	glTranslatef(1,0,0);
	NumberRenderer::RenderDigit((score%100)/10);
	glTranslatef(1,0,0);
	NumberRenderer::RenderDigit(score%10);
	glPopMatrix();*/
		const float top = .975f;
		const float width = 330/800.0f;
		const float left = .5f-width/2;
		const float height = 48/600.0f;
		glBegin(GL_QUADS);
		glTexCoord2f(0,16/128.0f); glVertex2f(left,top-height); //bottom left
		glTexCoord2f(330/512.0f,16/128.0f); glVertex2f(left+width,top-height);
		glTexCoord2f(330/512.0f,.5f); glVertex2f(left + width,top);
		glTexCoord2f(0,.5f); glVertex2f(left,top);
		glEnd();

		stringstream scoreStream;
		scoreStream<<"Name";
		for (unsigned short i = 0; i < maxNameLength - 6; i++) scoreStream<<' ';
		scoreStream<<"Score";
		auto currentSongScores = ScoreClass::GetScores(currentSongName);
		for (auto i = currentSongScores.begin(); i != currentSongScores.end(); i++){
			ScoreStruct s = (*i);
			scoreStream<<'\n'<<s.name<<setw(20-s.name.length())<<s.score;
		}
		TextRenderer::RenderText(scoreStream.str(),&colours[WHITE],0.1f,.15f,1/(maxNumScores*1.5f),true);

		//song selection
		TextRenderer::RenderText("<",selectedButton == 2 ? &colours[RED] : &colours[WHITE],.1f,.9f,.1f);
		//const float filenameSize = .04f;
		const float filenameSize = 2*(.7f-(.1f+.1f/2))/currentSongName.length();
		TextRenderer::RenderText(currentSongName,&colours[WHITE],(.1f+.1f/2+.715f)/2-currentSongName.length()*filenameSize/2/2,.9f+.1f/2-filenameSize/2,filenameSize);
		TextRenderer::RenderText(">",selectedButton == 3 ? &colours[RED] : &colours[WHITE],.715f,.9f,.1f);

		TextRenderer::CleanupRenderText();
	}
	glPopMatrix();
	CheckError("Error rendering score screen");
}

void ScoreState::MouseButton(int button, int state){
	__super::MouseButton(button, state);
	if (state == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT){
		if (selectedButton == 1) Game::ChangeState(new MenuState(Game::InitialSong));
		else if (selectedButton) {
			unsigned short numSongs = ScoreClass::GetNumSongs();
			if (selectedButton == 2){
				if (--selectedSongIndex < 0) selectedSongIndex = numSongs - 1;
			} else if (selectedButton == 3){
				if (++selectedSongIndex >= numSongs) selectedSongIndex = 0;
			}
			short j = 0;
			auto names = ScoreClass::GetScoreNames();
			for (auto i = names.begin(); i != names.end(); i++){
				if (j++ == selectedSongIndex) currentSongName = (*i);
			}
		}
	}
}

void ScoreState::MouseMove(float mouseX, float mouseY){
	__super::MouseMove(mouseX, mouseY);
	selectedButton = 0;
	if (mouseX >= 663/800.0f && mouseY >= 32/600.0f && mouseX <= 768/800.0f && mouseY <= 69/600.0f) selectedButton = 1;
	else if (mouseY <= .08f && mouseY >= .02f){
		if (mouseX>= .1f && mouseX <= .1f+.1f/2) selectedButton = 2;
		else if (mouseX>= .715f && mouseX <= .715f+.1f/2) selectedButton = 3;
	}
}

void ScoreState::Update(double currentTime, double deltaTime){
	glfwEnable(GLFW_MOUSE_CURSOR);
}

void ScoreState::Begin(double currentTime){
	glfwEnable(GLFW_MOUSE_CURSOR);
}

ScoreState::ScoreState(string songName, ScoreStruct s, bool newScore):texture(TextureHandler::LoadTexture(string("textures/scoreTexture.tga"),GLFW_BUILD_MIPMAPS_BIT)), mountainLayer1(256), mountainLayer2(256){
	selectedButton = 0;
	projectionMatrix = glm::perspective(60.0f,4.0f/3,0.01f,1000.0f);
		selectedSongIndex = 0;
	if (songName == "" && ScoreClass::GetNumSongs() > 0) {
		currentSongName = ScoreClass::GetScoreNames()[0];
	} else {
		currentSongName = songName;
		auto names = ScoreClass::GetScoreNames();
		auto j = 0;
		for (auto i = names.begin(); i != names.end(); i++){
			if ((*i) == songName){
				selectedSongIndex = j;
				break;
			}
			j++;
		}
	}
	if (newScore) ScoreClass::InsertScore(songName, s);
	//currentSongScores = ScoreClass::GetScores(currentSongName);
}

ScoreState::~ScoreState(){
	TextureHandler::UnloadTexture(string("textures/scoreTexture.tga"));
}