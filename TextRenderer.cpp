#include "StdAfx.h"
#include "TextureHandler.h"
#include "TextRenderer.h"

unsigned char numDigits(unsigned int num){
	unsigned char result = 1;
	while (num >= 10){
		num /= 10;
		result++;
	}
	return result;
}

void TextRenderer::RenderText(std::string text, const glm::vec4* colour, float left, float top, float height, bool load){
	if (load){
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);
		glDepthFunc(GL_ALWAYS);
		glUseProgram(0);
		CheckError("Error loading text!");
	}
	glColor4fv((float*)colour);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(left*2-1,1-top*2,0);
	glScalef(height,height*2,0);
	glBegin(GL_QUADS);
	int posX = 0;
	int posY = 0;
	for (std::string::iterator i = text.begin(); i != text.end(); i++){
		unsigned char c = (*i);
		switch(c){
			case '\n':
				posY--;
				posX = 0;
				break;
			case '\t':
				posX = ((posX+4)/4)*4;
				break;
			default:{
				float u = (c%16)/16.0f;
				float v = 1-(c/16+1)/8.0f;
			
				glTexCoord2f(u,v+1/8.0f); glVertex2f((GLfloat)posX,(GLfloat)posY);
				glTexCoord2f(u+1/16.0f,v+1/8.0f); glVertex2f(posX+1.0f,(GLfloat)posY);
				glTexCoord2f(u+1/16.0f,v); glVertex2f(posX+1.0f,posY-1.0f);
				glTexCoord2f(u,v); glVertex2f((GLfloat)posX,posY-1.0f);
			}
			case ' ':
				posX++;
		}
	}
	glEnd();
	glPopMatrix();
	CheckError("Error rendering text!");
}

void TextRenderer::CleanupRenderText(){
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDepthFunc(GL_LESS);
	CheckError("Error cleaning up text renderer!");
}

void TextRenderer::Init(){
	texture = TextureHandler::LoadTexture(string("textures/font.tga"));
}
void TextRenderer::Destroy(){
	TextureHandler::UnloadTexture(string("textures/font.tga"));
	texture = 0;
}

GLuint TextRenderer::texture = 0;