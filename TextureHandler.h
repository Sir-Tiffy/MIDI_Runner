#pragma once
#include "StdAfx.h"

class TextureHandler{
protected:
	static unordered_map<string, GLuint> textureList;
	static unordered_map<string, unsigned int> textureCount;
public:
	static GLuint LoadTexture(string& filename, int flags = GLFW_BUILD_MIPMAPS_BIT);
	static void UnloadTexture(string& filename);
};