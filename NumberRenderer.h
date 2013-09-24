#pragma once
#include "StdAfx.h"

class NumberRenderer{
	static GLuint texture;
public:
	static void RenderDigit(unsigned char digit);
	static void Init();
};