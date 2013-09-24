#include "StdAfx.h"
#include "NumberRenderer.h"
#include "TextureHandler.h"

void NumberRenderer::RenderDigit(unsigned char digit){
	float offset = digit * 48.0f/512;
	glBindTexture(GL_TEXTURE_2D, texture);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glTexCoord2f(offset,0); glVertex2f(0,0);
	glTexCoord2f(offset,1); glVertex2f(0,1);
	glTexCoord2f(offset+48.0f/512,1); glVertex2f(1,1);
	glTexCoord2f(offset+48.0f/512,0); glVertex2f(1,0);
	glEnd();
}

void NumberRenderer::Init(){
	texture = TextureHandler::LoadTexture(string("textures/numbers.tga"));
}

GLuint NumberRenderer::texture = 0;