#include "stdafx.h"
#include "CheckGLError.h"

const char* GLERRORNAMES[] = {
	"No Error",
	"Invalid Enum",
	"Invalid Value",
	"Invalid Operation",
	"Stack Overflow",
	"Stack Underflow",
	"Out of Memory"
};

void CheckErrorWithInfo(std::string msg, const char* filename, int line){
	GLenum err= glGetError();
	if (err != GL_NO_ERROR){
		std::stringstream stream;
		stream<<msg<<'\n'<<"OpenGL Error in file '"<<filename<<"' line "<<line<<":\n"<<err;
		throw std::runtime_error(stream.str());
	}
}