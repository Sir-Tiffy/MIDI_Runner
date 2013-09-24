#pragma once
#include "StdAfx.h"
#include "Shader.h"

class DiffuseShader: public Shader{
protected:
	GLint mvpLocation;
	GLint colourLocation;
	static DiffuseShader* instance;
public:
	static DiffuseShader* GetShader();
	void UploadUniforms(glm::mat4* mvp, glm::vec4* colour);
	//void UseShader(glm::mat4* mvp, glm::vec4* colour);
	DiffuseShader();
};