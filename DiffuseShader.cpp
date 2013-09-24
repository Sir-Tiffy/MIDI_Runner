#include "StdAfx.h"
#include "DiffuseShader.h"

void DiffuseShader::UploadUniforms(glm::mat4* mvp, glm::vec4* colour){
	glUniformMatrix4fv(mvpLocation,1,false,(float*)mvp);
	CheckError("Error applying matrix uniform");
	glUniform4fv(colourLocation, 1, (float*)colour);
	CheckError("Error applying colour uniform");
}
/*
void DiffuseShader::UseShader(glm::mat4* mvp, glm::vec4* colour){
	__super::UseShader();
	UploadUniforms(mvp, colour);
}
*/
DiffuseShader* DiffuseShader::GetShader(){
	if (instance == 0){
		instance = new DiffuseShader();
	}
	return instance;
}

DiffuseShader::DiffuseShader()
:Shader(
	"Shaders/fragmentShader.glsl",
	"Shaders/vertexShader.glsl"
){
	mvpLocation = glGetUniformLocation(programID, "mvp");
	colourLocation = glGetUniformLocation(programID, "colour");
} 

DiffuseShader* DiffuseShader::instance = 0;