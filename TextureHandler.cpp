#include "StdAfx.h"
#include "TextureHandler.h"
unordered_map<string, GLuint> TextureHandler::textureList;
unordered_map<string, unsigned int> TextureHandler::textureCount;

GLuint TextureHandler::LoadTexture(string& filename, int flags){

	auto i = textureList.find(filename);
	GLuint texture;
	if (i == textureList.end()){
		glGenTextures(1,&texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		if (!glfwLoadTexture2D(filename.c_str(),flags)){
			glDeleteTextures(1,&texture);
			stringstream s;
			s<<"Failed to load texture \""<<filename<<"\"!";
			throw runtime_error(s.str());
		}
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		textureList[filename] = texture;
		textureCount[filename] = 1;
		return texture;
	} else {
		textureCount[filename]++;
		return textureList[filename];
	}
}

void TextureHandler::UnloadTexture(string& filename){
	return;
	auto i = textureList.find(filename);
	if (i == textureList.end()){
		print("tried to unregister unknown texture");
		return;
	}
	if (--textureCount[filename] != 0) return;
	textureCount.erase(filename);
	glDeleteTextures(1,&textureList[filename]);
	textureList.erase(filename);
}