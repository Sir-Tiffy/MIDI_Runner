#version 110

attribute vec3 vertexPos;
attribute vec3 vertexNormal;

uniform mat4 mvp;

void main(){
	gl_Position = mvp*vec4(vertexPos,1);
}