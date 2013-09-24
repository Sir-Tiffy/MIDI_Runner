#include "StdAfx.h"
#include "Shader.h"

void Shader::UseShader(){
	glUseProgram(programID);
	CheckError("Error using shader");
}

GLuint Shader::LoadShader(string filename, GLenum type){
	GLuint id = glCreateShader(type);
	ifstream f(filename,ios::in);
	// shaderSource((istreambuf_iterator<char>(f)),istreambuf_iterator<char>());
	//f.read(shaderSource, length);

	string shaderSource;
	f.seekg(0,ios::end);
	shaderSource.resize((unsigned int)f.tellg());
	f.seekg(0,ios::beg);
	f.read(&shaderSource[0],shaderSource.size());
	f.close();
	
	const GLchar* test[2] = {&shaderSource[0]};

	f.close();
	//shaderSource.push_back(0);
	glShaderSource(id, 1, test,0);
	//delete[] shaderSource;
	glCompileShader(id);

	GLint status;
	glGetShaderiv(id, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE){
		glGetShaderiv(id,GL_INFO_LOG_LENGTH, &status);
		vector<char> log(status);
		glGetShaderInfoLog(id, status, 0, log.data());
		stringstream s;
		s<<"Failed to compile shader \""<<filename<<"\":\n"<<log.data();
		throw runtime_error(s.str());
	}
	return id;
}

Shader::Shader(string fragFilename, string vertFilename){
	programID = glCreateProgram();
	GLuint fragID = LoadShader(fragFilename, GL_FRAGMENT_SHADER);
	GLuint vertID = LoadShader(vertFilename, GL_VERTEX_SHADER);
	glAttachShader(programID, vertID);
	glAttachShader(programID, fragID);
	glLinkProgram(programID);
	glDeleteShader(fragID);
	glDeleteShader(vertID);
	GLint status;
	glGetProgramiv(programID, GL_LINK_STATUS, &status);
	if (status != GL_TRUE){
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &status);
		vector<char> log(status);
		glGetProgramInfoLog(programID, status, 0, log.data());
		stringstream s;
		s<<"Failed to link program F("<<fragFilename<<"), V("<<vertFilename<<"):\n"<<log.data();
		throw runtime_error(s.str());
	}
}

Shader::~Shader(){
	glDeleteProgram(programID);
}
