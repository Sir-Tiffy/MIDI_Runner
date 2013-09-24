#pragma once
#include "StdAfx.h"
#include "GameState.h"
#include "DiffuseShader.h"
#include "MeshHandler.h"

class MenuState:public GameState{
protected:
	 GLuint menuTexture;
	 glm::vec3 titleColour;
	 glm::vec3 selectColour;
	 glm::vec3 normalColour;
	 unsigned char mouseOver;
	 DiffuseShader* shader;
	 Mesh* mesh;
	 Mesh* coin;
	 float coinSpin;

public:
	void Render() override;
	void KeyPress(int key, int state) override;
	void MouseMove(float mouseX, float mouseY) override;
	void MouseButton(int button, int state) override;
	void Update(double currentTime, double deltaTime) override;
	void Begin(double currentTime) override;
	MenuState(string filename);
	~MenuState();
};