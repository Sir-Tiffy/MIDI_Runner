#pragma once
#include "StdAfx.h"

unsigned char numDigits(unsigned int num);

class TextRenderer{
protected:
	static GLuint texture;
	glm::vec4 colour;
public:
	static void RenderText(std::string text, const glm::vec4* colour, float left, float top, float height, bool load = false);
	static void CleanupRenderText();
	static void Init();
	static void Destroy();
};