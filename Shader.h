#pragma once
#include "StdAfx.h"

class Shader{
protected:
	string
		fragFilename,
		vertFilename;
	GLuint programID;
	virtual GLuint LoadShader(string filename, GLenum type);
public:
	void UseShader();
	//virtual void UseShader();

	Shader(string fragFilename, string vertFilename);
	virtual ~Shader();
};