#include "StdAfx.h"
#include "MenuState.h"
#include "TextureHandler.h"
#include "PlayableGameState.h"
#include "Game.h"
#include "TextRenderer.h"
#include "ScoreState.h"

void MenuState::Render(){
	__super::Render();
	glUseProgram(0);
	glPushMatrix();
	glLoadIdentity();
	glScalef(2,2,1);
	glTranslatef(-.5f,-.5f,0);

	glColor3fv((float*)&titleColour);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, menuTexture);
	glDepthFunc(GL_ALWAYS);
	glBegin(GL_QUADS);
	//title

	glTexCoord2f(43/1024.0f,23/128.0f/2+.5f);  glVertex2f(123/800.0f,491/600.0f);
	glTexCoord2f(595/1024.0f,23/128.0f/2+.5f);  glVertex2f(675/800.0f,491/600.0f);
	glTexCoord2f(595/1024.0f,102/128.0f/2+.5f);  glVertex2f(675/800.0f,571/600.0f);
	glTexCoord2f(43/1024.0f,102/128.0f/2+.5f);  glVertex2f(123/800.0f,571/600.0f);
	
	
	//instructions
	glColor3fv((float*)&normalColour);
	/*
	glTexCoord2f(0,0);  glVertex2f(377/800.0f,170/600.0f);
	glTexCoord2f(407/1024.0f,0);  glVertex2f(784/800.0f,170/600.0f);
	glTexCoord2f(407/1024.0f,150/256.0f);  glVertex2f(784/800.0f,322/600.0f);
	glTexCoord2f(0,150/256.0f);  glVertex2f(377/800.0f,322/600.0f);*/

	//name
	glTexCoord2f(689/1024.0f,190/256.0f); glVertex2f(219/800.0f,417/600.0f);
	glTexCoord2f(1,190/256.0f); glVertex2f(554/800.0f,417/600.0f);
	glTexCoord2f(1,239/256.0f); glVertex2f(554/800.0f,466/600.0f);
	glTexCoord2f(689/1024.0f,239/256.0f); glVertex2f(219/800.0f,466/600.0f);

	//play
	if (mouseOver == 1) glColor3fv((float*)&selectColour);
	glTexCoord2f(.75f,0/2+.5f);  glVertex2f(32/800.0f,192/600.0f);
	glTexCoord2f(.75+128/1024.0f,0/2+.5f);  glVertex2f((32+128)/800.0f,192/600.0f);
	glTexCoord2f(.75+128/1024.0f,.5f/2+.5f);  glVertex2f((32+128)/800.0f,256/600.0f);
	glTexCoord2f(.75f,.5f/2+.5f);  glVertex2f(32/800.0f,256/600.0f);
	if (mouseOver == 1) glColor3fv((float*)&normalColour);

	//high scores
	if (mouseOver == 2) glColor3fv((float*)&selectColour);
	glTexCoord2f(41/1024.0f,57/256.0f);  glVertex2f(32/800.0f,200/600.0f-64/600.0f);
	glTexCoord2f(41/1024.0f+332/1024.0f,57/256.0f);  glVertex2f((32+332)/800.0f,200/600.0f-64/600.0f);
	glTexCoord2f(41/1024.0f+332/1024.0f,57/256.0f+49/256.0f);  glVertex2f((32+332)/800.0f,200/600.0f-64/600.0f+49/600.0f);
	glTexCoord2f(41/1024.0f,57/256.0f+49/256.0f);  glVertex2f(32/800.0f,200/600.0f-64/600.0f+49/600.0f);
	if (mouseOver == 2) glColor3fv((float*)&normalColour);

	//credits
	/*if (mouseOver == 2) glColor3fv((float*)&selectColour);
	glTexCoord2f(.75f,0.5f/2+.5f);  glVertex2f(32/800.0f,192/600.0f-64/600.0f);
	glTexCoord2f(.75+256/1024.0f,0.5f/2+.5f);  glVertex2f(256/800.0f,192/600.0f-64/600.0f);
	glTexCoord2f(.75+256/1024.0f,1.0f/2+.5f);  glVertex2f(256/800.0f,256/600.0f-64/600.0f);
	glTexCoord2f(.75f,1.0f/2+.5f);  glVertex2f(32/800.0f,256/600.0f-64/600.0f);
	if (mouseOver == 2) glColor3fv((float*)&normalColour);*/
	//exit
	if (mouseOver == 3) glColor3fv((float*)&selectColour);
	glTexCoord2f(898/1024.0f,0/2+.5f);  glVertex2f(32/800.0f,192/600.0f-128/600.0f);
	glTexCoord2f(898/1024.0f+128/1024.0f,0/2+.5f);  glVertex2f((32+128)/800.0f,192/600.0f-128/600.0f);
	glTexCoord2f(898/1024.0f+128/1024.0f,.5f/2+.5f);  glVertex2f((32+128)/800.0f,256/600.0f-128/600.0f);
	glTexCoord2f(898/1024.0f,.5f/2+.5f);  glVertex2f(32/800.0f,256/600.0f-128/600.0f);
	glEnd();

	//fake player
	glDepthFunc(GL_ALWAYS);
	shader->UseShader();
	glm::mat4 test = projectionMatrix*glm::scale(glm::translate(glm::mat4(1),glm::vec3(-.6,1.64,-4)),glm::vec3(.025,.05,.025));
	shader->UploadUniforms(&test,&glm::vec4(GetColourFromHue(glfwGetTime()*2),1));
	//fake block
	mesh->LoadMesh();
	mesh->Draw();
	//test = projectionMatrix*glm::scale(glm::translate(glm::mat4(1),glm::vec3(1.3333333,-.70,-4)),glm::vec3(.2f,.1f,.1f));
	test = projectionMatrix*glm::scale(glm::translate(glm::mat4(1),glm::vec3(1.3333333,-.5,-4)),glm::vec3(.2f,.1f,.1f));
	shader->UploadUniforms(&test,&glm::vec4(0,1,0,1));
	mesh->Draw();
	coin->LoadMesh();
	//test = projectionMatrix*glm::scale(glm::rotate(glm::translate(glm::mat4(1),glm::vec3(1.3333333,-.5,-4)),coinSpin,glm::vec3(0,1,0)),glm::vec3(.05f,.05f,.05f));
	test = projectionMatrix*glm::scale(glm::rotate(glm::translate(glm::mat4(1),glm::vec3(1.3333333,-.25,-4)),coinSpin,glm::vec3(0,1,0)),glm::vec3(.05f,.05f,.05f));
	shader->UploadUniforms(&test,&glm::vec4(1,1,0,1));
	coin->Draw();


	glPopMatrix();
	glDepthFunc(GL_LESS);


	TextRenderer::RenderText("INSTRUCTIONS:\n Press Space to Jump\n Press Esc to Exit\n Hold Shift to Phase\n\t(Pass through blocks)\n Press P to Pause\n Collect Treasure\n Try not to fall down!",&colours[WHITE],(256+16)/800.0f,(256-48)/600.0f,24/600.0f,true);
	/*glTexCoord2f(.75f,0.5f/2+.5f);  glVertex2f(32/800.0f,192/600.0f-64/600.0f);
	glTexCoord2f(.75+256/1024.0f,0.5f/2+.5f);  glVertex2f(256/800.0f,192/600.0f-64/600.0f);
	glTexCoord2f(.75+256/1024.0f,1.0f/2+.5f);  glVertex2f(256/800.0f,256/600.0f-64/600.0f);
	glTexCoord2f(.75f,1.0f/2+.5f);  glVertex2f(32/800.0f,256/600.0f-64/600.0f);*/
	TextRenderer::CleanupRenderText();
}

void MenuState::Update(double currentTime, double deltaTime){
	__super::Update(currentTime, deltaTime);
	titleColour = GetColourFromHue(currentTime);
	selectColour = GetColourFromHue(currentTime+PI/2);
	coinSpin = (float)currentTime*360;
}

void MenuState::MouseMove(float mouseX, float mouseY){
	__super::MouseMove(mouseX, mouseY);
	mouseOver = 0;
	if (mouseX >= 32/800.0f && mouseY <= 256/600.0f && mouseY >= 192/600.0f-128/600.0f){
		if (mouseY >= (256-64)/600.0f && mouseX <= (32+128)/800.0f) mouseOver = 1;
		else if (mouseY >= (256-128)/600.0f && mouseY <= (256-64)/600.0f && mouseX <= (32+332)/800.0f) mouseOver = 2;
		else if (mouseY >= (256-192)/600.0f && mouseX <= (32+128)/800.0f) mouseOver = 3;
	}
}

void MenuState::KeyPress(int key, int state){
	if (key == 257 && state == GLFW_PRESS) Game::Exit();
}

void MenuState::MouseButton(int button, int state){
	if (button == GLFW_MOUSE_BUTTON_LEFT && state == GLFW_PRESS && mouseOver != 0){
		switch(mouseOver){
			case 1: Game::TryLoadSong(); break;
			case 2: Game::ChangeState(new ScoreState()); break;
			case 3: Game::Exit(); break;
		}
	}
}

void MenuState::Begin(double currentTime){
	__super::Begin(currentTime);
	UnPause();
	glfwEnable(GLFW_MOUSE_CURSOR);
}

MenuState::MenuState(string filename):GameState(filename, true), titleColour(1,1,1), selectColour(1,0,0), normalColour(1,1,1){
	menuTexture = TextureHandler::LoadTexture(string("textures/menuTexture.tga"),GLFW_BUILD_MIPMAPS_BIT);
	mouseOver = 1;
	shader = DiffuseShader::GetShader();
	mesh = LoadMesh("models/beveled_cube_base.scm");
	coinSpin = 0;
	coin = LoadMesh("models/coin.scm");
}

MenuState::~MenuState(){
	TextureHandler::UnloadTexture(string("textures/menuTexture.tga"));
	UnregisterMesh(mesh);
	UnregisterMesh(coin);
}